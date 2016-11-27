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
 *     Function WriteBData.c
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
#include "WriteBData.h"
#include "IO_types.h"


#define SEPAR_SPACE ' '

static lmint_t m3l_write_file_Bdata_filedescprt(node_t *, lmsize_t , FILE *);

/*
 * routine writes linked list structure to the file
 */
lmint_t m3l_WriteBData(node_t *List,  FILE *fp)
{
	node_t *Tmpnode, *Tmplist, *Tmpnext, *Tmpprev;
	lmsize_t i, n, tot_dim;
    
    IOstr_t IOstruct, *pIOstruct;
 
	if(List == NULL){
		Warning("WriteData: NULL list");
		return -1;
	}

	pIOstruct = &IOstruct;
    
	if(List->child == NULL ){
/*
 * loop over next nodes
 */
	Tmpnode = List;

		if(strncmp(Tmpnode->type, "DIR", 3) == 0){
            pIOstruct->Type  = Tmpnode->type;
            pIOstruct->Name = Tmpnode->name;
            pIOstruct->ndim = Tmpnode->ndim;

            if ( fwrite (pIOstruct ,IOLEN,  1 , fp ) < 1)
				Perror("fwrite");
		}
		else
		{
/*
 * write only FILE data, if DIR is empty, it will be written here too
 */
            pIOstruct->Type  = Tmpnode->type;
            pIOstruct->Name = Tmpnode->name;
            pIOstruct->ndim = Tmpnode->ndim;

            if ( fwrite (pIOstruct ,IOLEN,  1 , fp ) < 1)
				Perror("fwrite");

			if(Tmpnode->ndim > 0){
                tot_dim = 1;
				for(i=0; i<Tmpnode->ndim; i++){
						tot_dim = tot_dim * Tmpnode->fdim[i];
				}
/*
 * when writing character, dncrese dimensions, an extra dimensions was allocated for /0 trminating character
 */
				if (fwrite (Tmpnode->fdim ,sizeof(lmsize_t), Tmpnode->ndim , fp ) < Tmpnode->ndim)
                    Perror("fwrite");
/*
 * call to function printing actual data in file
 */						
                 m3l_write_file_Bdata_filedescprt(Tmpnode, tot_dim, fp);
			}
		}
		Tmpnode = Tmpnode->next;
	}
	else
	{
/*
 * list has children
 */
		Tmplist = List;

        pIOstruct->Type  = Tmplist->type;
        pIOstruct->Name = Tmplist->name;
        pIOstruct->ndim = Tmplist->ndim;

        if ( fwrite (pIOstruct ,IOLEN,  1 , fp ) < 1)
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
					
					if(m3l_WriteBData(Tmplist,fp) != 0){
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
					if(m3l_WriteBData(Tmpnode,fp) != 0){
						Warning("Write data problem");
						return -1;
					}
				}
			}
			else{
				if(m3l_WriteBData(Tmpnode,fp) != 0){
					Warning("Write data problem");
					return -1;
				}
			}
			Tmpnode = Tmpnode->next;
		}
	}
	return 0;
}






lmint_t m3l_write_file_Bdata_filedescprt(node_t *Tmpnode, lmsize_t tot_dim, FILE *fp)
{	
	lmsize_t i, n, counter;
	lmchar_t *pc;
	lmchar_t buff[MAX_WORD_LENGTH+1];
/*
 * find type of the argument - Floats
 */
		if (strncmp(Tmpnode->type,"LD",2) == 0){  /* long double */
            if ( fwrite (buff ,sizeof(lmlongdouble_t),  tot_dim, fp )< tot_dim);
		}
		else if(strncmp(Tmpnode->type,"D",1) == 0){  /* double */
            if ( fwrite (buff ,sizeof(lmdouble_t),  tot_dim, fp )< tot_dim);
		}
		else if(strncmp(Tmpnode->type,"F",1) == 0){  /* float */
            if ( fwrite (buff ,sizeof(lmfloat_t),  tot_dim, fp )< tot_dim);
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
            if ( fwrite (buff ,sizeof(lmullint_t),  tot_dim, fp )< tot_dim);
		}
		else if(strncmp(Tmpnode->type,"SLLI",4) == 0){  /* signed long long  int */
            if ( fwrite (buff ,sizeof(lmsllint_t),  tot_dim, fp )< tot_dim);
		}
		else if(strncmp(Tmpnode->type,"LLI",3) == 0){  /* signed long long  int */
            if ( fwrite (buff ,sizeof(lmllint_t),  tot_dim, fp )< tot_dim);
		}
		else if(strncmp(Tmpnode->type,"ULI",3) == 0){  /* unsigned long int */
            if ( fwrite (buff ,sizeof(lmulint_t),  tot_dim, fp )< tot_dim);
		}
		else if(strncmp(Tmpnode->type,"USI",3) == 0){  /* unsigned short int */
            if ( fwrite (buff ,sizeof(lmushint_t),  tot_dim, fp )< tot_dim);
		}
		else if(strncmp(Tmpnode->type,"SI",2) == 0){  /* short int */
            if ( fwrite (buff ,sizeof(lmshint_t),  tot_dim, fp )< tot_dim);
		}
		else if(strncmp(Tmpnode->type,"UI",2) == 0){  /* unsigned int */
            if ( fwrite (buff ,sizeof(lmuint_t),  tot_dim, fp )< tot_dim);
		}
		else if(strncmp(Tmpnode->type,"LI",2) == 0){  /* long  int */
            if ( fwrite (buff ,sizeof(lmlint_t),  tot_dim, fp )< tot_dim);
		}
		else if(strncmp(Tmpnode->type,"I",1) == 0){  /* int */
            if ( fwrite (buff ,sizeof(lmint_t),  tot_dim, fp )< tot_dim);
		}
/*
 * counters
 */
		else if(strncmp(Tmpnode->type,"ST",2) == 0){  /* size_t */
            if ( fwrite (buff ,sizeof(lmsize_t),  tot_dim, fp )< tot_dim);
		}
		else if(strncmp(Tmpnode->type,"PTRDF",1) == 0){  /* ptrdf_t */
            if ( fwrite (buff ,sizeof(lmintptr_t),  tot_dim, fp )< tot_dim);
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



