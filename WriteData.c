/*
 * function frees memory of the node 
 * copyright ï¿½ 2012 Adam Jirasek
 */
 
#include "Header.h"
#include "format_type.h"
#include "FunctionsPrt.h"
#include "WriteData.h"
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
						fwrite (buff ,sizeof(char),  strlen(buff) , fp );

//						write (fp ,buff,  strlen(buff)+1);

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
						fwrite (buff ,sizeof(char),  strlen(buff) , fp );
//						write (fp ,buff,  strlen(buff)+1);
						printf("%s  %s %ld  ", Tmpnode->name, Tmpnode->type, Tmpnode->ndim);


						tot_dim = 1;
						for(i=0; i<Tmpnode->ndim; i++){

							bzero(buff, sizeof(buff));
							if( snprintf(buff, MAX_WORD_LENGTH,"%ld ",Tmpnode->fdim[i]) < 0)
	      						        Perror("snprintf");
							fwrite (buff ,sizeof(char),  strlen(buff) , fp );
//							write (fp ,buff,  strlen(buff)+1);

							printf("%ld ", Tmpnode->fdim[i]);
							tot_dim = tot_dim * Tmpnode->fdim[i];
						}

						if( snprintf(buff, MAX_WORD_LENGTH,"\n") < 0)
	      						        Perror("snprintf");
						fwrite (buff ,sizeof(char),  strlen(buff) , fp );
//						write (fp ,buff,  strlen(buff)+1);

						printf("\n");
						for (i=0; i<tot_dim; i++){
							bzero(buff, sizeof(buff));
							if( snprintf(buff, MAX_WORD_LENGTH,"%d ", Tmpnode->data.i[i]) < 0)
	      					        	Perror("snprintf");
							fwrite (buff ,sizeof(char),  strlen(buff) , fp );
//							write (fp ,buff,  strlen(buff)+1);
							
							printf("%d ", Tmpnode->data.i[i]);
						}
						if( snprintf(buff, MAX_WORD_LENGTH,"\n") < 0)
	      						        Perror("snprintf");
						fwrite (buff ,sizeof(char),  strlen(buff) , fp );
//						write (fp ,buff,  strlen(buff)+1);

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
		fwrite (buff ,sizeof(char),  strlen(buff) , fp );
//		write (fp ,buff,  strlen(buff)+1);

		printf("%s  %s %ld\n", List->name, List->type, List->ndim);
		Tmpnode = List->child;
		
		if(WriteData(Tmpnode,fp) != 0){
			Warning("Write data problem");
			return -1;
		};
	}

	return 0;
}
