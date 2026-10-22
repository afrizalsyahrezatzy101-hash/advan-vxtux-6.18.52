/* Minimal mman.h stub for Windows builds (kconfig/confdata.c only uses mmap) */
#ifndef _SYS_MMAN_H
#define _SYS_MMAN_H

#include <sys/types.h>

#define PROT_READ       0x1
#define PROT_WRITE      0x2
#define PROT_EXEC       0x4
#define PROT_NONE       0x0
#define PROT_GROWSDOWN  0x01000000
#define PROT_GROWSUP    0x02000000

#define MAP_SHARED      0x01
#define MAP_PRIVATE     0x02
#define MAP_TYPE        0x0f
#define MAP_FIXED       0x10
#define MAP_ANONYMOUS   0x20
#define MAP_ANON        MAP_ANONYMOUS
#define MAP_FAILED      ((void *) -1)

#define MAP_NORESERVE   0x4000
#define MAP_POPULATE    0x8000
#define MAP_NONBLOCK    0x10000
#define MAP_STACK       0x20000
#define MAP_HUGETLB     0x40000

#define MS_ASYNC        1
#define MS_INVALIDATE   2
#define MS_SYNC         4

#ifdef __cplusplus
extern "C" {
#endif

extern void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
extern int munmap(void *addr, size_t length);
extern int mprotect(void *addr, size_t length, int prot);
extern int msync(void *addr, size_t length, int flags);
extern int madvise(void *addr, size_t length, int advice);
extern int mlock(const void *addr, size_t length);
extern int munlock(const void *addr, size_t length);

#ifdef __cplusplus
}
#endif

#endif
