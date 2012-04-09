/*
 * function frees memory of the node 
 * copyright ï¿½ 2012 Adam Jirasek
 */
 
#include "Header.h"
#include "format_type.h"
#include "FunctionsPrt.h"
#include "WriteData.h"
#include "format_conversion_spec.h"

#define SEPAR_SPACE ' '

static int write_file_data_filedescprt(node_t *, size_t , FILE *);

/*
 * routine writes linked list structure to the file
 */
int WriteData(node_t *List,  FILE *fp)
{
	node_t *Tmpnode; 
	size_t i, tot_dim;
	char buff[MAX_WORD_LENGTH];
 
	if(List == NULL){
		Warning("WriteData: NULL list");
		return -1;
	} 
 
	if(List->child == NULL){
/*
 * loop over next nodes
 */
	Tmpnode = List;

		if(Tmpnode != NULL){
			while(Tmpnode != NULL){
				if(Tmpnode->child != NULL){
					if(WriteData(Tmpnode, fp) != 0){
						Warning("Write data problem");
						return -1;
					}

					Tmpnode = Tmpnode->next;
				}
				else
				{
					if(strncmp(Tmpnode->type, "DIR", 3) == 0)
					{
						bzero(buff, sizeof(buff));
						if( snprintf(buff, MAX_WORD_LENGTH,"%s %s %ld\n",Tmpnode->name, Tmpnode->type, Tmpnode->ndim) < 0)
	      					        Perror("snprintf");
						if ( fwrite (buff ,sizeof(char),  strlen(buff) , fp ) < strlen(buff))
							Perror("fwrite");
						printf("%s  %s %ld\n", Tmpnode->name, Tmpnode->type, Tmpnode->ndim);	
					}
					else
					{
/*
 * write only FILE data, if DIR is empty, it will be written here too
 */
						bzero(buff, sizeof(buff));
						if( snprintf(buff, MAX_WORD_LENGTH,"%s %s %ld ",Tmpnode->name, Tmpnode->type, Tmpnode->ndim) < 0)
	      					        Perror("snprintf");
						if ( fwrite (buff ,sizeof(char),  strlen(buff) , fp )< strlen(buff))
							Perror("fwrite");
						printf("%s  %s %ld  ", Tmpnode->name, Tmpnode->type, Tmpnode->ndim);


						tot_dim = 1;
						for(i=0; i<Tmpnode->ndim; i++){

							bzero(buff, sizeof(buff));
							if( snprintf(buff, MAX_WORD_LENGTH,"%ld ",Tmpnode->fdim[i]) < 0)
	      						        Perror("snprintf");
							if (fwrite (buff ,sizeof(char),  strlen(buff) , fp ) < strlen(buff))
								Perror("fwrite");

							printf("%ld ", Tmpnode->fdim[i]);
							tot_dim = tot_dim * Tmpnode->fdim[i];
						}

						if( snprintf(buff, MAX_WORD_LENGTH,"\n") < 0)
	      						        Perror("snprintf");
						if ( fwrite (buff ,sizeof(char),  strlen(buff) , fp )< strlen(buff))
							Perror("fwrite");

						printf("\n");
/*
 * NOTE - here add a call to write_data or whatever else it is called
 */						

						write_file_data_filedescprt(Tmpnode, tot_dim, fp);


/*						for (i=0; i<tot_dim; i++){
							bzero(buff, sizeof(buff));
							if( snprintf(buff, MAX_WORD_LENGTH,"%d ", Tmpnode->data.i[i]) < 0)
	      					        	Perror("snprintf");
							if ( fwrite (buff ,sizeof(char),  strlen(buff) , fp )< strlen(buff))
								Perror("fwrite");
							
							printf("%d ", Tmpnode->data.i[i]);
						} */
						if( snprintf(buff, MAX_WORD_LENGTH,"\n") < 0)
	      						        Perror("snprintf");
						if (fwrite (buff ,sizeof(char),  strlen(buff) , fp )< strlen(buff))
							Perror("fwrite");

						printf("\n");
					}

					Tmpnode = Tmpnode->next;
				}
			}
		}
	}
	else
	{

		bzero(buff, sizeof(buff));
		if( snprintf(buff, MAX_WORD_LENGTH,"%s %s %ld\n",List->name, List->type, List->ndim) < 0)
	              Perror("snprintf");
		if ( fwrite (buff ,sizeof(char),  strlen(buff) , fp )< strlen(buff))
			Perror("fwrite");

		printf("%s  %s %ld\n", List->name, List->type, List->ndim);
		Tmpnode = List->child;
		
		if(WriteData(Tmpnode,fp) != 0){
			Warning("Write data problem");
			return -1;
		};
	}

	return 0;
}






int write_file_data_filedescprt(node_t *Tmpnode, size_t tot_dim, FILE *fp)
{	
	size_t i, n, counter;
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
				if( (n= FCS_W_LD(Tmpnode->data.ldf[i], SEPAR_SPACE)) < 0){
	      			       	Perror("snprintf");
					return -1;
				}
/*
 * set the last element of the buff to \0 and add buff to buffer
 */
				buff[n] = '\0';
				if ( fwrite (buff ,sizeof(char),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
			} 
		}
		else if(strncmp(Tmpnode->type,"D",1) == 0){  /* double */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_D(Tmpnode->data.df[i], SEPAR_SPACE)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if ( fwrite (buff ,sizeof(char),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
			} 
		}
		else if(strncmp(Tmpnode->type,"F",1) == 0){  /* float */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_F(Tmpnode->data.f[i], SEPAR_SPACE)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if ( fwrite (buff ,sizeof(char),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
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
			pc = &Tmpnode->data.sc[0];
			counter = 0;
			while(*pc != '\0'){  /*can be replaced by for (i=0; i<tot_dim; i++) loop */
				buff[counter] = *pc++;
				counter++;
				if(counter == MAXLINE){
					buff[counter] = '\0';
					if ( fwrite (buff ,sizeof(char),  strlen(buff) , fp )< strlen(buff))
						Perror("fwrite");
					counter = 0;
					bzero(buff, sizeof(buff));
				}	
				
			}
			if(counter != 0){
				buff[counter] = '\0';
				if ( fwrite (buff ,sizeof(char),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
			}	
		}
		else if(strncmp(Tmpnode->type,"UC",2) == 0){  /* unsigned char */
			bzero(buff, sizeof(buff));
			pc = &Tmpnode->data.uc[0];
			counter = 0;
			while(*pc != '\0'){     /*can be replaced by for (i=0; i<tot_dim; i++) loop */
				buff[counter] = *pc++;
				counter++;
				if(counter == MAXLINE){
					buff[counter] = '\0';
					if ( fwrite (buff ,sizeof(char),  strlen(buff) , fp )< strlen(buff))
						Perror("fwrite");
					counter = 0;
					bzero(buff, sizeof(buff));
				}	
				
			}
			if(counter != 0){
				buff[counter] = '\0';
				if ( fwrite (buff ,sizeof(char),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
			}
		}
		else if(strncmp(Tmpnode->type,"C",1) == 0){  /* char */
			bzero(buff, sizeof(buff));
			pc = &Tmpnode->data.c[0];
			counter = 0;
			while(*pc != '\0'){   /*can be replaced by for (i=0; i<tot_dim; i++) loop */
				buff[counter] = *pc++;
				counter++;
				if(counter == MAXLINE){
					buff[counter] = '\0';
					if ( fwrite (buff ,sizeof(char),  strlen(buff) , fp )< strlen(buff))
						Perror("fwrite");
					counter = 0;
					bzero(buff, sizeof(buff));
				}	
				
			}
			if(counter != 0){
				buff[counter] = '\0';
				if ( fwrite (buff ,sizeof(char),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
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
				if ( fwrite (buff ,sizeof(char),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
			} 
		}
		else if(strncmp(Tmpnode->type,"SLLI",4) == 0){  /* signed long long  int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_SLLI(Tmpnode->data.slli[i], SEPAR_SPACE)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if ( fwrite (buff ,sizeof(char),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
			} 
		}
		else if(strncmp(Tmpnode->type,"ULI",3) == 0){  /* unsigned long int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_ULI(Tmpnode->data.uli[i], SEPAR_SPACE)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if ( fwrite (buff ,sizeof(char),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
			} 
		}
		else if(strncmp(Tmpnode->type,"USI",3) == 0){  /* unsigned short int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_USI(Tmpnode->data.usi[i], SEPAR_SPACE)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if ( fwrite (buff ,sizeof(char),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
			} 
		}
		else if(strncmp(Tmpnode->type,"SI",2) == 0){  /* short int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_SI(Tmpnode->data.si[i], SEPAR_SPACE)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if ( fwrite (buff ,sizeof(char),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
			} 
		}
		else if(strncmp(Tmpnode->type,"UI",2) == 0){  /* unsigned int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_UI(Tmpnode->data.ui[i], SEPAR_SPACE)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if ( fwrite (buff ,sizeof(char),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
			} 
		}
		else if(strncmp(Tmpnode->type,"LI",2) == 0){  /* long  int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_LI(Tmpnode->data.li[i], SEPAR_SPACE)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if ( fwrite (buff ,sizeof(char),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
			} 
		}
		else if(strncmp(Tmpnode->type,"I",1) == 0){  /* int */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_I(Tmpnode->data.i[i], SEPAR_SPACE)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if ( fwrite (buff ,sizeof(char),  strlen(buff) , fp )< strlen(buff))
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
				if ( fwrite (buff ,sizeof(char),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
			} 
		}
		else if(strncmp(Tmpnode->type,"PTRDF",1) == 0){  /* ptrdf_t */
			for (i=0; i<tot_dim; i++){
				bzero(buff, sizeof(buff));
				if( (n=FCS_W_PTRDF(Tmpnode->data.ptrdf[i], SEPAR_SPACE)) < 0)
	      			       	Perror("snprintf");
				buff[n] = '\0';
				if ( fwrite (buff ,sizeof(char),  strlen(buff) , fp )< strlen(buff))
					Perror("fwrite");
			} 
		}
		else
		{
			Error("Unknown type");
		}					
}
