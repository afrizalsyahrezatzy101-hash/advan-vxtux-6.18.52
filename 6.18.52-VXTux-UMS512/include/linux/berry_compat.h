/* V18: Berry compat shims for VXTux 6.18 missing APIs */
#ifndef _BERRY_COMPAT_H
#define _BERRY_COMPAT_H

#include <linux/fs.h>

/* ====== security_backing_file_* (missing in VXTux 6.18) ====== */
/* VXTux 6.18 backing_file struct: contains struct file (NOT pointer)
 * So &ff->file gives struct file* (not struct file**)
 */
#ifdef CONFIG_SECURITY
static inline int security_backing_file_alloc(struct file *backing, const struct file *user)
{
    (void)backing;
    (void)user;
    return 0;
}
static inline void security_backing_file_free(struct file *f)
{
    (void)f;
}
#else
static inline int security_backing_file_alloc(struct file *backing, const struct file *user)
{
    (void)backing;
    (void)user;
    return 0;
}
static inline void security_backing_file_free(struct file *f)
{
    (void)f;
}
#endif

/* ====== FS_USERNS_DELEGATABLE (renamed in newer kernels) ====== */
#ifndef FS_USERNS_DELEGATABLE
#define FS_USERNS_DELEGATABLE FS_USERNS_MOUNT  /* fallback to old name */
#endif


/* ====== futex_exec_done (VXTux uses older name) ====== */
#ifndef futex_exec_done
static inline void futex_exec_done(struct task_struct *tsk)
{
    (void)tsk;
}
#endif

/* ====== may_see_all_namespaces (VXTux doesn't export) ====== */
static inline bool may_see_all_namespaces(void)
{
    return true;  /* Always allow - nsfs is read-only debug */
}

/* ====== PIDFD_INFO_SIZE_VER1 (VXTux only has VER0) ====== */
#ifndef PIDFD_INFO_SIZE_VER1
#define PIDFD_INFO_SIZE_VER1 PIDFD_INFO_SIZE_VER0
#endif

/* ====== security_mmap_backing_file (VXTux doesn't have it) ====== */
static inline int security_mmap_backing_file(struct vm_area_struct *vma,
                                              struct file *file,
                                              struct file *user_file)
{
    (void)vma; (void)file; (void)user_file;
    return 0;
}

/* ====== vsp_pw_on / vsp_pw_off (VXTux tree missing) ====== */
static inline int vsp_pw_on(int domain) { (void)domain; return 0; }
static inline int vsp_pw_off(int domain) { (void)domain; return 0; }
static inline int sprd_cam_pw_on(void) { return 0; }
static inline int sprd_cam_pw_off(void) { return 0; }
static inline int sprd_cam_domain_eb(void) { return 0; }
static inline int sprd_cam_domain_disable(void) { return 0; }

/* ====== soc_pcm_set_dai_params (ASoC compat) ====== */
/* soc_pcm_set_dai_params takes struct snd_soc_dai *dai, void *params in VXTux 6.18 */
static inline int soc_pcm_set_dai_params(void *dai, void *params)
{
    (void)dai; (void)params;
    return 0;
}

/* ====== dev_xmit_recursion (VXTux 6.18 missing) ====== */
static inline int dev_xmit_recursion(void) { return 0; }
static inline void dev_xmit_recursion_inc(void) {}
static inline void dev_xmit_recursion_dec(void) {}

/* ====== sk_msg_sg_copy_assign (BPF msg shim) ====== */
static inline void sk_msg_sg_copy_assign(void *msg, int i, void *from, int move_from)
{
    (void)msg; (void)i; (void)from; (void)move_from;
}

/* ====== netdev_get_by_index_lock (VXTux 6.18 missing) ====== */
static inline struct net_device *netdev_get_by_index_lock(struct net *net, int ifindex)
{
    (void)net; (void)ifindex;
    return NULL;
}
static inline void netdev_get_by_index_unlock(void) {}

/* ====== QCFG_RX_PAGE_SIZE & pp_memory_provider_params.rx_page_size ====== */
#define QCFG_RX_PAGE_SIZE 0
static inline int qops_ndo_default_qcfg(void *qops, void *qcfg) { (void)qops; (void)qcfg; return 0; }

/* ====== fib_rule_get_safe (VXTux 6.18 missing) ====== */
struct fib_rule;
static inline void *fib_rule_get_safe(struct fib_rule *rule, ...)
{
    (void)rule;
    return NULL;
}

/* ====== Qdisc depth (VXTux 6.18 missing) ====== */
static inline int qdisc_depth_get(void *q) { (void)q; return 0; }
static inline void qdisc_depth_set(void *q, int d) { (void)q; (void)d; }
static inline void qdisc_depth_inc(void *q) { (void)q; }

/* ====== kmalloc_obj (VXTux 6.18 missing) ====== */
static inline void *__berry_kmalloc_obj(size_t size, gfp_t flags) { return kmalloc(size, flags); }
#define kmalloc_obj(obj, ...) __berry_kmalloc_obj(sizeof(obj), ##__VA_ARGS__, GFP_KERNEL)

/* ====== sk_psock BPF stubs (VXTux 6.18 missing) ====== */
struct sk_psock;
static inline int sk_psock_dump_add(struct sk_psock *psock, struct sock *sk, void *p)
{
    (void)psock; (void)sk; (void)p;
    return 0;
}

/* ====== sk_psock msg_tot_len accessor (VXTux 6.18 missing) ====== */
static inline size_t sk_psock_msg_tot_len(struct sk_psock *psock) {
    (void)psock; return 0;
}
static inline void sk_psock_msg_tot_len_set(struct sk_psock *psock, size_t v) {
    (void)psock; (void)v;
}
static inline void sk_psock_msg_tot_len_add(struct sk_psock *psock, size_t v) {
    (void)psock; (void)v;
}
#define msg_tot_len sk_psock_msg_tot_len

/* ====== TCA_ACT_FLAGS_AT_INGRESS_OR_CLSACT (VXTux 6.18 missing) ====== */
#ifndef TCA_ACT_FLAGS_AT_CLSACT
#define TCA_ACT_FLAGS_AT_CLSACT ((__u32)0x40)
#endif
#ifndef TCA_ACT_FLAGS_AT_INGRESS_OR_CLSACT
#define TCA_ACT_FLAGS_AT_INGRESS_OR_CLSACT (TCA_ACT_FLAGS_AT_INGRESS | TCA_ACT_FLAGS_AT_CLSACT)
#endif

/* ====== tcf_pedit_nkeys_locked (VXTux 6.18 missing) ====== */
static inline int tcf_pedit_nkeys_locked(...) { return 0; }

#endif /* _BERRY_COMPAT_H */


