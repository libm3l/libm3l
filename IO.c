/*
 * IO operations
 */
 
#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "IO.h"
#include "FunctionsPrt.h"
#include "Find_Source.h"
#include "udf_rm.h"
#include "ReadDir.h"
#include "ReadDescriptor.h"

extern void Perror(const char *);
extern void Error(const char *);

/*
 * function reads file and allocates structure
 */
node_t *Fread(const char *name)
{
	FILE *fp;
	node_t *Lnode;

	Lnode = NULL;

	printf("Reading file %s\n", name);

	if ( (fp = fopen(name,"r")) == NULL)
		Perror("fopen");

	if( (Lnode = read_file(fp)) == NULL)
	Perror("ReadDir");
 /*
  * end of reading the file   - while (   ( fgets(buff, MAXLINE, fp) != NULL) ) {  -- loop
  */
	if( fclose (fp) != 0)
		Perror("fclose");

	return Lnode;
}
