/*
 *     Copyright (C) 2012  Adam Jirasek
 * 
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU Lesser General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU Lesser General Public License for more details.
 * 
 *     You should have received a copy of the GNU Lesser General Public License
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *     
 *     contact: libm3l@gmail.com
 * 
 */



/*
 *     Function Write2Socket.c
 *
 *     Date: 2012-06-24
 * 
 * 
 *
 *
 *     Description:
 * 
 *
 *     Input parameters:
 * 
 *
 *     Return value:
 * 
 * 
 *
 *     Modifications:
 *     Date		Version		Patch number		CLA 
 *
 *
 *     Description
 *
 */




 
#include "libm3l_header.h"
#include "format_type.h"
#include "FunctionsPrt.h"
#include "Write2Socket.h"
#include "format_conversion_spec.h"
#include "NumberConversion.h"

static lmint_t m3l_write_buffer(lmchar_t *, lmsize_t *, const lmchar_t *, lmint_t, lmint_t, lmint_t, opts_t *);
inline static lmint_t m3l_write_file_data_intdescprt(lmchar_t *, lmsize_t *,node_t *, lmsize_t , lmint_t, opts_t *);
inline static lmssize_t Write(lmchar_t *,lmint_t ,  lmsize_t);
static lmint_t m3l_write_to_socket_reentrant(lmchar_t *, lmchar_t *, lmsize_t *, lmint_t , node_t *,  lmint_t , opts_t *);
/*
 * routine writes linked list structure to socket
 */

lmint_t m3l_write_to_socket(lmint_t call, node_t *List,  lmint_t socket_descrpt, opts_t *Popts){

// 	lmchar_t *buffer, *buff;
// 	lmsize_t *bitcount;
//
// 	if( (buffer = (lmchar_t *)malloc(sizeof(lmchar_t) * MAXLINE)) == NULL)
// 		Perror("malloc");
// 	if( (buff = (lmchar_t *)malloc(sizeof(lmchar_t) * (MAX_WORD_LENGTH+1))) == NULL)
// 		Perror("malloc");
// 	if( (bitcount = (lmint_t *)malloc(sizeof(lmsize_t)))
// 		Perror("malloc");
	
	lmchar_t buffer[MAXLINE], buff[MAX_WORD_LENGTH+1];
	lmsize_t *bitcount, BITT;
	
	bitcount = &BITT;
	*bitcount = 0;
	
	m3l_write_to_socket_reentrant(buffer, buff, bitcount, 1, List,  socket_descrpt, Popts);
	
// 	free(buffer);
// 	free(buff);
// 	free(bitcount);
}

lmint_t m3l_write_to_socket_reentrant(lmchar_t *buffer,lmchar_t *buff,lmsize_t *bitcount, lmint_t call, node_t *List,  lmint_t socket_descrpt, opts_t *Popts)
{
	node_t *Tmpnode, *Tmplist, *Tmpnext, *Tmpprev;
	lmsize_t i, tot_dim, n;
/*
 * check if list has child or not
 */
	if(List == NULL){
		Warning("WriteData2Socket: NULL list");
		return -1;
	} 
 
	if(List->child == NULL){
/*
 * node does not have children, loop over next nodes
 */
	Tmpnode = List;
/*
 * child does not have children
 *
 *
 * make sure you consider empty DIRs, their -> is NULL
 */
		if(strncmp(Tmpnode->type, "DIR", 3) == 0){
			bzero(buff, sizeof(buff));
			if( (n=snprintf(buff, MAX_WORD_LENGTH,"%s%c%s%c%ld%c",Tmpnode->name, SEPAR_SIGN, Tmpnode->type,SEPAR_SIGN, Tmpnode->ndim, SEPAR_SIGN)) < 0)
    					        Perror("snprintf");
			buff[n] = '\0';
			if( m3l_write_buffer(buffer, bitcount,buff, socket_descrpt,0,0, Popts) == 0 )
				Error("Writing buffer");
		}
		else
		{
/*
 * write only FILE data, if DIR is empty, it will be written in snippet above
 */
			bzero(buff, sizeof(buff));
			if( (n=snprintf(buff, MAX_WORD_LENGTH,"%s%c%s%c%ld%c",Tmpnode->name, SEPAR_SIGN, Tmpnode->type, SEPAR_SIGN, Tmpnode->ndim, SEPAR_SIGN)) < 0)
    					        Perror("snprintf");
			buff[n] = '\0';
			if( m3l_write_buffer(buffer, bitcount,buff, socket_descrpt,0,0, Popts) == 0 )
				Error("Writing buffer");
/*
 * get field dimensions
 */
			if(Tmpnode->ndim > 0){
				tot_dim = 1;
				for(i=0; i<Tmpnode->ndim; i++){
						bzero(buff, sizeof(buff));
					if( (n=snprintf(buff, MAX_WORD_LENGTH,"%ld%c",Tmpnode->fdim[i], SEPAR_SIGN)) < 0)
								Perror("snprintf");
					buff[n] = '\0';
					if( m3l_write_buffer(buffer, bitcount,buff, socket_descrpt,0,0, Popts) == 0 )
					Error("Writing buffer");
						tot_dim = tot_dim * Tmpnode->fdim[i];
				}
/*
 * write data
 */
				m3l_write_file_data_intdescprt(buffer, bitcount, Tmpnode, tot_dim, socket_descrpt, Popts);
			}
		}
/*
 * jump to next node
 */
		Tmpnode = Tmpnode->next;
	}
	else	{
/*
 * node has children, it is type DIR, write its head 
 */
		Tmplist = List;
// 		if( strncmp(List->type, "LINK", 4 ) == 0) 
// 			Tmplist = Tmplist ->child;  /* list is populated by the target list where it points to */

		bzero(buff, sizeof(buff));
		if( (n=snprintf(buff, MAX_WORD_LENGTH,"%s%c%s%c%ld%c",Tmplist->name, SEPAR_SIGN, Tmplist->type, SEPAR_SIGN, Tmplist->ndim, SEPAR_SIGN)) < 0)
	              Perror("snprintf");
		buff[n] = '\0';

		if( m3l_write_buffer(buffer, bitcount,buff, socket_descrpt,0,0, Popts) == 0 )
			Error("Writing buffer");
/*
 * ... and go to its first child. Call write_to_socket recursivelly
 */
		Tmpnode = Tmplist->child;
		while(Tmpnode != NULL){
/*
 * if list is LINK, use the target node (saved in ->child) as a source of data
 */
			if( strncmp(Tmpnode->type, "LINK", 4 ) == 0){    /* NOTE change  */
/*
 * List is link, write the target
 */
				Tmplist=Tmpnode->child;
				if(Tmplist != NULL){
/*
 * save connectivity data and nullify them temporarily
 */
					Tmpnext = Tmplist->next;
					Tmpprev = Tmplist->prev;
					Tmplist->next = NULL;
					Tmplist->prev = NULL;				
					
					if(m3l_write_to_socket_reentrant(buffer, buff, bitcount, 2, Tmplist,socket_descrpt, Popts) != 0){
/*
 * restore original state
 */
						Tmplist->next = Tmpnext;
						Tmplist->prev = Tmpprev;
						Warning("Write data problem");
						return -1;
					}
/*
 * restore original state
 */
					Tmplist->next = Tmpnext;
					Tmplist->prev = Tmpprev;
				}
				else{
/*
 * empty LINK
 */					
					if(m3l_write_to_socket_reentrant(buffer, buff, bitcount, 2, Tmpnode,socket_descrpt, Popts) != 0){
						Warning("Write data problem");
						return -1;
					}
				}
			}				
			else{
				if(m3l_write_to_socket_reentrant(buffer, buff, bitcount, 2, Tmpnode,socket_descrpt, Popts) != 0){
					Warning("Write data problem");
					return -1;
				}
			}
			Tmpnode = Tmpnode->next;
		}
	}
/*
 * SEND LAST DATA TO SOCKET, do not forget to send EOMB instruction if ending initial call (ie call == 1)
 * call m3l_write_buffer with force = 1 to force sending the last piece of information
 */
	if(call == 1){
		bzero(buff, sizeof(buff));
		if( (n=snprintf(buff, MAX_WORD_LENGTH,"%s",EOFbuff)) < 0)
			Perror("snprintf");
		buff[n] = '\0';

		if( m3l_write_buffer(buffer, bitcount,buff, socket_descrpt,1,0, Popts) == 0 )
			Error("Writing buffer");
	}
	return 0;
}

/*
 * function writes actual data of the FILE, need to distinguish between the type of field
 */
lmint_t m3l_write_file_data_intdescprt(lmchar_t *buffer, lmsize_t *bitcount, node_t *Tmpnode, lmsize_t tot_dim, lmint_t socket_descrpt, opts_t *Popts)
{	
	lmsize_t i, n, length, length32, len;
	lmchar_t *pc;
	lmchar_t buff[MAX_WORD_LENGTH+1];
	
	Uint32_t fi;
	Uint64_t di;
	
	void *(*WRtoBuffD)( lmchar_t *, Uint64_t *, lmsize_t);
	void *(*WRtoBuffF)( lmchar_t *, Uint32_t *, lmsize_t);
	
	if(Popts == NULL)
		Error("Write2Socket: NULL Popts");
/*
 * determine buffer copying
 */
	if(Popts->opt_MEMCP == 'M'){
		WRtoBuffD = WR_MemcpyD;
		WRtoBuffF = WR_MemcpyF;
		length    = sizeof(di);  //8;
		length32  = sizeof(fi);  //4;
	}
	else{
		WRtoBuffD = WR_snprintfD;
		WRtoBuffF = WR_MemcpyF;
		length    = sizeof(buff);
	}
/*
 * find type of the argument - Floats
 */
		if (strncmp(Tmpnode->type,"LD",2) == 0){  /* long double */

			if(Popts->opt_tcpencoding == 'I'){  /* IEEE-754 encoding */
				
				for (i=0; i<tot_dim; i++){
					bzero(buff, sizeof(buff));
					di = pack754_128(Tmpnode->data.ldf[i]);
					WRtoBuffD(&buff[0], &di, length);
					if( m3l_write_buffer(buffer, bitcount,buff, socket_descrpt,0,0, Popts) == 0 )
						Error("Writing buffer");
				}
			}
			else if(Popts->opt_tcpencoding == 'r'){    /* Iraw data */
				Error("Raw coding not implemented");
				exit(0);
			}
			else if(Popts->opt_tcpencoding == 't'){ /* text enconding */
/*
 * loop over all elements of filed
 */
				for (i=0; i<tot_dim; i++){
/*
 * clean buff and write in i-th element of the field
 */
					bzero(buff, sizeof(buff));
					if( (n= FCS_W_LD(Tmpnode->data.ldf[i], SEPAR_SIGN)) < 0){
						Perror("snprintf");
						return -1;
					}
/*
 * set the last element of the buff to \0 and add buff to buffer
 */
					buff[n] = '\0';
					if( m3l_write_buffer(buffer, bitcount,buff, socket_descrpt,0,0, Popts) == 0 ){
						Error("Writing buffer");
						return -1;
					}
				}
			} 
		}
		else if(strncmp(Tmpnode->type,"D",1) == 0){  /* double */

			if(Popts->opt_tcpencoding == 'I'){  /* IEEE-754 encoding */
				for (i=0; i<tot_dim; i++){
					bzero(buff, sizeof(buff));
					di = pack754_64(Tmpnode->data.df[i]);
					WRtoBuffD(&buff[0], &di, length);
					if( m3l_write_buffer(buffer, bitcount,buff, socket_descrpt,0,0, Popts) == 0 )
						Error("Writing buffer");
				}
			}
			else if(Popts->opt_tcpencoding == 'r'){    /* Iraw data */
				Error("Raw coding not implemented");
				exit(0);
			}
			else if(Popts->opt_tcpencoding == 't'){ /* text enconding */
				for (i=0; i<tot_dim; i++){
					bzero(buff, sizeof(buff));	
	 				if( (n=FCS_W_D(Tmpnode->data.df[i], SEPAR_SIGN)) < 0)
	 	      			       	Perror("snprintf");
					buff[n] = '\0';
					if( m3l_write_buffer(buffer, bitcount,buff, socket_descrpt,0,0, Popts) == 0 )
						Error("Writing buffer");
				}
			}
		}


		else if(strncmp(Tmpnode->type,"F",1) == 0){  /* float */
			
			if(Popts->opt_tcpencoding == 'I'){   /* IEEE-754 encoding */
				len = sizeof(fi);
				for (i=0; i<tot_dim; i++){
					bzero(buff, sizeof(buff));
					fi = pack754_32(Tmpnode->data.f[i]);
					WRtoBuffF(&buff[0], &fi, length32);
					if( m3l_write_buffer(buffer, bitcount,buff, socket_descrpt,0,0, Popts) == 0 )
						Error("Writing buffer");
				}
			}
			else if(Popts->opt_tcpencoding == 'r'){   /* Iraw data */
				Error("Raw coding not implemented");
				exit(0);
			}
			else if(Popts->opt_tcpencoding == 't'){   /* text enconding */
				for (i=0; i<tot_dim; i++){
					bzero(buff, sizeof(buff));
					if( (n=FCS_W_F(Tmpnode->data.f[i], SEPAR_SIGN)) < 0)
						Perror("snprintf");
					buff[n] = '\0';
					if( m3l_write_buffer(buffer, bitcount,buff, socket_descrpt,0,0, Popts) == 0 )
						Error("Writing buffer");
				} 
			}
		}
/*
 * chars, do not serialize, write as they are
 */
		else if (strncmp(Tmpnode->type,"SC",2) == 0){  /* signed char */
/*
 * clean buff and make pointer pointing at its beginning
 */
			pc = (lmchar_t *)&Tmpnode->data.sc[0];
			if( m3l_write_buffer(buffer, bitcount,pc, socket_descrpt,0,1, Popts) == 0 )
				Error("Writing buffer");
		}
		else if(strncmp(Tmpnode->type,"UC",2) == 0){  /* unsigned char */
			pc = (lmchar_t *)&Tmpnode->data.uc[0];
			if( m3l_write_buffer(buffer, bitcount,pc, socket_descrpt,0,1, Popts) == 0 )
				Error("Writing buffer");
		}
		else if(strncmp(Tmpnode->type,"C",1) == 0){  /* char */
			pc = &Tmpnode->data.c[0];
			if( m3l_write_buffer(buffer, bitcount,pc, socket_descrpt,0,1, Popts) == 0 )
				Error("Writing buffer");
		}
/*
 * integers
 */
		else if(strncmp(Tmpnode->type,"ULLI",4) == 0){  /* unsigned long long  int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_ULLI(Tmpnode->data.ulli[i], SEPAR_SIGN)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if( m3l_write_buffer(buffer, bitcount,buff, socket_descrpt,0,0, Popts) == 0 )
					Error("Writing buffer");
			} 
		}
		else if(strncmp(Tmpnode->type,"SLLI",4) == 0){  /* signed long long  int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_SLLI(Tmpnode->data.slli[i], SEPAR_SIGN)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if( m3l_write_buffer(buffer, bitcount,buff, socket_descrpt,0,0, Popts) == 0 )
					Error("Writing buffer");
			} 
		}
		else if(strncmp(Tmpnode->type,"LLI",3) == 0){  /* signed long long  int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_LLI(Tmpnode->data.lli[i], SEPAR_SIGN)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if( m3l_write_buffer(buffer, bitcount,buff, socket_descrpt,0,0, Popts) == 0 )
					Error("Writing buffer");
			} 
		}
		else if(strncmp(Tmpnode->type,"ULI",3) == 0){  /* unsigned long int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_ULI(Tmpnode->data.uli[i], SEPAR_SIGN)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if( m3l_write_buffer(buffer, bitcount,buff, socket_descrpt,0,0, Popts) == 0 )
					Error("Writing buffer");
			} 
		}
		else if(strncmp(Tmpnode->type,"USI",3) == 0){  /* unsigned short int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_USI(Tmpnode->data.usi[i], SEPAR_SIGN)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if( m3l_write_buffer(buffer, bitcount,buff, socket_descrpt,0,0, Popts) == 0 )
					Error("Writing buffer");
			} 
		}
		else if(strncmp(Tmpnode->type,"SI",2) == 0){  /* short int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_SI(Tmpnode->data.si[i], SEPAR_SIGN)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if( m3l_write_buffer(buffer, bitcount,buff, socket_descrpt,0,0, Popts) == 0 )
					Error("Writing buffer");
			} 
		}
		else if(strncmp(Tmpnode->type,"UI",2) == 0){  /* unsigned int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_UI(Tmpnode->data.ui[i], SEPAR_SIGN)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if( m3l_write_buffer(buffer, bitcount,buff, socket_descrpt,0,0, Popts) == 0 )
					Error("Writing buffer");
			} 
		}
		else if(strncmp(Tmpnode->type,"LI",2) == 0){  /* long  int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_LI(Tmpnode->data.li[i], SEPAR_SIGN)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if( m3l_write_buffer(buffer, bitcount,buff, socket_descrpt,0,0, Popts) == 0 )
					Error("Writing buffer");
			} 
		}
		else if(strncmp(Tmpnode->type,"I",1) == 0){  /* int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_I(Tmpnode->data.i[i], SEPAR_SIGN)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if( m3l_write_buffer(buffer, bitcount,buff, socket_descrpt,0,0, Popts) == 0 )
					Error("Writing buffer");
			} 
		}
/*
 * counters
 */
		else if(strncmp(Tmpnode->type,"ST",2) == 0){  /* size_t */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_ST(Tmpnode->data.st[i], SEPAR_SIGN)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if( m3l_write_buffer(buffer, bitcount,buff, socket_descrpt,0,0, Popts) == 0 )
					Error("Writing buffer");
			} 
		}
		else if(strncmp(Tmpnode->type,"PTRDF",1) == 0){  /* ptrdf_t */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_PTRDF(Tmpnode->data.ptrdf[i], SEPAR_SIGN)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if( m3l_write_buffer(buffer, bitcount,buff, socket_descrpt,0,0, Popts) == 0 )
					Error("Writing buffer");
			} 
		}
		else if( strncmp(Tmpnode->type,"DISKFILE",8) == 0){
			Error("Write2Socket.c: not implemented yet");
		}
		else
		{
			Error("Unknown type");
		}
		
		return 1;
}
/*
 * function writes add data in buff to buffer
 * when the buffer is full, it is written to socket and 
 * pointer is set at the beginning of the buffer
 */ 
lmint_t m3l_write_buffer(lmchar_t *buffer, lmsize_t *bitcount, const lmchar_t *buff, lmint_t sockfd, lmint_t force, lmint_t add, opts_t *Popts)
{
/*
 * Function writes chunks of info in jbuff to buffer which is MAXLINE long before sending it 
 * to TCP/IP socket
 * force - parameter forces buffer to be written to 
 * 	socket even if it is not fully used
 * add   - parameter adds separ sing to the end, it is needed when 
 * 	chars are written in
 */
/*
 * NOTE-URGENT - check the algorithm for adding SEPAR_SIGN at the end of buffer, 
 * especially situation after condition if(bitcount < (MAXLINE-1) && add == 1) when bitcount == MAXLINE-1
 */
	lmsize_t size;
	lmssize_t n;
     
	while(*buff != '\0'){
		if(*bitcount == (MAXLINE-1))
		{
/*
 * end of buffer reached, send add the \0 sign to the very end and 
 * write to socket
 */
			*(buffer+*bitcount) = '\0';
			*bitcount = 0;
/*
* SEND TO SOCKET
*/
			size = strlen(buffer);
			if ( (n = Write(buffer,sockfd,size)) < size)
				Perror("write()");
		}
		*(buffer+*bitcount) = *buff++;
		(*bitcount)++;
	}
	
	if(*bitcount < (MAXLINE-1) && add == 1){
/*
 * add separ sign 
 * NOTE: make sure that if *bitcount here is = MAXLINE-2 and add = force then it adds SEPAR_SIGN here as well
 */
		*(buffer+*bitcount) = SEPAR_SIGN;
		(*bitcount)++;
	}
	
	if(*bitcount == (MAXLINE-1))
	{
		*(buffer+*bitcount) = '\0';
		*bitcount = 0;
/*
* SEND TO SOCKET
*/
		size = strlen(buffer);
		if ( (n = Write(buffer,sockfd,size)) < size)
			Perror("write()");
		
		if(add == 1 ){
/*
 * add separ sign 
 */
			*(buffer+*bitcount) = SEPAR_SIGN;
			(*bitcount)++;
		}
	}
	else if(force == 1){
/*
 * this is the end of sending processs, send everything you have in buffer regardless how long it is.
 * The last sequence of the bugger is -EOMB-
 */
		buffer[*bitcount] = '\0';
// 		size = strlen(buffer);
		size = *bitcount;
		if ( (n = Write(buffer, sockfd,size)) < size)
			Perror("write()");
	}
	return 1;
}


// lmint_t m3l_write_buffer_OC(const lmchar_t *buff, lmint_t sockfd, lmint_t force, lmint_t add, opts_t *Popts)
// {
// /* Function writes chunks of info inbuff to OCbuffer which is MAXLINE_OC long before sending it 
//  * to TCP/IP socket. The OCbuffer is then online compressed and send lined up in buffer (m3l_write_buffer function)
//  * force - parameter forces buffer to be written to 
//  * 	socket even if it is not fully used
//  * add   - parameter adds separ sing to the end, it is needed when 
//  * 	chars are written in
//  */
// 	lmsize_t size;
// 	lmssize_t n;
// 	     
// 	while(*buff != '\0'){
// 		if(bitcount == (MAXLINE_OC-1))
// 		{
// 			*(OCbuffer+bitcount) = '\0';
// 			bitcount = 0;
// /*
// * SEND TO SOCKET
// */
// 			size = strlen(OCbuffer);
// 
// 			if ( (n = Write(sockfd,size)) < size)     
// 				Perror("write()");
// 		}
// 		*(OCbuffer+bitcount) = *buff++;
// 		bitcount++;
// 	}
// 	
// 	if(bitcount < (MAXLINE_OC-1) && add == 1){
// /*
//  * add separ sign 
//  * NOTE: make sure that if bitcount here is = MAXLINE_OC-2 and add = force then it adds SEPAR_SIGN here as well
//  */
// 		*(OCbuffer+bitcount) = SEPAR_SIGN;
// 		bitcount++;
// 	}
// 	
// 	if(bitcount == (MAXLINE_OC-1))
// 	{
// 		*(OCbuffer+bitcount) = '\0';
// 		bitcount = 0;
// /*
// * SEND TO SOCKET
// */
// 		size = strlen(OCbuffer);
// 		if ( (n = Write(sockfd,size)) < size)
// 			Perror("write()");
// 		
// 		if(add == 1 ){
// /*
//  * add separ sign 
//  */
// 			*(OCbuffer+bitcount) = SEPAR_SIGN;
// 			bitcount++;
// 		}
// 	}
// 	else if(force == 1){
// /*
//  * this is the end of sending processs, send everything you have in OCbuffer regardless how long it is.
//  * The last sequence of the bugger is -EOMB-
//  */
// 		size = strlen(OCbuffer);
// 		if ( (n = Write(sockfd,size)) < size)
// 			Perror("write()");
// 	}
// 	return 1;
// }

lmssize_t Write(lmchar_t *buffer, lmint_t sockfd,  lmsize_t size){
/*
 * write buffer to socket
 */

	lmssize_t total, n;	
	total = 0;
	lmchar_t *buff;

	buff = buffer;
	
	while(size > 0) {
		
		if ( (n = write(sockfd,buff,size)) < 0){
			if (errno == EINTR) continue;
			return (total == 0) ? -1 : total;
		}
 		buff += n;
		total += n;
		size -= n;
	}
/*
 * buffer was sent, nullify it
 */
	bzero(buffer, sizeof(buffer));
	return total;
}


/*
 * http://stackoverflow.com/questions/1674162/how-to-handle-eintr-interrupted-system-call
 */ 
