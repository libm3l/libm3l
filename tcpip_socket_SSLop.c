/*
 * copyright � 2012 Adam Jirasek
 */
 
/* ssl_server.c
 * some of the parts of this routine done according to 
 * Sean Walton http://www.cs.utah.edu/~swalton/listings/sockets/programs/
 */


// This product includes software developed by the OpenSSL Project for use in the OpenSSL Toolkit. (http://www.openssl.org/)

#include "Header.h"
#include "format_type.h"
#include "tcpip_socket_SSLop.h"

/*
 * function initializes SSL server and create context
 */
SSL_CTX* init_server_context(void)
{  
	const SSL_METHOD *method;
	SSL_CTX *ctx;
/*
 * load and register all crypto algorithms
 */
	OpenSSL_add_all_algorithms();
/*
 * load error messages
 */
	SSL_load_error_strings();
/*
 * create new-server-method instance
 */
	method = SSLv23_server_method();
/*
 * create new context from method
 */
	if ( (ctx = SSL_CTX_new(method)) == NULL)
		Perror("ctx");
	return ctx;
}



/*
 * function initializes SSL client and create context
 */
SSL_CTX* init_client_context(void)
{  
	const SSL_METHOD *method;
	SSL_CTX *ctx;
/*
 * load and register all crypto algorithms
 */
	OpenSSL_add_all_algorithms();
/*
 * load error messages
 */
	SSL_load_error_strings();
/*
 * create new-server-method instance
 */
	method = SSLv23_client_method();
/*
 * create new context from method
 */
	if ( (ctx = SSL_CTX_new(method)) == NULL)
		Perror("ctx");
	return ctx;
}

void load_ssl_certificates(SSL_CTX* ctx, char* CertFile, char* KeyFile)
{
/*
 * function loads certificates from file
 */
	
/*
 * set the local certificate from CertFile 
 */
	if ( SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0 )
		Perror("SSL_CTX_use_certificate_file");
/* 
 *  set the private key from KeyFile (may be the same as CertFile) 
 */
	if ( SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0 )
		Perror("SSL_CTX_use_PrivateKey_file");
/*
 * verify private key 
 */
	if ( !SSL_CTX_check_private_key(ctx) )
		Perror("SSL_CTX_check_private_key");
}


void show_ssl_certs(SSL* ssl)
{
/*
 * function prints certificates
 */
	X509 *cert;
	char *line;
/*
 * get certificate
 */
	if( (cert = SSL_get_peer_certificate(ssl)) != NULL)
	{
		printf("Server certificates:\n");
		line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
		printf("Subject: %s\n", line);
		free(line);
		line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
		printf("Issuer: %s\n", line);
		free(line);
		X509_free(cert);
	}
	else
	{
		printf("No certificates.\n");
	}
}