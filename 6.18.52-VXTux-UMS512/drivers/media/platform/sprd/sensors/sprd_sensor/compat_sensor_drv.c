/*
 * Copyright (C) 2012 Spreadtrum Communications Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/compat.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <video/sprd_sensor_k.h>
#include "sprd_sensor_drv.h"
#include "compat_sensor_drv.h"

struct compat_sensor_reg_tab_tag {
	compat_caddr_t sensor_reg_tab_ptr;
	uint32_t reg_count;
	uint32_t reg_bits;
	uint32_t burst_mode;
};

struct compat_sensor_i2c_tag {
	compat_caddr_t i2c_data;
	uint16_t i2c_count;
	uint16_t slave_addr;
	uint16_t read_len;
};

struct compat_sensor_otp_data_info_tag {
	uint32_t size;
	compat_caddr_t data_ptr;
};

struct compat_sensor_otp_param_tag {
	uint32_t type;
	uint32_t start_addr;
	uint32_t len;
	compat_caddr_t buff;
	struct compat_sensor_otp_data_info_tag golden;
	struct compat_sensor_otp_data_info_tag awb;
	struct compat_sensor_otp_data_info_tag lsc;
};

struct compat_sensor_muti_aec_i2c_tag {
	compat_caddr_t sensor_id;
	uint16_t id_size;
	compat_caddr_t i2c_slave_addr;
	uint16_t i2c_slave_len;
	compat_caddr_t addr_bits_type;
	uint16_t addr_bits_type_len;
	compat_caddr_t data_bits_type;
	uint16_t data_bits_type_len;
	compat_caddr_t master_i2c_tab;
	uint16_t msize;
	compat_caddr_t slave_i2c_tab;
	uint16_t ssize;
	compat_caddr_t slave_i2c_tab_2;
	uint16_t ssize_2;
};

#define COMPAT_SENSOR_IO_I2C_WRITE_REGS \
	_IOW(SENSOR_IOC_MAGIC,  14, struct compat_sensor_reg_tab_tag)
#define COMPAT_SENSOR_IO_GRC_I2C_WRITE \
	_IOW(SENSOR_IOC_MAGIC,  19, struct compat_sensor_i2c_tag)
#define COMPAT_SENSOR_IO_GRC_I2C_READ \
	_IOWR(SENSOR_IOC_MAGIC, 20, struct compat_sensor_i2c_tag)
#define COMPAT_SENSOR_IO_MUTI_I2C_WRITE \
	_IOW(SENSOR_IOC_MAGIC,  23, struct compat_sensor_muti_aec_i2c_tag)
#define COMPAT_SENSOR_IO_READ_OTPDATA \
	_IOWR(SENSOR_IOC_MAGIC, 254, struct compat_sensor_otp_param_tag)

static long compat_get_sensor_reg_tab_tag(
			struct compat_sensor_reg_tab_tag __user *data32,
			struct sensor_reg_tab_tag *data)
{
	unsigned long c;
	uint32_t i;
	int err;

	err = get_user(c, &data32->sensor_reg_tab_ptr);
	data->sensor_reg_tab_ptr = (struct sensor_reg_tag *)c;
	err |= get_user(i, &data32->reg_count);
	data->reg_count = i;
	err |= get_user(i, &data32->reg_bits);
	data->reg_bits = i;
	err |= get_user(i, &data32->burst_mode);
	data->burst_mode = i;

	return err;
}

static long compat_get_sensor_i2c_tag(
			struct compat_sensor_i2c_tag __user *data32,
			struct sensor_i2c_tag *data)
{
	unsigned long c;
	uint16_t i;
	int err;

	err = get_user(c, &data32->i2c_data);
	data->i2c_data = (uint8_t *)c;
	err |= get_user(i, &data32->i2c_count);
	data->i2c_count = i;
	err |= get_user(i, &data32->slave_addr);
	data->slave_addr = i;
	err |= get_user(i, &data32->read_len);
	data->read_len = i;

	return err;
}

static long compat_get_muti_aec_i2c_tag(
			struct compat_sensor_muti_aec_i2c_tag __user *data32,
			struct sensor_muti_aec_i2c_tag *data)
{
	unsigned long c;
	uint16_t i;
	int err;

	err = get_user(c, &data32->sensor_id);
	data->sensor_id = compat_ptr(c);
	err |= get_user(i, &data32->id_size);
	data->id_size = i;
	err |= get_user(c, &data32->i2c_slave_addr);
	data->i2c_slave_addr = compat_ptr(c);
	err |= get_user(i, &data32->i2c_slave_len);
	data->i2c_slave_len = i;
	err |= get_user(c, &data32->addr_bits_type);
	data->addr_bits_type = compat_ptr(c);
	err |= get_user(i, &data32->addr_bits_type_len);
	data->addr_bits_type_len = i;
	err |= get_user(c, &data32->data_bits_type);
	data->data_bits_type = compat_ptr(c);
	err |= get_user(i, &data32->data_bits_type_len);
	data->data_bits_type_len = i;
	err |= get_user(c, &data32->master_i2c_tab);
	data->master_i2c_tab = compat_ptr(c);
	err |= get_user(i, &data32->msize);
	data->msize = i;
	err |= get_user(c, &data32->slave_i2c_tab);
	data->slave_i2c_tab = compat_ptr(c);
	err |= get_user(i, &data32->ssize);
	data->ssize = i;
	err |= get_user(c, &data32->slave_i2c_tab_2);
	data->slave_i2c_tab_2 = compat_ptr(c);
	err |= get_user(i, &data32->ssize_2);
	data->ssize_2 = i;

	return err;
}

/*
 * Kept for the day sprd_sensor_core.c grows a 64-bit
 * SENSOR_IO_READ_OTPDATA case (it has none today, so nothing calls this
 * yet). 2026-10-10: converted to the kernel-side form like the others.
 */
static long __maybe_unused compat_get_otp_param_tag(
			struct compat_sensor_otp_param_tag __user *data32,
			struct _sensor_otp_param_tag *data)
{
	unsigned long c;
	uint32_t i;
	int err;

	err = get_user(i, &data32->type);
	data->type = i;
	err |= get_user(i, &data32->start_addr);
	data->start_addr = i;
	err |= get_user(i, &data32->len);
	data->len = i;
	err |= get_user(c, &data32->buff);
	data->buff = (uint8_t *)c;

	err |= get_user(i, &data32->golden.size);
	data->golden.size = i;
	err |= get_user(c, &data32->golden.data_ptr);
	data->golden.data_ptr = (void *)c;

	err |= get_user(i, &data32->awb.size);
	data->awb.size = i;
	err |= get_user(c, &data32->awb.data_ptr);
	data->awb.data_ptr = (void *)c;

	err |= get_user(i, &data32->lsc.size);
	data->lsc.size = i;
	err |= get_user(c, &data32->lsc.data_ptr);
	data->lsc.data_ptr = (void *)c;

	return err;
}

/*
 * 2026-10-10 port for 6.18: compat_alloc_user_space() was removed in 5.12,
 * so a 32-bit struct can no longer be translated into a scratch buffer in
 * the caller's address space. The four i2c commands therefore build the
 * 64-bit struct in kernel memory and call the (now non-static) ioctl
 * helpers directly; sprd_sensor_core.c does the same copy_from_user() for
 * its own 64-bit path.
 */
long compat_sensor_k_ioctl(struct file *filp,
			unsigned int cmd, unsigned long arg)
{
	long err = 0;
	struct sprd_sensor_file_tag *p_file;

	if (!filp->f_op || !filp->f_op->unlocked_ioctl)
		return -ENOTTY;

	p_file = filp->private_data;
	if (!p_file)
		return -ENODEV;

	switch (cmd) {
	case COMPAT_SENSOR_IO_I2C_WRITE_REGS:
	{
		struct sensor_reg_tab_tag tab;

		err = compat_get_sensor_reg_tab_tag(compat_ptr(arg), &tab);
		if (err)
			break;
		err = sprd_sensor_io_write_i2c_regs(p_file, &tab);
		break;
	}

	case COMPAT_SENSOR_IO_MUTI_I2C_WRITE:
	{
		struct sensor_muti_aec_i2c_tag tab;

		err = compat_get_muti_aec_i2c_tag(compat_ptr(arg), &tab);
		if (err)
			break;
		err = sprd_sensor_io_muti_write_i2c(p_file, &tab);
		break;
	}

	case COMPAT_SENSOR_IO_GRC_I2C_WRITE:
	{
		struct sensor_i2c_tag tab;

		err = compat_get_sensor_i2c_tag(compat_ptr(arg), &tab);
		if (err)
			break;
		err = sprd_sensor_io_grc_write_i2c(p_file, &tab);
		break;
	}

	case COMPAT_SENSOR_IO_GRC_I2C_READ:
	{
		struct sensor_i2c_tag tab;

		err = compat_get_sensor_i2c_tag(compat_ptr(arg), &tab);
		if (err)
			break;
		err = sprd_sensor_io_grc_read_i2c(p_file, &tab);
		break;
	}

	/*
	 * COMPAT_SENSOR_IO_READ_OTPDATA (nr 254) exists in the uapi header,
	 * but sprd_sensor_core.c has no 64-bit case for it either, so there
	 * is nothing to translate into. It falls through to the same no-op
	 * the 64-bit path gives it.
	 */

	default:
		err = filp->f_op->unlocked_ioctl(filp, cmd,
						(unsigned long)compat_ptr(arg));
		break;
	}

	return err;
}
