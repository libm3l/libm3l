
/*                                                                     
       All rights reserved.  Unpublished, all rights reserved under  
       copyright law and international treaty.  Use of a copyright   
       notice is precautionary only and does not imply publication   
       or disclosure.                                                
                                                                     
       Permission to use, copy, modify and distribute this software  
       for any non-commercial purpose and without fee is hereby      
       granted, provided the Adam Jirasek copyright notice appear on all 
       copies of the software. The Adam Jirasek name or Logo may not be  
       used in any advertising or publicity pertaining to the use    
       of the software without the written permission of Adam Jirasek.                                                  
                                                                     
       Permission to use, copy, modify and distribute this software  
       for any commercial purpose requires a fee to be paid to       
       Adam Jirasek. Once the fee is received by Adam Jirasek    
       the latest version of the software will be delivered and a    
       license will be granted for use within an enterprise,         
       provided the Adam Jirasek copyright notice appear on all copies   
       of the software. The Adam Jirasek or Logo may not be used    
       in any advertising or publicity pertaining to the use of the  
       software without the written permission of Adam Jirasek                                                  
                                                                     
       Adam Jirasek makes no warranty or representations     
       about the suitability of the software for any purpose. It is  
       provided "AS IS" without any expressed or implied warranty,   
       including the implied warranties of merchantability, fitness  
       for a particular purpose and non-infringement. Adam Jirasek
       shall not be liable for any direct, indirect,     
       special or consequential damages resulting from the loss of   
       use, data or projects, whether in an action of contract or    
       tort, arising out of or in connection with the use or         
       performance of this software                                  
                                                                     
        Copyright (C) 2012 Adam Jirasek.                 
*/                            


/*
 * function frees memory of the node 
 * copyright ï¿½ 2012 Adam Jirasek
 */
 
#include "Header.h"
#include "format_type.h"
#include "FunctionsPrt.h"
#include "Write2Socket.h"

static int write_buffer(const char *, int);
static int write_file_data(node_t *, size_t , int);

static char *pc, buffer[MAXLINE];
static size_t bitcount;

/*
 * routine writes linked list structure to socket
 */
int write_to_socket(int call, node_t *List,  int socket_descrpt)
{
	node_t *Tmpnode; 
	size_t i, tot_dim, n;
	char buff[MAX_WORD_LENGTH+1];	
	
	void *p;
	void (*foo)(void *);	
/*
 * initial call - nullify counters
 */
	if(call == 1)
		bitcount = 0;
 
	if(List == NULL){
		Warning("WriteData: NULL list");
		return -1;
	} 
 
	if(List->child == NULL){
/*
 * node does not have children, loop over next nodes
 */
	Tmpnode = List;

		if(Tmpnode != NULL){
/*
 * node has children
 */
			while(Tmpnode != NULL){
/*
 * loop over children
 */
				if(Tmpnode->child != NULL){
/*
 * if child has child, call recursively WriteData function
 */
					if(write_to_socket(2, Tmpnode, socket_descrpt) != 0){
						Warning("Write data problem");
						return -1;
					}
/*
 * jump to next child
 */

					Tmpnode = Tmpnode->next;
				}
				else
				{
/*
 * child does not have children
 *
 *
 * make sure you consider empty DIRs, their -> is NULL
 */
					if(strncmp(Tmpnode->type, "DIR", 3) == 0)
					{
						bzero(buff, sizeof(buff));
						if( (n=snprintf(buff, MAX_WORD_LENGTH,"%sSEPAR_SIGN%sSEPAR_SIGN%ldSEPAR_SIGN",Tmpnode->name, Tmpnode->type, Tmpnode->ndim)) < 0)
	      					        Perror("snprintf");
						buff[n] = '\0';
						if( write_buffer(buff, socket_descrpt) == 0 )
							Error("Writing buffer");
					}
					else
					{
/*
 * write only FILE data, if DIR is empty, it will be written in snippet above
 */
						bzero(buff, sizeof(buff));
						if( (n=snprintf(buff, MAX_WORD_LENGTH,"%sSEPAR_SIGN%sSEPAR_SIGN%ldSEPAR_SIGN",Tmpnode->name, Tmpnode->type, Tmpnode->ndim)) < 0)
	      					        Perror("snprintf");
						buff[n] = '\0';
						if( write_buffer(buff, socket_descrpt) == 0 )
							Error("Writing buffer");
/*
 * get field dimensions
 */
						tot_dim = 1;
						for(i=0; i<Tmpnode->ndim; i++){

							bzero(buff, sizeof(buff));
							if( (n=snprintf(buff, MAX_WORD_LENGTH,"%ldSEPAR_SIGN",Tmpnode->fdim[i])) < 0)
	      						        Perror("snprintf");
							buff[n] = '\0';
							if( write_buffer(buff, socket_descrpt) == 0 )
							Error("Writing buffer");

							tot_dim = tot_dim * Tmpnode->fdim[i];
						}
/*
 * got to new line
 */
						if( (n=snprintf(buff, MAX_WORD_LENGTH,"SEPAR_SIGN")) < 0)
	      						        Perror("snprintf");
						buff[n] = '\0';
						if( write_buffer(buff, socket_descrpt) == 0 )
							Error("Writing buffer");
/*
 * write data
 */
						write_file_data(Tmpnode, tot_dim, socket_descrpt);

/*
 * got to new line
 */
						if( (n=snprintf(buff, MAX_WORD_LENGTH,"SEPAR_SIGN")) < 0)
	      						        Perror("snprintf");
						buff[n] = '\0';
						if( write_buffer(buff, socket_descrpt) == 0 )
							Error("Writing buffer");
					}
/*
 * jump to next node
 */
					Tmpnode = Tmpnode->next;
				}
			}
		}
	}
	else
	{
/*
 * node has children, it is type DIR, write its head 
 */
		bzero(buff, sizeof(buff));
		if( (n=snprintf(buff, MAX_WORD_LENGTH,"%sSEPAR_SIGN%sSEPAR_SIGN%ldSEPAR_SIGN",List->name, List->type, List->ndim)) < 0)
	              Perror("snprintf");
		buff[n] = '\0';
		if( write_buffer(buff, socket_descrpt) == 0 )
			Error("Writing buffer");
/*
 * ... and go to its first child. Call WriteData recursivelly
 */
		Tmpnode = List->child;
		
		if(write_to_socket(2, Tmpnode,socket_descrpt) != 0){
			Warning("Write data problem");
			return -1;
		};
	}
/*
 * SEND LAST DATA TO SOCKET, do not forget to send EOMB instruction
 */

	bzero(buff, sizeof(buff));
	if( (n=snprintf(buff, MAX_WORD_LENGTH,EOFbuff)) < 0)
		Perror("snprintf");
	buff[n] = '\0';
	if( write_buffer(buff, socket_descrpt) == 0 )
		Error("Writing buffer");
	
	return 0;
}

/*
 * function writes actual data of the FILE, need to distinguish between the type of field
 */
int write_file_data(node_t *Tmpnode, size_t tot_dim, int socket_descrpt)
{	
	size_t i, n;
	char *pc;
	char buff[MAX_WORD_LENGTH+1];
/*
 * find type of the argument - Floats
 */
		if (strncmp(Tmpnode->type,"LD",2) == 0){  /* long double */
/*
 * loop over all elements of filed
 */
			for (i=0; i<tot_dim; i++){
/*
 * clean buff and write in i-th element of the field
 */
				bzero(buff, sizeof(buff));
				if( (n=snprintf(buff, MAX_WORD_LENGTH,"%lfSEPAR_SIGN", Tmpnode->data.ldf[i])) < 0){
	      			       	Perror("snprintf");
					return -1;
				}
/*
 * set the last element of the buff to \0 and add buff to buffer
 */
				buff[n] = '\0';
				if( write_buffer(buff, socket_descrpt) == 0 ){
					Error("Writing buffer");
					return -1;
				}
			} 
		}
		else if(strncmp(Tmpnode->type,"D",1) == 0){  /* double */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=snprintf(buff, MAX_WORD_LENGTH,"%lfSEPAR_SIGN", Tmpnode->data.df[i])) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if( write_buffer(buff, socket_descrpt) == 0 )
					Error("Writing buffer");
			} 
		}
		else if(strncmp(Tmpnode->type,"F",1) == 0){  /* float */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=snprintf(buff, MAX_WORD_LENGTH,"%lfSEPAR_SIGN", Tmpnode->data.f[i])) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if( write_buffer(buff, socket_descrpt) == 0 )
					Error("Writing buffer");
			} 
		}
/*
 * chars
 */
		else if (strncmp(Tmpnode->type,"SC",2) == 0){  /* signed char */
/*
 * clean buff and make pointer pointing at its beginning
 */
			bzero(buff, sizeof(buff));
			pc = &buff[0];
/*
 * loop over entire field
 */
			for (i=0; i<tot_dim; i++){
/*
 * copy char to buff, once at the end of buff ...
 */
				*pc++ = Tmpnode->data.sc[i];
				if(i == MAX_WORD_LENGTH){
/*
 * set last buff char to \0 and add tu buffer
 */
					*pc = '\0';
					if( write_buffer(buff, socket_descrpt) == 0 )
						Error("Writing buffer");
/*
 * clean buff and make pointer pointing at its beginning
 */
					bzero(buff, sizeof(buff));
					pc = &buff[0];
				}	
			}
		}
		else if(strncmp(Tmpnode->type,"UC",2) == 0){  /* unsigned char */
			bzero(buff, sizeof(buff));
			pc = &buff[0];
			for (i=0; i<tot_dim; i++){
				*pc++ = Tmpnode->data.uc[i];
				if(i == MAX_WORD_LENGTH){
					*pc = '\0';
					if( write_buffer(buff, socket_descrpt) == 0 )
						Error("Writing buffer");
					bzero(buff, sizeof(buff));
					pc = &buff[0];
				}	
			}
		}
		else if(strncmp(Tmpnode->type,"C",1) == 0){  /* char */
			bzero(buff, sizeof(buff));
			pc = &buff[0];
			for (i=0; i<tot_dim; i++){
				*pc++ = Tmpnode->data.c[i];
				if(i == MAX_WORD_LENGTH){
					*pc = '\0';
					if( write_buffer(buff, socket_descrpt) == 0 )
						Error("Writing buffer");
					bzero(buff, sizeof(buff));
					pc = &buff[0];
				}	
			}
		}
/*
 * integers
 */
		else if(strncmp(Tmpnode->type,"ULLI",4) == 0){  /* unsigned long long  int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=snprintf(buff, MAX_WORD_LENGTH,"%lfSEPAR_SIGN", Tmpnode->data.ulli[i])) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if( write_buffer(buff, socket_descrpt) == 0 )
					Error("Writing buffer");
			} 
		}
		else if(strncmp(Tmpnode->type,"SLLI",4) == 0){  /* signed long long  int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=snprintf(buff, MAX_WORD_LENGTH,"%lfSEPAR_SIGN", Tmpnode->data.slli[i])) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if( write_buffer(buff, socket_descrpt) == 0 )
					Error("Writing buffer");
			} 
		}
		else if(strncmp(Tmpnode->type,"ULI",3) == 0){  /* unsigned long int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=snprintf(buff, MAX_WORD_LENGTH,"%lfSEPAR_SIGN", Tmpnode->data.uli[i])) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if( write_buffer(buff, socket_descrpt) == 0 )
					Error("Writing buffer");
			} 
		}
		else if(strncmp(Tmpnode->type,"USI",3) == 0){  /* unsigned short int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=snprintf(buff, MAX_WORD_LENGTH,"%lfSEPAR_SIGN", Tmpnode->data.usi[i])) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if( write_buffer(buff, socket_descrpt) == 0 )
					Error("Writing buffer");
			} 
		}
		else if(strncmp(Tmpnode->type,"SI",2) == 0){  /* short int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=snprintf(buff, MAX_WORD_LENGTH,"%lfSEPAR_SIGN", Tmpnode->data.si[i])) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if( write_buffer(buff, socket_descrpt) == 0 )
					Error("Writing buffer");
			} 
		}
		else if(strncmp(Tmpnode->type,"UI",2) == 0){  /* unsigned int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=snprintf(buff, MAX_WORD_LENGTH,"%lfSEPAR_SIGN", Tmpnode->data.ui[i])) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if( write_buffer(buff, socket_descrpt) == 0 )
					Error("Writing buffer");
			} 
		}
		else if(strncmp(Tmpnode->type,"LI",2) == 0){  /* long  int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=snprintf(buff, MAX_WORD_LENGTH,"%lfSEPAR_SIGN", Tmpnode->data.li[i])) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if( write_buffer(buff, socket_descrpt) == 0 )
					Error("Writing buffer");
			} 
		}
		else if(strncmp(Tmpnode->type,"I",1) == 0){  /* int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=snprintf(buff, MAX_WORD_LENGTH,"%lfSEPAR_SIGN", Tmpnode->data.i[i])) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if( write_buffer(buff, socket_descrpt) == 0 )
					Error("Writing buffer");
			} 
		}
/*
 * counters
 */
		else if(strncmp(Tmpnode->type,"ST",2) == 0){  /* size_t */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=snprintf(buff, MAX_WORD_LENGTH,"%ldSEPAR_SIGN", Tmpnode->data.st[i])) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if( write_buffer(buff, socket_descrpt) == 0 )
					Error("Writing buffer");
			} 
		}
		else if(strncmp(Tmpnode->type,"PTRDF",1) == 0){  /* ptrdf_t */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=snprintf(buff, MAX_WORD_LENGTH,"%ldSEPAR_SIGN", Tmpnode->data.ptrdf[i])) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if( write_buffer(buff, socket_descrpt) == 0 )
					Error("Writing buffer");
			} 
		}
		else
		{
			Error("Unknown type");
		}
/*
 * add buff to buffer
 */		
	if( write_buffer(buff, socket_descrpt) == 0 )
		Error("Writing buffer");
							
}

/*
 * function writes add data in buff to buffer
 * when the buffer is full, it is written to socket and 
 * pointer is set at the beginning of the buffer
 */ 
int write_buffer(const char *buff, int sockfd)
{
	size_t n,i;
     

	while(*buff != '\0'){
		if(bitcount == (MAXLINE-1))
		{
			*(buffer+bitcount) = '\0';
			bitcount = 0;
/*
* SEND TO SOCKET
*/
			if ( (n = write(sockfd,buffer,strlen(buffer))) < 0)
				Perror("write()");
			bzero(buffer, sizeof(buffer));
		}
		*(buffer+bitcount) = *buff++;
		bitcount++;
	}
	
	if(bitcount == (MAXLINE-1))
	{
		*(buffer+bitcount) = '\0';
		bitcount = 0;
/*
* SEND TO SOCKET
*/
		if ( (n = write(sockfd,buffer,strlen(buffer))) < 0)
			Perror("write()");
		bzero(buffer, sizeof(buffer));
	}
	
	return n;
}	