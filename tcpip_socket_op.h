#ifndef __CLI_OPEN_SOCKET_H__
#define __CLI_OPEN_SOCKET_H__

int openbindlistensocket(int );

SSL_CTX* init_server_context(void);

SSL_CTX* init_client_context(void);

void load_ssl_certificates(SSL_CTX* , char* , char* );

void show_ssl_certs(SSL*);

#endif
