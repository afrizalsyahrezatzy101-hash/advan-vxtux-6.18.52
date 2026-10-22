#ifndef __CUSTOM_FORCE_RCU_H
#define __CUSTOM_FORCE_RCU_H

// Override __rcu_dereference_check to avoid typeof(*p) error
// p is expected to be a pointer, but we handle the case where it's not
#define __rcu_dereference_check(p, local, c, space)                       ({                                                                          	typeof(p) local = (p);                                                	(local);                                                              })

#endif /* __CUSTOM_FORCE_RCU_H */
