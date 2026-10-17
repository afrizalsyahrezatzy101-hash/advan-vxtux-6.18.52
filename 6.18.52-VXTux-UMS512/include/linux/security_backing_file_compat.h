/* V18: Compat shim for security_backing_file_* declarations
 * Missing in some 6.18.x backports
 */
#ifndef _LINUX_SECURITY_BACKING_FILE_COMPAT_H
#define _LINUX_SECURITY_BACKING_FILE_COMPAT_H

#include <linux/fs.h>
#include <linux/backing-file.h>

#ifdef CONFIG_SECURITY
#ifndef security_backing_file_alloc
int security_backing_file_alloc(struct file **backing, const struct file *user);
#endif
#ifndef security_backing_file_free
void security_backing_file_free(struct backing_file *bf);
#endif
#endif /* CONFIG_SECURITY */

#endif /* _LINUX_SECURITY_BACKING_FILE_COMPAT_H */
