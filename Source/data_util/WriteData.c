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
 *     Function WriteData.c
 *
 *     Date: 2012-06-28
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
#include "WriteData.h"
#include "format_conversion_spec.h"

#define SEPAR_SPACE ' '

static lmint_t m3l_write_file_data_filedescprt(node_t *, lmsize_t , FILE *);

/*
 * routine writes linked list structure to the file
 */
lmint_t m3l_WriteData(lmint_t call, node_t *List,  FILE *fp)
{
	node_t *Tmpnode, *Tmplist, *Tmpnext, *Tmpprev;
	lmsize_t i, n, tot_dim;
	lmchar_t buff[MAX_WORD_LENGTH];
 
	if(List == NULL){
		Warning("WriteData: NULL list");
		return -1;
	} 
 
// 	printf(" name of node is %s, type is %s child is %p ndim is %ld\n", List->name, List->type, List->child, List->ndim);

	if(List->child == NULL ){
/*
 * loop over next nodes
 */
	Tmpnode = List;

		if(strncmp(Tmpnode->type, "DIR", 3) == 0){
			bzero(buff, sizeof(buff));
			if( snprintf(buff, MAX_WORD_LENGTH,"%s %s %ld\n",Tmpnode->name, Tmpnode->type, Tmpnode->ndim) < 0)
				Perror("snprintf");
			
// 			if ( fwrite (&buff ,strlen(buff)*sizeof(char),  1 , fp ) < 1)
 			if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp ) < strlen(buff))
				Perror("fwrite");
		}
		else
		{
/*
 * write only FILE data, if DIR is empty, it will be written here too
 */
			bzero(buff, sizeof(buff));
			if( snprintf(buff, MAX_WORD_LENGTH,"%s %s %ld ",Tmpnode->name, Tmpnode->type, Tmpnode->ndim) < 0)
    					        Perror("snprintf");
			if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
				Perror("fwrite");

			if(Tmpnode->ndim > 0){
/*
 * when writing character, decrese dimensions, an extra dimensions was allocated for /0 terminating character
 */
				tot_dim = 1;
				for(i=0; i<Tmpnode->ndim; i++){
                   bzero(buff, sizeof(buff));
                   if( strncmp(Tmpnode->type,"UC",2) == 0 || strncmp(Tmpnode->type,"SC",2) == 0 || 
                            Tmpnode->type[0] == 'C' || strncmp(Tmpnode->type,"DISKFILE",8) == 0){
                                if( snprintf(buff, MAX_WORD_LENGTH,"%ld ",Tmpnode->fdim[i]-1) < 0)
                                    Perror("snprintf");
                   }
                   else
                   {
                            if( snprintf(buff, MAX_WORD_LENGTH,"%ld ",Tmpnode->fdim[i]) < 0)
							    Perror("snprintf");
                   }
					if (fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp ) < strlen(buff))
						Perror("fwrite");
						tot_dim = tot_dim * Tmpnode->fdim[i];
				}
					if( snprintf(buff, MAX_WORD_LENGTH,"\n") < 0)
								Perror("snprintf");
				if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
/*
 * call to function printing actual data in file
 */						
			m3l_write_file_data_filedescprt(Tmpnode, tot_dim, fp);
			}

			if( snprintf(buff, MAX_WORD_LENGTH,"\n") < 0)
    						        Perror("snprintf");
			if (fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
				Perror("fwrite");
		}
		Tmpnode = Tmpnode->next;
	}
	else
	{
/*
 * list has children
 */
		Tmplist = List;
// 		if( strncmp(List->type, "LINK", 4 ) == 0)
// 			Tmplist = Tmplist ->child;  /* list is populated by the target list where it points to */

		bzero(buff, sizeof(buff));
		if( snprintf(buff, MAX_WORD_LENGTH,"%s %s %ld\n",Tmplist->name, Tmplist->type, Tmplist->ndim) < 0)
			Perror("snprintf");
		if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
			Perror("fwrite");

		Tmpnode = Tmplist->child;
		while(Tmpnode != NULL){

			if( strncmp(Tmpnode->type, "LINK", 4 ) == 0){
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
					
					if(m3l_WriteData(2,Tmplist,fp) != 0){
						Warning("Write data problem");
/*
 * restore original state
 */
						Tmplist->next = Tmpnext;
						Tmplist->prev = Tmpprev;
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
					if(m3l_WriteData(2,Tmpnode,fp) != 0){
						Warning("Write data problem");
						return -1;
					}
				}
			}
			else{
				if(m3l_WriteData(2,Tmpnode,fp) != 0){
					Warning("Write data problem");
					return -1;
				}
			}
			Tmpnode = Tmpnode->next;
		}
	}
/*
 * write the EOMF instruction if ending initial call (ie call == 1)
 */
	if(call == 1){
		bzero(buff, sizeof(buff));
		if( (n=snprintf(buff, MAX_WORD_LENGTH,"%s",EOFfile)) < 0)
			Perror("snprintf");
		buff[n] = '\0';
/*
 * set the last element of the buff to \0 and add buff to buffer
 */
		buff[n] = '\0';
		if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
			Perror("fwrite");
	}
	return 0;
}






lmint_t m3l_write_file_data_filedescprt(node_t *Tmpnode, lmsize_t tot_dim, FILE *fp)
{	
	lmsize_t i, n, counter;
	lmchar_t *pc;
	lmchar_t buff[MAX_WORD_LENGTH+1];
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
				if( (n= FCS_W_LD(Tmpnode->data.ldf[i], SEPAR_SPACE)) < 0){
	      			       	Perror("snprintf");
					return -1;
				}
/*
 * set the last element of the buff to \0 and add buff to buffer
 */
				buff[n] = '\0';
				if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
			} 
		}
		else if(strncmp(Tmpnode->type,"D",1) == 0){  /* double */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_D(Tmpnode->data.df[i], SEPAR_SPACE)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
			} 
		}
		else if(strncmp(Tmpnode->type,"F",1) == 0){  /* float */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_F(Tmpnode->data.f[i], SEPAR_SPACE)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
			} 
		}
/*
 * chars
 */
		else if(strncmp(Tmpnode->type,"UC",2) == 0){  /* char */
			bzero(buff, sizeof(buff));
			pc = (lmchar_t *)&Tmpnode->data.uc[0];
/*
 * start with writing TEXT_SEPAR_SIGN
 */
			counter = 0;
			buff[counter] = TEXT_SEPAR_SIGN;
			counter++;
			
			while(*pc != '\0'){   /*can be replaced by for (i=0; i<tot_dim; i++) loop */
				buff[counter] = *pc++;
				counter++;
				if(counter == MAXLINE){
					buff[counter] = '\0';
					if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
						Perror("fwrite");
					counter = 0;
					bzero(buff, sizeof(buff));
				}	
				
			}
/*
 * add terminating TEXT_SEPAR_SIGN
 */
			if(counter != 0 && counter < MAXLINE-1){
/*
 * if enough space, add both TEXT_SEPAR_SIGN and '\0'
 */
				buff[counter] = TEXT_SEPAR_SIGN;
				counter++;
				buff[counter] = '\0';
				if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
				bzero(buff, sizeof(buff));
			}
			else if(counter == MAXLINE){
/*
 * if not enough space, add '0\' signe
 * write to buffer and set counter to 0 
 */
				buff[counter] = '\0';
				if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
				bzero(buff, sizeof(buff));
				counter = 0;
			}
/*
 * if counter == 0, add TEXT_SEPAR_SIGN and '\0'
 */			
			if(counter == 0){
				buff[counter] = TEXT_SEPAR_SIGN;
				counter++;
				buff[counter] = '\0';
				if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
				bzero(buff, sizeof(buff));
			}
		}
		else if (strncmp(Tmpnode->type,"SC",2) == 0){  /* signed char */
/*
 * clean buff and make pointer pointing at its beginning
 */
			bzero(buff, sizeof(buff));
			pc = (lmchar_t *)&Tmpnode->data.sc[0];
/*
 * start with writing TEXT_SEPAR_SIGN
 */
			counter = 0;
			buff[counter] = TEXT_SEPAR_SIGN;
			counter++;
			
			while(*pc != '\0'){   /*can be replaced by for (i=0; i<tot_dim; i++) loop */
				buff[counter] = *pc++;
				counter++;
				if(counter == MAXLINE){
					buff[counter] = '\0';
					if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
						Perror("fwrite");
					counter = 0;
					bzero(buff, sizeof(buff));
				}	
				
			}
/*
 * add terminating TEXT_SEPAR_SIGN
 */
			if(counter != 0 && counter < MAXLINE-1){
/*
 * if enough space, add both TEXT_SEPAR_SIGN and '\0'
 */
				buff[counter] = TEXT_SEPAR_SIGN;
				counter++;
				buff[counter] = '\0';
				if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
				bzero(buff, sizeof(buff));
			}
			else if(counter == MAXLINE){
/*
 * if not enough space, add '0\' signe
 * write to buffer and set counter to 0 
 */
				buff[counter] = '\0';
				if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
				bzero(buff, sizeof(buff));
				counter = 0;
			}
/*
 * if counter == 0, add TEXT_SEPAR_SIGN and '\0'
 */			
			if(counter == 0){
				buff[counter] = TEXT_SEPAR_SIGN;
				counter++;
				buff[counter] = '\0';
				if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
				bzero(buff, sizeof(buff));
			}
		}
		else if(strncmp(Tmpnode->type,"C",1) == 0){  /* char */
			bzero(buff, sizeof(buff));
			pc = &Tmpnode->data.c[0];
/*
 * start with writing TEXT_SEPAR_SIGN
 */
			counter = 0;
			buff[counter] = TEXT_SEPAR_SIGN;
			counter++;
			
			while(*pc != '\0'){   /*can be replaced by for (i=0; i<tot_dim; i++) loop */
				buff[counter] = *pc++;
				counter++;
				if(counter == MAXLINE){
					buff[counter] = '\0';
					if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
						Perror("fwrite");
					counter = 0;
					bzero(buff, sizeof(buff));
				}	
				
			}
/*
 * add terminating TEXT_SEPAR_SIGN
 */
			if(counter != 0 && counter < MAXLINE-1){
/*
 * if enough space, add both TEXT_SEPAR_SIGN and '\0'
 */
				buff[counter] = TEXT_SEPAR_SIGN;
				counter++;
				buff[counter] = '\0';
				if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
				bzero(buff, sizeof(buff));
			}
			else if(counter == MAXLINE){
/*
 * if not enough space, add '0\' signe
 * write to buffer and set counter to 0 
 */
				buff[counter] = '\0';
				if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
				bzero(buff, sizeof(buff));
				counter = 0;
			}
/*
 * if counter == 0, add TEXT_SEPAR_SIGN and '\0'
 */			
			if(counter == 0){
				buff[counter] = TEXT_SEPAR_SIGN;
				counter++;
				buff[counter] = '\0';
				if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
				bzero(buff, sizeof(buff));
			}
		}
		
		
/*
 * integers
 */
		else if(strncmp(Tmpnode->type,"ULLI",4) == 0){  /* unsigned long long  int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_ULLI(Tmpnode->data.ulli[i], SEPAR_SPACE)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
			} 
		}
		else if(strncmp(Tmpnode->type,"SLLI",4) == 0){  /* signed long long  int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_SLLI(Tmpnode->data.slli[i], SEPAR_SPACE)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
			} 
		}
		else if(strncmp(Tmpnode->type,"LLI",3) == 0){  /* signed long long  int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_LLI(Tmpnode->data.lli[i], SEPAR_SPACE)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
			} 
		}
		else if(strncmp(Tmpnode->type,"ULI",3) == 0){  /* unsigned long int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_ULI(Tmpnode->data.uli[i], SEPAR_SPACE)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
			} 
		}
		else if(strncmp(Tmpnode->type,"USI",3) == 0){  /* unsigned short int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_USI(Tmpnode->data.usi[i], SEPAR_SPACE)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
			} 
		}
		else if(strncmp(Tmpnode->type,"SI",2) == 0){  /* short int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_SI(Tmpnode->data.si[i], SEPAR_SPACE)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
			} 
		}
		else if(strncmp(Tmpnode->type,"UI",2) == 0){  /* unsigned int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_UI(Tmpnode->data.ui[i], SEPAR_SPACE)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
			} 
		}
		else if(strncmp(Tmpnode->type,"LI",2) == 0){  /* long  int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_LI(Tmpnode->data.li[i], SEPAR_SPACE)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
			} 
		}
		else if(strncmp(Tmpnode->type,"I",1) == 0){  /* int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_I(Tmpnode->data.i[i], SEPAR_SPACE)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
			} 
		}
/*
 * counters
 */
		else if(strncmp(Tmpnode->type,"ST",2) == 0){  /* size_t */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_ST(Tmpnode->data.st[i], SEPAR_SPACE)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
			} 
		}
		else if(strncmp(Tmpnode->type,"PTRDF",1) == 0){  /* ptrdf_t */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_PTRDF(Tmpnode->data.ptrdf[i], SEPAR_SPACE)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if ( fwrite (buff ,sizeof(lmchar_t),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
			} 
		}
		else
		{
			Error("Unknown type");
		}
		
	return 1;
}

/*
 *  function writes bindary files
 */



