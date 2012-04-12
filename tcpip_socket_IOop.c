
#include "Header.h"
#include "format_type.h"
#include "FunctionsPrt.h"

#include "Client_Open_Socket.h"
#include "Write2Socket.h"
#include "ReadSocket.h"
/*
 * function opens socket, writes data to it and close the socket
 */
int send_to_tcpipsocket(node_t *Lnode, const char *hostname, int portnumber)
{
	
	int socketnr;
	
	if ( (socketnr =  cli_open_socket("localhost", portnumber)) < 0)
		Error("Could not open socket");
	if ( write_to_socket(1, Lnode,  socketnr) < 0)
		Error("Error during writing data to socket");
	if( close(socketnr) == -1)
		Perror("close");
	
	return 1;
}

/*
 * function opens socket, writes data to it, reads data from it and close the socket
 */
node_t *send_receive_tcpipsocket(node_t *Lnode, const char *hostname, int portnumber)
{
	
	
	node_t *Gnode;
	int socketnr;
	
	if ( (socketnr =  cli_open_socket("localhost", portnumber)) < 0)
		Error("Could not open socket");
	if ( write_to_socket(1, Lnode,  socketnr) < 0)
		Error("Error during writing data to socket");
	if( (Gnode = read_socket(socketnr)) == NULL)
		Error("Error during reading data from socket");
	if( close(socketnr) == -1)
		Perror("close");
	
	return Gnode;
}

/*
 * function opens socket, reads data from it , writes data to it and close the socket
 */
node_t *receive_send_tcpipsocket(node_t *Lnode, const char *hostname, int portnumber)
{
	
	
	node_t *Gnode;
	int socketnr;
	
	if ( (socketnr =  cli_open_socket("localhost", portnumber)) < 0)
		Error("Could not open socket");
	if( (Gnode = read_socket(socketnr)) == NULL)
		Error("Error during reading data from socket");
	if ( write_to_socket(1, Lnode,  socketnr) < 0)
		Error("Error during writing data to socket");
	if( close(socketnr) == -1)
		Perror("close");
	
	return Gnode;
}
