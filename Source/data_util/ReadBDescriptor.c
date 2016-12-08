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
 *     Function ReadBDescriptor.c
 *
 *     Date: 2012-06-30
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
#include "internal_format_type.h"
#include "format_conversion_spec.h"

#include "FunctionsPrt.h"
#include "udf_rm.h"

#include "IO_types.h"


#define EXPR       (**pc != ' ' && **pc != '\t' && **pc != '\n' && **pc != '\0')
#define IFEXPR     ((**pc == ' ' || **pc == '\t' || **pc == '\n') && **pc != '\0')
#define LASTEXPR   (lastchar != ' ' && lastchar != '\t' && lastchar != '\n' && lastchar != '\0')

static node_t *m3l_read_Blist(tmpstruct_t , FILE *f, lmsize_t *, opts_t *);
static node_t *m3l_read_file_Bthreadsafe(FILE *, lmsize_t *, opts_t *);
static node_t *m3l_read_Bfile_data(FILE*, lmsize_t *, opts_t *);
static node_t *m3l_read_file_Bfll(FILE *, lmsize_t *, opts_t *);
static node_t *m3l_read_Bflllist(tmpstruct_t , FILE *f, lmsize_t *, opts_t *);
static lmchar_t *type_convert(lmchar_t *);

/*
 * Function read just one line from a socket, disregarding comments line
 * It identifies if the line is a header of DATA or DIR list
 * If DATA type of list it allocates a node_t and calls ReadDataLine which reads the
 * actual conent of the list
 * If the list if of DIR type, it calls ReadDir routine which reads DIR types of list (recursive calling)
 */ 

node_t *m3l_read_Bfile(FILE *fp, opts_t *Popts){
	
	lmssize_t pos, *ppos;;
	node_t *Lnode;
	
    ppos = &pos;
    pos = 0;
    
    if(Popts->opt_xformat == 'f'){
        Lnode = m3l_read_file_Bfll(fp, ppos, Popts);
    }else{
        Lnode = m3l_read_file_Bthreadsafe(fp, ppos, Popts);        
    }

	return Lnode;
	
}


node_t *m3l_read_file_Bthreadsafe(FILE *fp, lmsize_t *pos, opts_t *Popts)
{
    IOstr_t IOstruct, *pIOstruct;
    lmsize_t ngotten;
    tmpstruct_t TMPSTR;	
    node_t *Dnode;

	pIOstruct = &IOstruct;

    fseek( fp, *pos, SEEK_SET );
	
	if(   (ngotten = fread(pIOstruct ,IOLEN,  1 , fp ))   < 1){
		Perror("fread");
	}

    if( snprintf(TMPSTR.Type, sizeof(TMPSTR.Type),"%s",  pIOstruct->Type) < 0)
        Perror("snprintf");
    if( snprintf(TMPSTR.Name_Of_List, sizeof(TMPSTR.Name_Of_List),"%s",  pIOstruct->Name) < 0)
        Perror("snprintf");
     
    TMPSTR.ndim = pIOstruct->ndim;
    TMPSTR.dim=NULL;
     
    if( (Dnode = m3l_read_Blist(TMPSTR, fp, pos, Popts)) == NULL)
        Perror("ReadDirData - ReadDir");
     
    return Dnode;
}



/*
 * reads data after line identifying DIR
 */

node_t *m3l_read_Blist(tmpstruct_t TMPSTR, FILE *fp, lmsize_t *pos, opts_t *Popts)
{
	lmsize_t i;
	node_t *Dnode, *Tmpnode, *Pnode;
 /*
 * allocate node which is DIR type
 */	
	if( (Dnode = m3l_AllocateNode(TMPSTR, Popts)) == NULL){
		Error("Allocate");
	}

	for(i=1;i<=TMPSTR.ndim; i++){

        Tmpnode=NULL;	

  		if ( (Tmpnode = m3l_read_Bfile_data(fp, pos, Popts)) == NULL)
			Error("ReadDirData: ReadData");
/*
 * add to node
 */
		if(i ==1){
			Pnode         = Tmpnode;
			Dnode->child  = Pnode;
			Pnode->parent = Dnode;
		}
		else
		{
			Pnode->next      = Tmpnode;
			Tmpnode->prev    = Pnode;
			Tmpnode->parent  = Dnode;
			 Pnode            = Tmpnode;
		}
	}
	return Dnode;
}

node_t *m3l_read_Bfile_data(FILE* fp, lmsize_t * pos, opts_t *Popts){
    
	node_t *Dnode;    
    tmpstruct_t TMPSTR;
    lmsize_t ngotten;
    IOstr_t IOstruct, *pIOstruct;
    lmsize_t i,tot_dim;

	pIOstruct = &IOstruct;
	
    if(   (ngotten = fread(pIOstruct ,IOLEN,  1 , fp ))   < 1){
		Perror("fread");
	}

    *pos = *pos + IOLEN;
         
    if( snprintf(TMPSTR.Type, sizeof(TMPSTR.Type),"%s",  pIOstruct->Type) < 0)
        Perror("snprintf");
    if( snprintf(TMPSTR.Name_Of_List, sizeof(TMPSTR.Name_Of_List),"%s",  pIOstruct->Name) < 0)
        Perror("snprintf");
     
    TMPSTR.ndim = pIOstruct->ndim;
    TMPSTR.dim=NULL;
    
    if ( strncmp(TMPSTR.Type,"DIR",3) != 0 &&  strncmp(TMPSTR.Type,"LINK",4) != 0 ){
/*
 * if type is FILE, read what it contains
 */
        if( (TMPSTR.dim=(lmsize_t *)malloc(TMPSTR.ndim * sizeof(lmsize_t))) == NULL)
			Perror("malloc");
/*
 * read dimensions
 */
        if(   (ngotten = fread(TMPSTR.dim ,sizeof(lmsize_t),  TMPSTR.ndim , fp ))   < TMPSTR.ndim ){
            Perror("fread");
        }
        *pos = *pos + sizeof(lmsize_t) *  TMPSTR.ndim;
        
        tot_dim = 1;
        for(i=0;i< TMPSTR.ndim; i++)
            tot_dim = tot_dim*TMPSTR.dim[i];
/*
 * allocate node
 */
        if( (Dnode = m3l_AllocateNode(TMPSTR, Popts)) == NULL){
            Error("Allocate");
        }

        if (strncmp(TMPSTR.Type,"LD",2) == 0){  /* long double */
            if(   (ngotten = fread(Dnode->data.ldf ,sizeof(lmlongdouble_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
            *pos = *pos + sizeof(lmlongdouble_t) *  tot_dim;
        }
        else if(strncmp(TMPSTR.Type,"D",1) == 0){  /* double */
            if(   (ngotten = fread(Dnode->data.df ,sizeof(lmdouble_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
             *pos = *pos + sizeof(lmdouble_t) *  tot_dim;
       }
        else if(strncmp(TMPSTR.Type,"F",1) == 0){  /* float */
            if(   (ngotten = fread(Dnode->data.f ,sizeof(lmfloat_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
            *pos = *pos + sizeof(lmfloat_t) *  tot_dim;
        }
/*
 * integers
 */
        else if(strncmp(TMPSTR.Type,"ULLI",4) == 0){  /* unsigned long long  int */
            if(   (ngotten = fread(Dnode->data.ulli ,sizeof(lmullint_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
            *pos = *pos + sizeof(lmullint_t) *  tot_dim;
        }
        else if(strncmp(TMPSTR.Type,"SLLI",4) == 0){  /* signed long long  int */
            if(   (ngotten = fread(Dnode->data.slli ,sizeof(lmsllint_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
            *pos = *pos + sizeof(lmsllint_t) *  tot_dim;
        }
        else if(strncmp(TMPSTR.Type,"ULI",3) == 0){  /* unsigned long int */
            if(   (ngotten = fread(Dnode->data.uli,sizeof(lmulint_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
            *pos = *pos + sizeof(lmsllint_t) *  tot_dim;
        }
        else if(strncmp(TMPSTR.Type,"LLI",3) == 0){  /* unsigned long int */
            if(   (ngotten = fread(Dnode->data.lli ,sizeof(lmllint_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
            *pos = *pos + sizeof(lmllint_t) *  tot_dim;
        }
        else if(strncmp(TMPSTR.Type,"USI",3) == 0){  /* unsigned short int */
            if(   (ngotten = fread(Dnode->data.usi ,sizeof(lmushint_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
            *pos = *pos + sizeof(lmushint_t) *  tot_dim;
        }
        else if(strncmp(TMPSTR.Type,"SI",2) == 0){  /* short int */
            if(   (ngotten = fread(Dnode->data.si ,sizeof(lmshint_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
            *pos = *pos + sizeof(lmshint_t) *  tot_dim;
        }
        else if(strncmp(TMPSTR.Type,"UI",2) == 0){  /* unsigned int */
            if(   (ngotten = fread(Dnode->data.ui ,sizeof(lmuint_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
            *pos = *pos + sizeof(lmuint_t) *  tot_dim;
        }
        else if(strncmp(TMPSTR.Type,"LI",2) == 0){  /* long  int */
            if(   (ngotten = fread(Dnode->data.li ,sizeof(lmlint_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
            *pos = *pos + sizeof(lmlint_t) *  tot_dim;
        }
        else if(strncmp(TMPSTR.Type,"I",1) == 0){  /* int */
            if(   (ngotten = fread(Dnode->data.i ,sizeof(lmint_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
            *pos = *pos + sizeof(lmint_t) *  tot_dim;
        }
/*
 * chars and strings
 */
        else if ( strncmp(TMPSTR.Type,"SC",2) == 0 ){
            if(   (ngotten = fread(Dnode->data.sc ,sizeof(lmsignchar_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
            Dnode->data.c[tot_dim] = '\0'; 
            *pos = *pos + sizeof(lmsignchar_t) *  tot_dim;            
        }
        else if ( strncmp(TMPSTR.Type,"UC",2) == 0 ){
            if(   (ngotten = fread(Dnode->data.uc ,sizeof(lmusignchar_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
            Dnode->data.c[tot_dim] = '\0'; 
            *pos = *pos + sizeof(lmusignchar_t) *  tot_dim;             
        }
        else if ( TMPSTR.Type[0] == 'C'){
            if(   (ngotten = fread(Dnode->data.c ,sizeof(lmchar_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
            Dnode->data.c[tot_dim] = '\0';            
            *pos = *pos + sizeof(lmchar_t) *  tot_dim;
        }
/*
 * counters
 */
        else if(strncmp(TMPSTR.Type,"ST",2) == 0){  /* lmsize_t */
//             pst = (*Lnode)->data.st;
        }
        else if(strncmp(TMPSTR.Type,"PTRDF",1) == 0){  /* ptrdf_t */
//             pptrdf = (*Lnode)->data.ptrdf;
        }
        
        
        
    }else{
/*
 * if type is DIR, read it
 */
        if( (Dnode = m3l_read_Blist(TMPSTR, fp, pos, Popts)) == NULL)
            Perror("ReadDirData - ReadDir");
        return Dnode;
    }
    return Dnode;
    
    
}



node_t *m3l_read_file_Bfll(FILE *fp, lmsize_t *pos, opts_t *Popts)
{
    lmsize_t ngotten;
    tmpstruct_t TMPSTR;	
    node_t *Dnode;
    lmchar_t Name[17], Type[5];
    lmlint_t ndim, nsize;
    
    fseek( fp, *pos, SEEK_SET );
    
    if(   (ngotten = fread(Name ,16,  1 , fp ))   < 1){
		Perror("fread");
	}
	Name[16] = '\0';
	
    if(   (ngotten = fread(Type ,4,  1 , fp ))   < 1){
		Perror("fread");
	}
	Type[4] = '\0';
	
    if(   (ngotten = fread(&ndim ,8,  1 , fp ))   < 1){
		Perror("fread");
	}
	
    if(   (ngotten = fread(&nsize ,8,  1 , fp ))   < 1){
		Perror("fread");
	}
    
    *pos = *pos + 36;
   
	
    if( snprintf(TMPSTR.Type, 4,"%s",  Type) < 0)
        Perror("snprintf");
    if( snprintf(TMPSTR.Name_Of_List, 16,"%s",  Name) < 0)
        Perror("snprintf");
         
//     if(ndim > 1 && nsize > 1)
       TMPSTR.ndim = ndim;
//     else
//         TMPSTR.ndim = 1;

    TMPSTR.dim=NULL;
     
    if( (Dnode = m3l_read_Bflllist(TMPSTR, fp, pos, Popts)) == NULL)
        Perror("ReadDirData - ReadDir");
     
    return Dnode;
}



node_t *m3l_read_Bfllfile_data(FILE* fp, lmsize_t * pos, opts_t *Popts){
    
	node_t *Dnode;    
    tmpstruct_t TMPSTR;
    lmsize_t ngotten;
    lmsize_t i,tot_dim;
	
    lmchar_t Name[17], Type[5], Ntype[5];
    lmlint_t ndim, nsize;
        
    if(   (ngotten = fread(Name ,16,  1 , fp ))   < 1){
		Perror("fread");
	}
	Name[16] = '\0';
	
    if(   (ngotten = fread(Type ,4,  1 , fp ))   < 1){
		Perror("fread");
	}
	Type[4] = '\0';
	
    if(   (ngotten = fread(&ndim ,8,  1 , fp ))   < 1){
		Perror("fread");
	}
	
    if(   (ngotten = fread(&nsize ,8,  1 , fp ))   < 1){
		Perror("fread");
	}
/*
 * convert type from fll format to libm3l
 */	
    *pos = *pos + 36;
         
    if( snprintf(TMPSTR.Type, sizeof(TMPSTR.Type),"%s",  type_convert(Type)) < 0)
        Perror("snprintf");
    if( snprintf(TMPSTR.Name_Of_List, sizeof(TMPSTR.Name_Of_List),"%s",  Name) < 0)
        Perror("snprintf");
     
    if(ndim > 1 && nsize > 1)
       TMPSTR.ndim = 2;
    else
        TMPSTR.ndim = 1;

    tot_dim = ndim * nsize;

    TMPSTR.dim=NULL;
    
    if ( strncmp(TMPSTR.Type,"DIR",3) != 0 &&  strncmp(TMPSTR.Type,"LINK",4) != 0 ){
/*
 * if type is FILE, read what it contains
 */
        if( (TMPSTR.dim=(lmsize_t *)malloc(TMPSTR.ndim * sizeof(lmsize_t))) == NULL)
			Perror("malloc");
/*
 * save dimensions
 */
        if(ndim > 1 && nsize > 1){
            TMPSTR.dim[0] = ndim;
            TMPSTR.dim[1] = nsize;}
        else
            TMPSTR.dim[0] = ndim * nsize ;
/*
 * allocate node
 */
        if( (Dnode = m3l_AllocateNode(TMPSTR, Popts)) == NULL){
            Error("Allocate");
        }

        if (strncmp(TMPSTR.Type,"LD",2) == 0){  /* long double */
            if(   (ngotten = fread(Dnode->data.ldf ,sizeof(lmlongdouble_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
            *pos = *pos + sizeof(lmlongdouble_t) *  tot_dim;
        }
        else if(strncmp(TMPSTR.Type,"D",1) == 0){  /* double */
            if(   (ngotten = fread(Dnode->data.df ,sizeof(lmdouble_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
             *pos = *pos + sizeof(lmdouble_t) *  tot_dim;
       }
        else if(strncmp(TMPSTR.Type,"F",1) == 0){  /* float */
            if(   (ngotten = fread(Dnode->data.f ,sizeof(lmfloat_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
            *pos = *pos + sizeof(lmfloat_t) *  tot_dim;
        }
/*
 * integers
 */
        else if(strncmp(TMPSTR.Type,"ULLI",4) == 0){  /* unsigned long long  int */
            if(   (ngotten = fread(Dnode->data.ulli ,sizeof(lmullint_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
            *pos = *pos + sizeof(lmullint_t) *  tot_dim;
        }
        else if(strncmp(TMPSTR.Type,"SLLI",4) == 0){  /* signed long long  int */
            if(   (ngotten = fread(Dnode->data.slli ,sizeof(lmsllint_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
            *pos = *pos + sizeof(lmsllint_t) *  tot_dim;
        }
        else if(strncmp(TMPSTR.Type,"ULI",3) == 0){  /* unsigned long int */
            if(   (ngotten = fread(Dnode->data.uli,sizeof(lmulint_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
            *pos = *pos + sizeof(lmsllint_t) *  tot_dim;
        }
        else if(strncmp(TMPSTR.Type,"LLI",3) == 0){  /* unsigned long int */
            if(   (ngotten = fread(Dnode->data.lli ,sizeof(lmllint_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
            *pos = *pos + sizeof(lmllint_t) *  tot_dim;
        }
        else if(strncmp(TMPSTR.Type,"USI",3) == 0){  /* unsigned short int */
            if(   (ngotten = fread(Dnode->data.usi ,sizeof(lmushint_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
            *pos = *pos + sizeof(lmushint_t) *  tot_dim;
        }
        else if(strncmp(TMPSTR.Type,"SI",2) == 0){  /* short int */
            if(   (ngotten = fread(Dnode->data.si ,sizeof(lmshint_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
            *pos = *pos + sizeof(lmshint_t) *  tot_dim;
        }
        else if(strncmp(TMPSTR.Type,"UI",2) == 0){  /* unsigned int */
            if(   (ngotten = fread(Dnode->data.ui ,sizeof(lmuint_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
            *pos = *pos + sizeof(lmuint_t) *  tot_dim;
        }
        else if(strncmp(TMPSTR.Type,"LI",2) == 0){  /* long  int */
            if(   (ngotten = fread(Dnode->data.li ,sizeof(lmlint_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
            *pos = *pos + sizeof(lmlint_t) *  tot_dim;
        }
        else if(strncmp(TMPSTR.Type,"I",1) == 0){  /* int */
            if(   (ngotten = fread(Dnode->data.i ,sizeof(lmint_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
            *pos = *pos + sizeof(lmint_t) *  tot_dim;
        }
/*
 * chars and strings
 */
        else if ( strncmp(TMPSTR.Type,"SC",2) == 0 ){
            if(   (ngotten = fread(Dnode->data.sc ,sizeof(lmsignchar_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
            Dnode->data.c[tot_dim] = '\0'; 
            *pos = *pos + sizeof(lmsignchar_t) *  tot_dim;            
        }
        else if ( strncmp(TMPSTR.Type,"UC",2) == 0 ){
            if(   (ngotten = fread(Dnode->data.uc ,sizeof(lmusignchar_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
            Dnode->data.c[tot_dim] = '\0'; 
            *pos = *pos + sizeof(lmusignchar_t) *  tot_dim;             
        }
        else if ( TMPSTR.Type[0] == 'C'){
            if(   (ngotten = fread(Dnode->data.c ,sizeof(lmchar_t),  tot_dim , fp ))   < tot_dim ){
                Perror("fread");
            }
            Dnode->data.c[tot_dim] = '\0';            
            *pos = *pos + sizeof(lmchar_t) *  tot_dim;
        }
/*
 * counters
 */
        else if(strncmp(TMPSTR.Type,"ST",2) == 0){  /* lmsize_t */
//             pst = (*Lnode)->data.st;
        }
        else if(strncmp(TMPSTR.Type,"PTRDF",1) == 0){  /* ptrdf_t */
//             pptrdf = (*Lnode)->data.ptrdf;
        }
        
        
        
    }else{
/*
 * if type is DIR, read it
 */
        if( (Dnode = m3l_read_Blist(TMPSTR, fp, pos, Popts)) == NULL)
            Perror("ReadDirData - ReadDir");
        return Dnode;
    }
					
					
    return Dnode;
    
    
}


node_t *m3l_read_Bflllist(tmpstruct_t TMPSTR, FILE *fp, lmsize_t *pos, opts_t *Popts)
{
	lmsize_t i;
	node_t *Dnode, *Tmpnode, *Pnode;
 /*
 * allocate node which is DIR type
 */	
	if( (Dnode = m3l_AllocateNode(TMPSTR, Popts)) == NULL){
		Error("Allocate");
	}
	
	for(i=1;i<=TMPSTR.ndim; i++){

        Tmpnode=NULL;	

  		if ( (Tmpnode = m3l_read_Bfllfile_data(fp, pos, Popts)) == NULL)
			Error("ReadDirData: ReadData");
/*
 * add to node
 */
		if(i ==1){
			Pnode         = Tmpnode;
			Dnode->child  = Pnode;
			Pnode->parent = Dnode;
		}
		else
		{
			Pnode->next      = Tmpnode;
			Tmpnode->prev    = Pnode;
			Tmpnode->parent  = Dnode;
			 Pnode            = Tmpnode;
		}
	}
	return Dnode;
}

 lmchar_t *type_convert(lmchar_t *Type){
     
     lmchar_t *pNtype, Ntype[5];
     pNtype = Ntype;
     
     if(Type[0]  == 'R'){
        Ntype[0] = 'F';
        Ntype[1] = '\0';
     }
     else if(Type[0]  == 'D'){
        Ntype[0] = 'D';
        Ntype[1] = '\0';
     }
     else if(Type[0]  == 'I'){
        Ntype[0] = 'I';
        Ntype[1] = '\0';
     }
     else if(Type[0]  == 'L'){
        Ntype[0] = 'L';
        Ntype[1] = 'I';
        Ntype[2] = '\0';
     }
     else if(Type[0]  == 'S'){
        Ntype[0] = 'C';
        Ntype[1] = '\0';         
     }
     
     return pNtype;
         
         
 }

