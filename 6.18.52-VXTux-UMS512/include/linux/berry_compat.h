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

#endif /* _BERRY_COMPAT_H */
