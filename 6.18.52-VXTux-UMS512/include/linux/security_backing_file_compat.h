/* V18: Compat shim for security_backing_file_* (missing in VXTux 6.18) */
#ifndef _LINUX_SECURITY_BACKING_FILE_COMPAT_H
#define _LINUX_SECURITY_BACKING_FILE_COMPAT_H

#include <linux/fs.h>

#ifdef CONFIG_SECURITY
/* Match VXTux file_table.c call signatures:
 *   security_backing_file_alloc(&ff->file, user_file)  -- struct file**
 *   security_backing_file_free(&ff->file)              -- struct file*
 */
static inline int security_backing_file_alloc(struct file **backing, const struct file *user)
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
static inline int security_backing_file_alloc(struct file **backing, const struct file *user)
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

#endif /* _LINUX_SECURITY_BACKING_FILE_COMPAT_H */
