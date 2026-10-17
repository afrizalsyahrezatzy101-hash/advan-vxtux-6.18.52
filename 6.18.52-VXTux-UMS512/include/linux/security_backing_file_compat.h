/* V18: Compat shim for security_backing_file_* (missing in VXTux 6.18) */
#ifndef _LINUX_SECURITY_BACKING_FILE_COMPAT_H
#define _LINUX_SECURITY_BACKING_FILE_COMPAT_H

#include <linux/fs.h>

/* VXTux 6.18 backing_file struct: contains struct file (NOT pointer)
 * So &ff->file gives struct file* (not struct file**)
 * 
 * file_table.c calls:
 *   security_backing_file_alloc(&ff->file, user_file)  -- struct file*, const struct file*
 *   security_backing_file_free(&ff->file)              -- struct file*
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

#endif /* _LINUX_SECURITY_BACKING_FILE_COMPAT_H */
