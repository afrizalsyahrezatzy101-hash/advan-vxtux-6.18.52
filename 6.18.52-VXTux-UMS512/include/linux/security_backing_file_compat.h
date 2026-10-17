/* V18: Compat shim for security_backing_file_* (missing in VXTux 6.18) */
#ifndef _LINUX_SECURITY_BACKING_FILE_COMPAT_H
#define _LINUX_SECURITY_BACKING_FILE_COMPAT_H

#include <linux/fs.h>

/* Stub out since VXTux 6.18 doesn't have this API */
#ifdef CONFIG_SECURITY
static inline int security_backing_file_alloc(struct file **backing, const struct file *user)
{
    return 0;
}
static inline void security_backing_file_free(struct backing_file *bf)
{
    /* no-op */
}
#else
static inline int security_backing_file_alloc(struct file **backing, const struct file *user)
{
    return 0;
}
static inline void security_backing_file_free(struct backing_file *bf)
{
    /* no-op */
}
#endif

#endif /* _LINUX_SECURITY_BACKING_FILE_COMPAT_H */
