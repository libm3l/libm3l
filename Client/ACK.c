/*
 * example of simple for-per request server.
 * Server receives a data set, print it out and send back an answer with it's PID number 
 * Author: Adam Jirasek 
 * Date:   Aug-2012
 */
#include "libm3l.h"
#include "ACK.h"


node_t *ackn(void)
{
	node_t *Gnode, *TmpNode;
	char *answer="ACKN";
	size_t *dim;
	
	if(  (Gnode = m3l_Mklist("Answer", "DIR", 0, 0, (node_t **)NULL, (const char *)NULL, (const char *)NULL, (char *)NULL)) == 0)
		Perror("m3l_Mklist");
	
	dim = (size_t *) malloc( 1* sizeof(size_t));
	dim[0] = strlen(answer)+1;
	
	if(  (TmpNode = m3l_Mklist("ANSWER", "C", 1, dim, &Gnode, "/Answer", "./", "--no_malloc", (char *)NULL)) == 0)
		Error("m3l_Mklist");
	TmpNode->data.c = answer;
	
	free(dim);
	
 	return Gnode;
}

node_t *solver_name(char *name)
{
	node_t *Gnode, *TmpNode;
	char *answer="ACKN";
	size_t *dim;
	
	if(  (Gnode = m3l_Mklist("Solver", "DIR", 0, 0, (node_t **)NULL, (const char *)NULL, (const char *)NULL, (char *)NULL)) == 0)
		Perror("m3l_Mklist");
	
	dim = (size_t *) malloc( 1* sizeof(size_t));
	dim[0] = strlen(name)+1;
	
	if(  (TmpNode = m3l_Mklist("Name", "C", 1, dim, &Gnode, "/Solver", "./", "--no_malloc", (char *)NULL)) == 0)
		Error("m3l_Mklist");
	TmpNode->data.c = name;
	
	free(dim);
	
 	return Gnode;
}