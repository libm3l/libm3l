#ifndef __TCPIP_SOCKET_OP_H__
#define __TCPIP_SOCKET_OP_H__

int Sent_to_tcpipsocket(node_t *, const char *, int , char * , ...);
node_t *Send_receive_tcpipsocket(node_t *, const char *, int, char * , ... );
node_t *Receive_send_tcpipsocket(node_t *, const char *, int , char * , ...);
node_t *Receive_tcpipsocket(const char *, int , char *, ...);

#endif
