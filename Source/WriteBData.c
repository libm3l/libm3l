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
	lmsize_t i, n, tot_dim, *fdim;
    
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
/*
 * when writing character, dncrese dimensions, an extra dimensions was allocated for /0 terminating character
 */
                if( strncmp(Tmpnode->type,"UC",2) == 0 || strncmp(Tmpnode->type,"SC",2) == 0 || 
                            Tmpnode->type[0] == 'C' || strncmp(Tmpnode->type,"DISKFILE",8) == 0){
                    
                    tot_dim = 1;
                    for(i=0; i<Tmpnode->ndim; i++)
						tot_dim = tot_dim * (Tmpnode->fdim[i]-1);
                    
                    if ( (fdim  = (lmsize_t *)malloc(Tmpnode->ndim * sizeof(lmsize_t))) == NULL)
                        Perror("malloc");
                    for(i=0; i<Tmpnode->ndim; i++)
						fdim[i] =  Tmpnode->fdim[i]-1;
                    
                    if (fwrite (fdim ,sizeof(lmsize_t), Tmpnode->ndim , fp ) < Tmpnode->ndim)
                            Perror("fwrite");
                    free(fdim);
                }
                else
                {
                    tot_dim = 1;
                    for(i=0; i<Tmpnode->ndim; i++)
						tot_dim = tot_dim * Tmpnode->fdim[i];
                   if (fwrite (Tmpnode->fdim ,sizeof(lmsize_t), Tmpnode->ndim , fp ) < Tmpnode->ndim)
                     Perror("fwrite");
                }
// 				if (fwrite (Tmpnode->fdim ,sizeof(lmsize_t), Tmpnode->ndim , fp ) < Tmpnode->ndim)
//                     Perror("fwrite");
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
/*
 * find type of the argument - Floats
 */
		if (strncmp(Tmpnode->type,"LD",2) == 0){  /* long double */
            if ( fwrite (Tmpnode->data.ldf ,sizeof(lmlongdouble_t),  tot_dim, fp )< tot_dim)Perror("fwrite");
		}
		else if(strncmp(Tmpnode->type,"D",1) == 0){  /* double */
            if ( fwrite (Tmpnode->data.df ,sizeof(lmdouble_t),  tot_dim, fp )< tot_dim)Perror("fwrite");
		}
		else if(strncmp(Tmpnode->type,"F",1) == 0){  /* float */
            if ( fwrite (Tmpnode->data.f ,sizeof(lmfloat_t),  tot_dim, fp )< tot_dim)Perror("fwrite");
		}
/*
 * chars
 */
		else if(strncmp(Tmpnode->type,"UC",2) == 0){  /* char */
            if ( fwrite (Tmpnode->data.sc ,sizeof(lmusignchar_t),  tot_dim, fp )< tot_dim)Perror("fwrite");
		}
		else if (strncmp(Tmpnode->type,"SC",2) == 0){  /* signed char */
            if ( fwrite (Tmpnode->data.sc ,sizeof(lmsignchar_t),  tot_dim, fp )< tot_dim)Perror("fwrite");
		}
		else if(strncmp(Tmpnode->type,"C",1) == 0){  /* char */
            if ( fwrite (Tmpnode->data.c ,sizeof(lmchar_t),  tot_dim, fp )< tot_dim)Perror("fwrite");
		}
/*
 * integers
 */
		else if(strncmp(Tmpnode->type,"ULLI",4) == 0){  /* unsigned long long  int */
            if ( fwrite (Tmpnode->data.ulli ,sizeof(lmullint_t),  tot_dim, fp )< tot_dim)Perror("fwrite");
		}
		else if(strncmp(Tmpnode->type,"SLLI",4) == 0){  /* signed long long  int */
            if ( fwrite (Tmpnode->data.slli ,sizeof(lmsllint_t),  tot_dim, fp )< tot_dim)Perror("fwrite");
		}
		else if(strncmp(Tmpnode->type,"LLI",3) == 0){  /* signed long long  int */
            if ( fwrite (Tmpnode->data.lli ,sizeof(lmllint_t),  tot_dim, fp )< tot_dim)Perror("fwrite");
		}
		else if(strncmp(Tmpnode->type,"ULI",3) == 0){  /* unsigned long int */
            if ( fwrite (Tmpnode->data.uli ,sizeof(lmulint_t),  tot_dim, fp )< tot_dim)Perror("fwrite");
		}
		else if(strncmp(Tmpnode->type,"USI",3) == 0){  /* unsigned short int */
            if ( fwrite (Tmpnode->data.usi ,sizeof(lmushint_t),  tot_dim, fp )< tot_dim)Perror("fwrite");
		}
		else if(strncmp(Tmpnode->type,"SI",2) == 0){  /* short int */
            if ( fwrite (Tmpnode->data.si ,sizeof(lmshint_t),  tot_dim, fp )< tot_dim)Perror("fwrite");
		}
		else if(strncmp(Tmpnode->type,"UI",2) == 0){  /* unsigned int */
            if ( fwrite (Tmpnode->data.ui ,sizeof(lmuint_t),  tot_dim, fp )< tot_dim)Perror("fwrite");
		}
		else if(strncmp(Tmpnode->type,"LI",2) == 0){  /* long  int */
            if ( fwrite (Tmpnode->data.li ,sizeof(lmlint_t),  tot_dim, fp )< tot_dim)Perror("fwrite");
		}
		else if(strncmp(Tmpnode->type,"I",1) == 0){  /* int */
            if ( fwrite (Tmpnode->data.i ,sizeof(lmint_t),  tot_dim, fp )< tot_dim)Perror("fwrite");
		}
/*
 * counters
 */
		else if(strncmp(Tmpnode->type,"ST",2) == 0){  /* size_t */
            if ( fwrite (Tmpnode->data.st ,sizeof(lmsize_t),  tot_dim, fp )< tot_dim)Perror("fwrite");
		}
		else if(strncmp(Tmpnode->type,"PTRDF",1) == 0){  /* ptrdf_t */
            if ( fwrite (Tmpnode->data.ptrdf ,sizeof(lmintptr_t),  tot_dim, fp )< tot_dim)Perror("fwrite");
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



