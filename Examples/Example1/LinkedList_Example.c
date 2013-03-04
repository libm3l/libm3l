/*
 *     Copyright (C) 2012  Adam Jirasek
 * 
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *     
 *     contact: libm3l@gmail.com
 * 
 */




#include "libm3l.h"


lmint_t main(void) 
{	
    node_t *Gnode=NULL, *RecNode=NULL, *Anode=NULL, *Tmpnode=NULL, *NewList=NULL;
    find_t *Founds=NULL;
    
    lmint_t i, count,countgrp, socketnr, j;

    lmsize_t *dim;
    
    lmchar_t name[255], type[30];
    lmchar_t *pc;
    
    path_t *parsed_path;

    lmint_t  num, wc;
    
    lmlongdouble_t test1;
    lmdouble_t test2;
    lmfloat_t test3;	
 

    FILE *fp;
    
    lmchar_t *node_path;
    
    
    lmsize_t maxsize = (lmsize_t)-1;
/*
 * read file
 */

	printf(" ----------------------------------  \n");		
  	if( (Gnode = m3l_Fread("FILE.DAT" , "--clean_empy_links", (lmchar_t *)NULL))  == NULL)
  		Perror("Linked_test: m3l_Fread");
	
	if(m3l_Cat(Gnode, "--all", "-P", "-L","--links",  "*",   (lmchar_t *)NULL) != 0)
 	        Error("CatData");
/* 
 * read second file 
 */	
	printf(" ----------------------------------  \n");		
	printf(" Reading file #2\n");		
	if( (Anode = m3l_Fread("TEST.dat", (lmchar_t *)NULL))  == NULL)
 		Perror("Linked_test: m3l_Fread");	

 		if(m3l_Cat(Anode, "--all", "-P", "-L", "--links", "*", (lmchar_t *)NULL) != 0)
 	                   Error("CatData");
/*
 * find Dir list containing file "Gobal_Data_Set" which contains text "Global_Data_Set"
 */
	printf(" Searching data set Dir containing file with text Global_Data_Set1 in Gnode #1\n");		

 		if( (Founds = m3l_Locate(Gnode, "/main/Dir", "/*/SV_Gobal_Data_Set=Global_Data_Set1",  (lmchar_t *)NULL)) != NULL){
			
			for(i=0; i < Founds->founds; i++){
	
				if(m3l_Cat(Founds->Found_Nodes[i]->List, "--all", "-P", "-L", "*", (lmchar_t *)NULL) != 0)
					Error("CatData");
			}
/* 
 * free memory allocated in m3l_Locate
 */
			m3l_DestroyFound(&Founds);
		}
		else
		{
			printf(" No founds\n");
		}
		
/*
 * copy "Additional_directory_TEST" from TEST.dat data structure to 
 * the fist "Dir" in Gnode (data set from the first file
 */ 
		printf(" ----------------------------------  \n");		
		printf(" Copying data set Additional_directory_TEST to the first data set Dir in Gnode\n");		
		m3l_Cp(&Anode, "/Main_DATA_Structure/Additional_directory_TEST", "/*/*", &Gnode, "/main/Dir", "/*/n=1", (lmchar_t *)NULL);   	   

		if(m3l_Cat(Gnode, "--all", "-P", "-L","--links",  "*",   (lmchar_t *)NULL) != 0)
 	        	Error("CatData");
/*
 * move "Additional_directory_TEST" from the first Dir to the second Dir in Gnode 
 */ 
		printf(" ----------------------------------  \n");		
		printf(" Moving data set Additional_directory_TEST in Gnode to the second data set Dir in Gnode\n");		

		m3l_Mv(&Gnode,  "/main/Dir/Additional_directory_TEST", "/*/*/*", &Gnode, "/main/Dir", "/*/n=2", (lmchar_t *)NULL);   /* rename node */
		if(m3l_Cat(Gnode, "--all", "-P", "-L","--links",  "*",   (lmchar_t *)NULL) != 0)
 	        	Error("CatData");
/*
 * link the "Additional_directory_TEST" from the second Dir to the first Dir
 */ 
		printf(" ----------------------------------  \n");		
  		printf(" Linking  data set Additional_directory_TEST in Gnode back to Dir #1\n");		
		m3l_Ln(&Gnode,  "/main/Dir/Additional_directory_TEST", "/*/*/*", &Gnode, "/main/Dir", "/*/n=1", (lmchar_t *)NULL);   /* rename node */

		if(m3l_Cat(Gnode,  "--all", "--links", "-P", "-L", "*", (lmchar_t *)NULL) != 0)
			Error("CatData");

/*
 * remove  the "Additional_directory_TEST" from the second Dir 
 * note that this produces empty link in Dir #1
 */ 
		printf(" ----------------------------------  \n");		
  		printf(" Removing  data set Additional_directory_TEST in Gnodein Dir #2\n");		
		m3l_Rm(&Gnode,  "/main/Dir/Additional_directory_TEST", "/*/n=2/*", (lmchar_t *)NULL);   /* rename node */

		if(m3l_Cat(Gnode,  "--all", "--links", "-P", "-L", "*", (lmchar_t *)NULL) != 0)
			Error("CatData");
/*
 * Empty links are not cleaned autoatically, you can specify option --clean_empty_links to do it
 */ 
		printf(" ----------------------------------  \n");		
		printf(" Number of empty links is %ld \n\n", m3l_Ln(NULL, NULL, NULL, &Gnode, NULL, NULL, "--clean_empty_links", (lmchar_t *)NULL));	
		if(m3l_Cat(Gnode,  "--all", "--links", "-P", "-L", "*", (lmchar_t *)NULL) != 0)
			Error("CatData");
/* 
 * add new data set called MADE_LIST which is an integer array to the main in Gnode
 */
		dim = (lmsize_t *) malloc( 1* sizeof(lmsize_t));
		dim[0] = 10;
	 	if(  (NewList = m3l_Mklist("MADE_LIST", "I", 1, dim, &Gnode, "/main", "./", (lmchar_t *)NULL)) == 0)
			Error("Mklist");
		free(dim);
		for (i=0; i<10; i++)
 			NewList->data.i[i]=2*i;

		printf(" ----------------------------------  \n");		
		printf(" New list is \n");

 		if(m3l_Cat(NewList,  "--all", "--links", "-P", "-L", "*", (lmchar_t *)NULL) != 0)
 			Error("CatData");

		printf(" ----------------------------------  \n");		
		printf(" ... and  modified main data set is \n\n");
		
		if(m3l_Cat(Gnode,  "--all", "--links", "-P", "-L", "*", (lmchar_t *)NULL) != 0)
			Error("CatData");
/*
 * remove both linked lists from memory
 */
		printf(" ----------------------------------  \n");		
		printf(" Writing Gnode to file - WriteFile.txt \n\n");

		m3l_Fwrite(Gnode, "WriteFile.txt", (lmchar_t *)NULL);

		if(m3l_Umount(&Gnode) != 1)
			Perror("m3l_Umount");
		if(m3l_Umount(&Anode) != 1)
			Perror("m3l_Umount");
	
  return 0;

}