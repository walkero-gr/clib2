/*
 * $Id: inet.h,v 1.5 2022-09-15 12:06:14 clib2devs Exp $
*/

#ifndef	_ARPA_INET_H
#define	_ARPA_INET_H

#ifndef _NETINET_IN_H
#include <netinet/in.h>
#endif /* _NETINET_IN_H */

#ifndef _SYS_SOCKET_H
#include <sys/socket.h>
#endif

#include <features.h>

__BEGIN_DECLS

#ifdef __NO_NET_API
/* This undefs are needed if you try to compile something
 * is using bsdsocket SDK since the functions are already
 * defined using ISocket->func
 */
#undef inet_addr
#undef inet_aton
#undef inet_network
#undef inet_ntop
#undef inet_pton
#endif

extern in_addr_t inet_addr(const char *cp);
extern int inet_aton(const char *cp, struct in_addr *pin);
extern in_addr_t inet_lnaof(struct in_addr in);
extern struct in_addr inet_makeaddr(in_addr_t net, in_addr_t lna);
extern in_addr_t inet_netof(struct in_addr in);
extern in_addr_t inet_network(const char *cp);
extern char *inet_ntoa(struct in_addr in);
extern const char *inet_ntop (int af, const void *src, char *dst, socklen_t cnt);
extern int inet_pton (int af, const char *src, void *dst);

__END_DECLS

#endif /* _ARPA_INET_H */
