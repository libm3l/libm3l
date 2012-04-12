#ifndef __TCPIP_SOCKET_OP_H__
#define __TCPIP_SOCKET_OP_H__

int sent_to_tcpipsocket(node_t *, const char *, int );

node_t *send_receive_tcpipsocket(node_t *, const char *, int );

node_t *receive_send_tcpipsocket(node_t *, const char *, int );

#endif
