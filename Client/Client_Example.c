/*
 * example of client
 * Client creates a data set containing a text and array of int numbers
 * sends it to server and gets back answer with PID of server
 * then send a subset and gets back answer with PID of server
 * Author: Adam Jirasek 
 * Date:   Aug-2012
 */
#include "libm3l.h"

int main(int argc, char *argv[])
{
	node_t *Gnode=NULL, *RecNode=NULL;
	pid_t  childpid;
	size_t *dim;

       find_t *SFounds;

    	 int sockfd, newsockfd, portno, n, status, ch_stat;

       socklen_t clilen;
       struct sockaddr_in cli_addr;
	char *answer="This is answer";

	opts_t *Popts, opts;

	opts.opt_i = '\0'; opts.opt_d = '\0'; opts.opt_f = '\0'; opts.opt_r = 'r'; opts.opt_I = '\0'; opts.opt_k = '\0'; opts.opt_b = '\0';opts.opt_l = '\0';
	opts.opt_add = '\0';
	Popts = &opts;
/*
 * get port number
 */
     if (argc < 3) {
       fprintf(stderr,"ERROR, no IPaddress and port number provided\n");
       exit(1);
     }
 	portno = atoi(argv[2]);
/*
 * read data file with data set containg made up 
*/
 	if( (Gnode = m3l_Fread("DATA_Example" , "--clean_empy_links", (char *)NULL))  == NULL)
 		Perror("Linked_test: m3l_Fread");
/*
 * print data set on screen
 */
	if(m3l_Cat(Gnode, "--all", "-P", "-L",  "*",   (char *)NULL) != 0)
                   Error("CatData");
	printf("\n\n------------------------------------------------------------------------------------------------\n\n\n");

/*
 * send data set to server and get back answer
 */
	if( (RecNode = m3l_Send_receive_tcpipsocket(Gnode,argv[1], portno, "--encoding" , "IEEE-754",  (char *)NULL)) == NULL)
			Perror("Send_receive");
/*
 * print answer set on screen and remove received data set
 */
	printf("\n\nserver answered:\n\n\n");
	if(m3l_Cat(RecNode, "--all", "-P", "-L",  "*",   (char *)NULL) != 0)
                   Error("CatData");
	if(m3l_Umount(&RecNode) != 1)
			Perror("m3l_Umount");
	printf("\n\n------------------------------------------------------------------------------------------------\n\n\n");

/*
 * find a grid1 which contains file with value SV_name_of_grid=CFD_grid
 */
	if ( (SFounds = m3l_locator_caller(Gnode, "/main/grid1", "/*/SV_name_of_grid=CFD_grid", Popts)) == NULL){
		Warning("ln: NULL SFounds");
		return 0;
	}
/*
 * once found, send it over to server and prints answer on screen
 */
	if( (RecNode = m3l_Send_receive_tcpipsocket(SFounds->Found_Nodes[0]->List,argv[1], portno, "--encoding" , "IEEE-754",  (char *)NULL)) == NULL)
			Perror("Send_receive");
	printf("\n\nserver answered:\n\n\n");
	if(m3l_Cat(RecNode, "--all", "-P", "-L",  "*",   (char *)NULL) != 0)
                   Error("CatData");

/*
 * free all allocated data sets
 */
	if(m3l_Umount(&RecNode) != 1)
			Perror("m3l_Umount");
	if(m3l_Umount(&Gnode) != 1)
			Perror("m3l_Umount");	
	m3l_DestroyFound(&SFounds);

     return 0; 
}
