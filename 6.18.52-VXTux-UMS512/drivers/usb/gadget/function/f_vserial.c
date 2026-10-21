// SPDX-License-Identifier: GPL-2.0
/*
 * f_vserial.c - Unisoc USB gadget virtual serial (VSER) function
 *
 * Reconstructed from the stock vendor blob usb_f_vser.ko (kernel 5.4.210,
 * aarch64) for the UMS512 / T618 (sharkl5pro) platform.
 *
 * The vendor blob provides a two-bulk-endpoint function plus two extra
 * paths:
 *
 *   - a "bypass" path where the host-to-device bulk payload is handed
 *     straight to a callback registered by another kernel module
 *     (kernel_vser_register_callback), and
 *   - a /dev/vser misc device so userspace can read and write the same
 *     pipe.
 *
 * Exported API (must match the stock blob exactly):
 *   kernel_vser_register_callback(cb, data)  - register the bulk-IN callback
 *   kernel_vser_set_pass_mode(enable)        - toggle the bypass path
 *   vser_iq_write(buf, len)                  - write to the host from kernel
 *   vser_pass_user_write(buf, len)           - same, bypass flavour
 *
 * The vendor source file was drivers/usb/gadget/function/f_vserial.c; the
 * module was named usb_f_vser.ko and the configfs instance "vser".
 *
 * Copyright (C) 2024 Spreadtrum
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/configfs.h>
#include <linux/usb/composite.h>
#include <linux/usb/gadget.h>
#include <linux/miscdevice.h>
#include <linux/wait.h>
#include <linux/workqueue.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/uaccess.h>

#define VSER_STRING_NAME		"vser"
#define VSER_MAX_INST_NAME_LEN		40
#define VSER_MAX_OPEN			2
#define VSER_TX_REQ_COUNT		4
#define VSER_BUF_SIZE			1024

/* the vendor control request that switches the data path */
#define VSER_SET_CTRL_REQ		0x21
#define VSER_SET_CTRL_REQ_VAL		0x22

/* dev->flag bits */
#define VSER_FLAG_CONNECTED		(1 << 0)
#define VSER_FLAG_WR_ERROR		(1 << 1)
#define VSER_FLAG_SEND_DATA		(1 << 2)
#define VSER_FLAG_TEST_MODE		(1 << 7)

/* dev->mode values */
#define VSER_MODE_NONE			0
#define VSER_MODE_ECHO			1
#define VSER_MODE_PASS			2
#define VSER_MODE_DUAL			3

/*
 * The stock driver keeps its per-request state in a struct whose first
 * member is the list_head and whose usb_request follows it; the requests
 * are chained on two lists, one for the bulk IN direction (requests waiting
 * to be filled) and one for the bulk OUT direction.
 *
 * In 6.18 the usb_request is allocated by the UDC (usb_ep_alloc_request()
 * returns it), so the wrapper holds a pointer to it plus the buffer we
 * own for it.
 */
struct vser_req {
	struct list_head	list;
	struct usb_request	*req;
	char			*buf;
};

struct vser_dev {
	struct usb_function		func;
	struct usb_function_instance	inst;

	struct usb_ep			*in_ep;		/* bulk IN  */
	struct usb_ep			*out_ep;	/* bulk OUT */

	struct usb_request	*in_req;	/* the request we read into */

	struct list_head		tx_list;	/* free bulk IN requests */
	struct list_head		rx_list;	/* queued bulk IN requests */
	spinlock_t			lock;

	wait_queue_head_t		read_wq;
	wait_queue_head_t		write_wq;

	struct workqueue_struct		*wq;
	struct work_struct		test_work;

	int				open_count;
	int				read_sem;
	int				write_sem;
	int				wr_error;
	int				rx_pending;
	int				flag;
	int				mode;
	int				intf;
	int				alt;

	int				in_size;
	int				out_size;

	int				test_interval;
	int				test_time;

	unsigned long			tx_req_count;

	char				*inst_name;
};

static struct vser_dev		*_vser_dev;
static int			tx_req_count;
static unsigned char		s_in_bypass_mode;
static unsigned long		s_callback_data;
static void			(*bulk_in_complete_function)(void *, unsigned int,
							 unsigned long);

static inline struct vser_dev *func_to_vser(struct usb_function *f)
{
	return container_of(f, struct vser_dev, func);
}

static inline struct vser_dev *to_vser_dev(struct config_item *item)
{
	return container_of(item, struct vser_dev, inst.group.cg_item);
}

static inline struct vser_dev *to_vser_inst(struct usb_function_instance *inst)
{
	return container_of(inst, struct vser_dev, inst);
}

/* interface descriptor: */
static struct usb_interface_descriptor vser_interface_desc = {
	.bLength =		USB_DT_INTERFACE_SIZE,
	.bDescriptorType =	USB_DT_INTERFACE,
	/* .bInterfaceNumber = DYNAMIC */
	.bNumEndpoints =	2,
	.bInterfaceClass =	USB_CLASS_VENDOR_SPEC,
	.bInterfaceSubClass =	0,
	.bInterfaceProtocol =	0,
	/* .iInterface = DYNAMIC */
};

static struct usb_endpoint_descriptor vser_fs_in_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,
	.bEndpointAddress =	USB_DIR_IN,
	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
};

static struct usb_endpoint_descriptor vser_fs_out_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,
	.bEndpointAddress =	USB_DIR_OUT,
	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
};

static struct usb_descriptor_header *vser_fs_function[] = {
	(struct usb_descriptor_header *) &vser_interface_desc,
	(struct usb_descriptor_header *) &vser_fs_in_desc,
	(struct usb_descriptor_header *) &vser_fs_out_desc,
	NULL,
};

static struct usb_endpoint_descriptor vser_hs_in_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,
	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize =	cpu_to_le16(512),
};

static struct usb_endpoint_descriptor vser_hs_out_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,
	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize =	cpu_to_le16(512),
};

static struct usb_descriptor_header *vser_hs_function[] = {
	(struct usb_descriptor_header *) &vser_interface_desc,
	(struct usb_descriptor_header *) &vser_hs_in_desc,
	(struct usb_descriptor_header *) &vser_hs_out_desc,
	NULL,
};

static struct usb_endpoint_descriptor vser_ss_in_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,
	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize =	cpu_to_le16(1024),
};

static struct usb_endpoint_descriptor vser_ss_out_desc = {
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,
	.bmAttributes =		USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize =	cpu_to_le16(1024),
};

static struct usb_ss_ep_comp_descriptor vser_ss_bulk_comp_desc = {
	.bLength =		sizeof vser_ss_bulk_comp_desc,
	.bDescriptorType =	USB_DT_SS_ENDPOINT_COMP,
};

static struct usb_descriptor_header *vser_ss_function[] = {
	(struct usb_descriptor_header *) &vser_interface_desc,
	(struct usb_descriptor_header *) &vser_ss_in_desc,
	(struct usb_descriptor_header *) &vser_ss_out_desc,
	(struct usb_descriptor_header *) &vser_ss_bulk_comp_desc,
	NULL,
};

static void vser_complete_out(struct usb_ep *ep, struct usb_request *req);
static void vser_complete_in(struct usb_ep *ep, struct usb_request *req);
static void vser_pass_complete_in(struct usb_ep *ep, struct usb_request *req);
static void vser_free_inst(struct usb_function_instance *inst);
static void vser_free(struct usb_function *f);
static int vser_open(struct inode *inode, struct file *filp);
static int vser_release(struct inode *inode, struct file *filp);
static ssize_t vser_read(struct file *filp, char __user *buf, size_t count,
			 loff_t *ppos);
static ssize_t vser_write(struct file *filp, const char __user *buf,
			  size_t count, loff_t *ppos);

static const struct file_operations vser_fops = {
	.owner		= THIS_MODULE,
	.open		= vser_open,
	.release	= vser_release,
	.read		= vser_read,
	.write		= vser_write,
};

static struct miscdevice vser_device = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= VSER_STRING_NAME,
	.fops	= &vser_fops,
};

/*
 * Free every request we ever queued on the bulk IN endpoint.
 */
static void vser_free_all_request(struct vser_dev *dev)
{
	struct vser_req *req;
	unsigned long flags;

	if (dev->in_req) {
		kfree(dev->in_req->buf);
		usb_ep_free_request(dev->out_ep, dev->in_req);
		dev->in_req = NULL;
	}

	spin_lock_irqsave(&dev->lock, flags);
	while (!list_empty(&dev->tx_list)) {
		req = list_first_entry(&dev->tx_list, struct vser_req, list);
		list_del(&req->list);
		spin_unlock_irqrestore(&dev->lock, flags);
		tx_req_count--;
		kfree(req->buf);
		usb_ep_free_request(dev->in_ep, req->req);
		kfree(req);
		spin_lock_irqsave(&dev->lock, flags);
	}
	spin_unlock_irqrestore(&dev->lock, flags);
}

/*
 * (Re)allocate the request set used by the data path.  Four bulk IN
 * requests are kept in flight at a time, plus one OUT request that
 * collects the host-to-device traffic.
 */
static int vser_alloc_all_request(struct vser_dev *dev)
{
	struct vser_req *req;
	unsigned long flags;
	int i;

	/*
	 * 6.18 hands the usb_request back from usb_ep_alloc_request(); the
	 * UDC owns the struct, we own the data buffer.
	 */
	dev->in_req = usb_ep_alloc_request(dev->out_ep, GFP_KERNEL);
	if (!dev->in_req)
		return -ENOMEM;

	dev->in_req->complete = vser_complete_out;
	dev->in_req->length = 0;
	dev->in_req->context = dev;

	dev->in_req->buf = kmalloc(dev->out_size, GFP_KERNEL);
	if (!dev->in_req->buf) {
		usb_ep_free_request(dev->out_ep, dev->in_req);
		dev->in_req = NULL;
		return -ENOMEM;
	}

	for (i = 0; i < VSER_TX_REQ_COUNT; i++) {
		req = kmalloc(sizeof(*req), GFP_KERNEL);
		if (!req) {
			vser_free_all_request(dev);
			return -ENOMEM;
		}

		req->req = usb_ep_alloc_request(dev->in_ep, GFP_KERNEL);
		if (!req->req) {
			kfree(req);
			vser_free_all_request(dev);
			return -ENOMEM;
		}

		req->req->complete = s_in_bypass_mode ? vser_pass_complete_in :
						      vser_complete_in;
		req->req->context = req;
		req->req->length = 0;
		INIT_LIST_HEAD(&req->list);

		req->buf = kmalloc(dev->in_size, GFP_KERNEL);
		if (!req->buf) {
			usb_ep_free_request(dev->in_ep, req->req);
			kfree(req);
			vser_free_all_request(dev);
			return -ENOMEM;
		}
		req->req->buf = req->buf;

		spin_lock_irqsave(&dev->lock, flags);
		list_add_tail(&req->list, &dev->tx_list);
		tx_req_count++;
		spin_unlock_irqrestore(&dev->lock, flags);
	}
	return 0;
}

/*
 * Claim the two bulk endpoints and get the data path moving.
 */
static int vser_create_bulk_endpoints(struct vser_dev *dev)
{
	struct usb_gadget *gadget = dev->func.config->cdev->gadget;
	struct usb_ep *ep_in, *ep_out;
	int ret;

	ep_in = usb_ep_autoconfig(gadget, &vser_fs_in_desc);
	if (!ep_in) {
		dev_err(&gadget->dev, "usb_ep_autoconfig for ep_in failed\n");
		return -ENOMEM;
	}
	ep_in->driver_data = dev;
	dev->in_ep = ep_in;

	ep_out = usb_ep_autoconfig(gadget, &vser_fs_out_desc);
	if (!ep_out) {
		dev_err(&gadget->dev,
			"usb_ep_autoconfig for vser ep_out failed\n");
		return -ENOMEM;
	}
	ep_out->driver_data = dev;
	dev->out_ep = ep_out;

	ret = vser_alloc_all_request(dev);
	if (ret)
		return ret;

	return 0;
}

/*
 * The host finished sending us data on the OUT endpoint.
 */
static void vser_complete_out(struct usb_ep *ep, struct usb_request *req)
{
	struct vser_dev *dev = ep->driver_data;

	if (req->status)
		dev->wr_error = 1;
	dev->rx_pending = 1;
	wake_up(&dev->read_wq);
}

/*
 * The host took our data on the IN endpoint.
 */
static void vser_complete_in(struct usb_ep *ep, struct usb_request *req)
{
	struct vser_dev *dev = ep->driver_data;
	struct vser_req *vreq = req->context;
	unsigned long flags;

	if (req->status)
		dev->wr_error = 1;

	spin_lock_irqsave(&dev->lock, flags);
	list_add_tail(&vreq->list, &dev->tx_list);
	tx_req_count++;
	spin_unlock_irqrestore(&dev->lock, flags);

	wake_up(&dev->write_wq);
}

/*
 * Bypass flavour of the completion handler: the payload is handed straight
 * to the callback registered by kernel_vser_register_callback().
 */
static void vser_pass_complete_in(struct usb_ep *ep, struct usb_request *req)
{
	struct vser_dev *dev = ep->driver_data;
	struct vser_req *vreq = req->context;
	unsigned long flags;

	if (req->status)
		dev->wr_error = 1;

	spin_lock_irqsave(&dev->lock, flags);
	list_add_tail(&vreq->list, &dev->tx_list);
	tx_req_count++;
	spin_unlock_irqrestore(&dev->lock, flags);

	if (bulk_in_complete_function)
		bulk_in_complete_function(req->buf, req->actual,
					  s_callback_data);

	wake_up(&dev->write_wq);
}

/*
 * Write len bytes from the kernel to the host.
 */
int vser_iq_write(void *buf, int len)
{
	struct vser_dev *dev = _vser_dev;
	struct vser_req *req;
	unsigned long flags;
	int ret;

	if (!dev || !dev->flag)
		return -ENODEV;
	if (!dev->in_ep || !dev->in_ep->desc ||
	    !dev->in_ep->desc->bEndpointAddress)
		return -ENODEV;

	if (len <= 0)
		return 0;

	for (;;) {
		spin_lock_irqsave(&dev->lock, flags);
		if (!list_empty(&dev->tx_list)) {
			req = list_first_entry(&dev->tx_list,
					       struct vser_req, list);
			list_del(&req->list);
			tx_req_count--;
			spin_unlock_irqrestore(&dev->lock, flags);
			break;
		}
		spin_unlock_irqrestore(&dev->lock, flags);

		if (wait_event_interruptible_timeout(dev->write_wq,
				!list_empty(&dev->tx_list),
				msecs_to_jiffies(100)) <= 0)
			return -ERESTARTSYS;
	}

	req->req->length = len;
	req->req->buf = buf;

	ret = usb_ep_queue(dev->in_ep, req->req, GFP_KERNEL);
	if (ret < 0) {
		dev->wr_error = 1;
		spin_lock_irqsave(&dev->lock, flags);
		list_add_tail(&req->list, &dev->tx_list);
		tx_req_count++;
		spin_unlock_irqrestore(&dev->lock, flags);
		return -EIO;
	}

	return len;
}
EXPORT_SYMBOL(vser_iq_write);

/*
 * Same as vser_iq_write() but flagged as coming from the bypass path.
 */
int vser_pass_user_write(void *buf, int len)
{
	struct vser_dev *dev = _vser_dev;
	struct vser_req *req;
	unsigned long flags;
	int ret;

	if (!dev || !dev->flag)
		return -ENODEV;
	if (!dev->in_ep || !dev->in_ep->desc ||
	    !dev->in_ep->desc->bEndpointAddress)
		return -ENODEV;

	if (len <= 0)
		return 0;

	for (;;) {
		spin_lock_irqsave(&dev->lock, flags);
		if (!list_empty(&dev->tx_list)) {
			req = list_first_entry(&dev->tx_list,
					       struct vser_req, list);
			list_del(&req->list);
			tx_req_count--;
			spin_unlock_irqrestore(&dev->lock, flags);
			break;
		}
		spin_unlock_irqrestore(&dev->lock, flags);

		if (wait_event_interruptible_timeout(dev->write_wq,
				!list_empty(&dev->tx_list),
				msecs_to_jiffies(100)) <= 0)
			return -ERESTARTSYS;
	}

	req->req->length = len;
	req->req->buf = buf;

	ret = usb_ep_queue(dev->in_ep, req->req, GFP_KERNEL);
	if (ret < 0) {
		dev->wr_error = 1;
		spin_lock_irqsave(&dev->lock, flags);
		list_add_tail(&req->list, &dev->tx_list);
		tx_req_count++;
		spin_unlock_irqrestore(&dev->lock, flags);
		return -EIO;
	}

	return len;
}
EXPORT_SYMBOL(vser_pass_user_write);

/*
 * Register the callback that receives the bulk IN payload in bypass mode.
 */
void kernel_vser_register_callback(void (*cb)(void *, unsigned int,
						     unsigned long),
					  unsigned long data)
{
	bulk_in_complete_function = cb;
	s_callback_data = data;
}
EXPORT_SYMBOL(kernel_vser_register_callback);

/*
 * Toggle the bypass path.
 */
void kernel_vser_set_pass_mode(unsigned char enable)
{
	s_in_bypass_mode = enable & 1;
}
EXPORT_SYMBOL(kernel_vser_set_pass_mode);

/*
 * Vendor control request handler: lets the host switch the data path
 * between echo, pass and dual mode, and arm or disarm the test pattern.
 */
static int vser_setup(struct usb_function *func,
		       const struct usb_ctrlrequest *ctrl)
{
	struct vser_dev *dev = func_to_vser(func);
	int ret = -EINVAL;

	switch (ctrl->bRequestType & 0x60) {
	case 0x40:	/* vendor */
		switch (ctrl->wValue) {
		case 0x00:	/* connect */
			dev->flag |= VSER_FLAG_CONNECTED;
			break;
		case 0x01:	/* disconnect */
			dev->flag &= ~VSER_FLAG_CONNECTED;
			break;
		case 0x10:	/* arm the test pattern */
			vser_free_all_request(dev);
			vser_alloc_all_request(dev);
			dev->test_interval = (ctrl->wValue << 16) | ctrl->wValue;
			dev->flag |= VSER_FLAG_TEST_MODE;
			queue_work(dev->wq, &dev->test_work);
			break;
		case 0x20:	/* disarm the test pattern */
			vser_free_all_request(dev);
			break;
		default:
			return -EINVAL;
		}
		break;
	default:
		if (ctrl->bRequestType == VSER_SET_CTRL_REQ &&
		    ctrl->bRequest == VSER_SET_CTRL_REQ_VAL) {
			dev->mode = (ctrl->wValue >> 8) & 0xff;
			ret = 0;
		}
		break;
	}

	return ret;
}

/*
 * Bind the function to a configuration.
 */
static int vser_function_bind(struct usb_configuration *c,
			       struct usb_function *f)
{
	struct vser_dev *dev = func_to_vser(f);
	int ret;

	dev->func.config = c;

	ret = usb_interface_id(c, f);
	if (ret < 0)
		return ret;

	dev->intf = ret;

	ret = vser_create_bulk_endpoints(dev);
	if (ret)
		return ret;

	return 0;
}

/*
 * Unbind: give every request back.
 */
static void vser_function_unbind(struct usb_configuration *c,
				  struct usb_function *f)
{
	struct vser_dev *dev = func_to_vser(f);
	struct vser_req *req;
	unsigned long flags;

	vser_free_all_request(dev);

	spin_lock_irqsave(&dev->lock, flags);
	while (!list_empty(&dev->rx_list)) {
		req = list_first_entry(&dev->rx_list, struct vser_req, list);
		list_del(&req->list);
		spin_unlock_irqrestore(&dev->lock, flags);
		tx_req_count--;
		usb_ep_free_request(dev->in_ep, req->req);
		kfree(req->buf);
		kfree(req);
		spin_lock_irqsave(&dev->lock, flags);
	}
	spin_unlock_irqrestore(&dev->lock, flags);

	dev->flag = 0;
	dev->mode = VSER_MODE_NONE;
}

/*
 * Enable the endpoints once the host selected an alternate setting.
 */
static int vser_function_set_alt(struct usb_function *f, unsigned intf,
				 unsigned alt)
{
	struct vser_dev *dev = func_to_vser(f);
	struct usb_gadget *gadget = dev->func.config->cdev->gadget;
	int ret;

	dev->intf = intf;
	dev->alt = alt;

	ret = config_ep_by_speed(gadget, f, dev->in_ep);
	if (ret)
		return ret;

	ret = usb_ep_enable(dev->in_ep);
	if (ret)
		return ret;

	ret = config_ep_by_speed(gadget, f, dev->out_ep);
	if (ret) {
		usb_ep_disable(dev->in_ep);
		return ret;
	}

	ret = usb_ep_enable(dev->out_ep);
	if (ret) {
		usb_ep_disable(dev->in_ep);
		return ret;
	}

	return 0;
}

/*
 * Tear the endpoints down.
 */
static void vser_function_disable(struct usb_function *f)
{
	struct vser_dev *dev = func_to_vser(f);

	dev->flag = 0;
	dev->mode = VSER_MODE_NONE;
	usb_ep_disable(dev->in_ep);
	usb_ep_disable(dev->out_ep);
	wake_up(&dev->read_wq);
	wake_up(&dev->write_wq);
}

/*
 * The periodic test-pattern worker.  It keeps the data path exercised so
 * the host sees a healthy device even when nothing else is talking to it.
 */
static void vser_test_works(struct work_struct *work)
{
	struct vser_dev *dev = container_of(work, struct vser_dev, test_work);
	struct vser_req *req;
	unsigned long flags;
	int remain;

	remain = dev->test_interval;
	if (remain <= 0)
		remain = VSER_BUF_SIZE;

	while (dev->flag && (dev->flag & VSER_FLAG_TEST_MODE)) {
		spin_lock_irqsave(&dev->lock, flags);
		if (list_empty(&dev->tx_list)) {
			spin_unlock_irqrestore(&dev->lock, flags);
			if (wait_event_interruptible_timeout(dev->write_wq,
					!list_empty(&dev->tx_list),
					msecs_to_jiffies(100)) <= 0)
				break;
			continue;
		}
		req = list_first_entry(&dev->tx_list, struct vser_req, list);
		list_del(&req->list);
		tx_req_count--;
		spin_unlock_irqrestore(&dev->lock, flags);

		if (remain <= dev->in_size) {
			memset(req->buf, 'x', remain);
			req->req->length = remain;
		} else {
			memset(req->buf, 'x', dev->in_size);
			req->req->length = dev->in_size;
		}
		req->req->buf = req->buf;

		if (usb_ep_queue(dev->in_ep, req->req, GFP_KERNEL) < 0) {
			dev->wr_error = 1;
			break;
		}
		remain -= req->req->length;
		if (remain <= 0)
			break;
	}

	dev->flag &= ~VSER_FLAG_TEST_MODE;
}

/*
 * configfs plumbing.
 */
static void vser_attr_release(struct config_item *item)
{
	struct vser_dev *dev = to_vser_dev(item);

	usb_put_function_instance(&dev->inst);
}

static struct configfs_item_operations vser_item_ops = {
	.release	= vser_attr_release,
};

static struct configfs_group_operations vser_group_ops = {
};

static struct config_item_type vser_func_type = {
	.ct_item_ops	= &vser_item_ops,
	.ct_group_ops	= &vser_group_ops,
	.ct_owner	= THIS_MODULE,
};

/*
 * Give the configfs instance its name.
 */
static int vser_set_inst_name(struct usb_function_instance *inst,
			      const char *name)
{
	struct vser_dev *dev = to_vser_inst(inst);
	size_t len;

	len = strlen(name);
	if (len + 1 > VSER_MAX_INST_NAME_LEN)
		return -EINVAL;

	dev->inst_name = kstrndup(name, len + 1, GFP_KERNEL);
	if (!dev->inst_name)
		return -ENOMEM;

	return 0;
}

/*
 * Create the per-instance state.
 */
static struct usb_function_instance *vser_alloc_inst(void)
{
	struct vser_dev *dev;
	int ret;

	dev = kzalloc(sizeof(*dev), GFP_KERNEL);
	if (!dev)
		return NULL;

	dev->inst.set_inst_name = vser_set_inst_name;
	dev->inst.free_func_inst = vser_free_inst;

	_vser_dev = dev;

	INIT_LIST_HEAD(&dev->tx_list);
	INIT_LIST_HEAD(&dev->rx_list);
	spin_lock_init(&dev->lock);
	init_waitqueue_head(&dev->read_wq);
	init_waitqueue_head(&dev->write_wq);

	dev->in_size = VSER_BUF_SIZE;
	dev->out_size = VSER_BUF_SIZE;
	dev->open_count = 0;
	dev->read_sem = 0;
	dev->write_sem = 0;
	dev->mode = VSER_MODE_NONE;

	INIT_WORK(&dev->test_work, vser_test_works);
	dev->wq = alloc_workqueue("VserTestWq",
				  WQ_UNBOUND | WQ_MEM_RECLAIM | WQ_FREEZABLE, 1);
	if (!dev->wq) {
		kfree(dev);
		return NULL;
	}

	ret = misc_register(&vser_device);
	if (ret) {
		destroy_workqueue(dev->wq);
		kfree(dev);
		return NULL;
	}

	config_group_init_type_name(&dev->inst.group, VSER_STRING_NAME,
				    &vser_func_type);

	return &dev->inst;
}

/*
 * Release the per-instance state.
 */
static void vser_free_inst(struct usb_function_instance *inst)
{
	struct vser_dev *dev = to_vser_inst(inst);

	kfree(dev->inst_name);
	kfree(dev);
	misc_deregister(&vser_device);
	kfree(_vser_dev);
	_vser_dev = NULL;
}

/*
 * Create the per-function state.
 */
static struct usb_function *vser_alloc(struct usb_function_instance *inst)
{
	struct vser_dev *dev = to_vser_inst(inst);

	dev->func.name = VSER_STRING_NAME;
	dev->func.bind = vser_function_bind;
	dev->func.unbind = vser_function_unbind;
	dev->func.set_alt = vser_function_set_alt;
	dev->func.disable = vser_function_disable;
	dev->func.setup = vser_setup;
	dev->func.free_func = vser_free;

	dev->func.fs_descriptors = vser_fs_function;
	dev->func.hs_descriptors = vser_hs_function;
	dev->func.ss_descriptors = vser_ss_function;

	return &dev->func;
}

static void vser_free(struct usb_function *f)
{
}

/*
 * The misc device lets userspace read and write the same pipe.
 */
static int vser_open(struct inode *inode, struct file *filp)
{
	struct vser_dev *dev = _vser_dev;
	unsigned long flags;

	spin_lock_irqsave(&dev->lock, flags);
	filp->private_data = dev;
	if (dev->open_count >= VSER_MAX_OPEN) {
		spin_unlock_irqrestore(&dev->lock, flags);
		return -EBUSY;
	}
	dev->open_count++;
	spin_unlock_irqrestore(&dev->lock, flags);
	return 0;
}

static int vser_release(struct inode *inode, struct file *filp)
{
	struct vser_dev *dev = _vser_dev;
	unsigned long flags;

	spin_lock_irqsave(&dev->lock, flags);
	if (dev->open_count > 0)
		dev->open_count--;
	spin_unlock_irqrestore(&dev->lock, flags);
	return 0;
}

static ssize_t vser_read(struct file *filp, char __user *buf, size_t count,
			 loff_t *ppos)
{
	struct vser_dev *dev = filp->private_data;
	int ret;

	if (!dev || !dev->flag)
		return -ENODEV;
	if (!dev->in_req)
		return -ENODEV;
	if (count > (size_t)dev->out_size)
		return -EINVAL;

	ret = wait_event_interruptible(dev->read_wq,
				       dev->rx_pending || dev->wr_error);
	if (ret)
		return ret;

	if (dev->wr_error) {
		dev->wr_error = 0;
		return -EIO;
	}

	dev->rx_pending = 0;
	ret = copy_to_user(buf, dev->in_req->buf,
			   min_t(size_t, count, dev->in_req->actual));
	if (ret)
		return -EFAULT;

	return min_t(size_t, count, dev->in_req->actual);
}

static ssize_t vser_write(struct file *filp, const char __user *buf,
			  size_t count, loff_t *ppos)
{
	struct vser_dev *dev = filp->private_data;
	struct vser_req *req;
	unsigned long flags;
	ssize_t ret;
	size_t chunk;

	if (!dev || !dev->flag)
		return -ENODEV;

	if (s_in_bypass_mode) {
		msleep(100);
		return count;
	}

	chunk = count;
	while (chunk) {
		size_t this_len = min_t(size_t, chunk, (size_t)dev->in_size);

		spin_lock_irqsave(&dev->lock, flags);
		if (!list_empty(&dev->tx_list)) {
			req = list_first_entry(&dev->tx_list,
					       struct vser_req, list);
			list_del(&req->list);
			tx_req_count--;
			spin_unlock_irqrestore(&dev->lock, flags);
		} else {
			spin_unlock_irqrestore(&dev->lock, flags);
			if (wait_event_interruptible_timeout(dev->write_wq,
					!list_empty(&dev->tx_list),
					msecs_to_jiffies(100)) <= 0)
				return -ERESTARTSYS;
			continue;
		}

		if (copy_from_user(req->buf, buf, this_len)) {
			spin_lock_irqsave(&dev->lock, flags);
			list_add_tail(&req->list, &dev->tx_list);
			tx_req_count++;
			spin_unlock_irqrestore(&dev->lock, flags);
			return -EFAULT;
		}

		req->req->length = this_len;
		req->req->buf = req->buf;
		ret = usb_ep_queue(dev->in_ep, req->req, GFP_KERNEL);
		if (ret < 0) {
			dev->wr_error = 1;
			spin_lock_irqsave(&dev->lock, flags);
			list_add_tail(&req->list, &dev->tx_list);
			tx_req_count++;
			spin_unlock_irqrestore(&dev->lock, flags);
			return -EIO;
		}

		buf += this_len;
		chunk -= this_len;
	}

	return count;
}

static struct usb_function_driver vserusb_func = {
	.name		= "usb_f_vser",
	.mod		= THIS_MODULE,
	.alloc_inst	= vser_alloc_inst,
	.alloc_func	= vser_alloc,
};

static int __init vser_mod_init(void)
{
	int ret;

	ret = usb_function_register(&vserusb_func);
	if (ret) {
		pr_err("Error: Create vser function failed.\n");
		return ret;
	}

	return 0;
}

static void __exit vser_mod_exit(void)
{
	usb_function_unregister(&vserusb_func);
}

module_init(vser_mod_init);
module_exit(vser_mod_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("SPRD USB VSER");
MODULE_ALIAS("usbfunc:vser");
