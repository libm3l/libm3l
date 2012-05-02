/*
 *     function Locator.c - searches subset of the Founds set according to specified loction info.
 * 
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
 *     
 *     Also add information on how to contact you by electronic and paper mail.
 *     contact: Adam.Jirasek@gmail.com
 */

/*
 * author: Adam Jirasek
 * date:   1-May-2012
 * version - 0.0
 */

/*
 * Modifications:
 * version	name		date		description (event. patch number)
 */

#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "Locator.h"
#include "Find.h"
#include "FunctionsPrt.h"
#include "Find_Source.h"

extern int optind;
static int verbose_flag;

find_t *locator(size_t call, find_t *Founds, const char *path_loc, opts_t *Popt)
{
/*
 * function looks for subset in nodel List
 * and specified by path and location
 * before this function is invoked, the Founds subset must be
 * created by calling Founds function
 */
	size_t i, j;
	path_t *parsed_path_loc;
	get_arg_t argsstr;
 	size_t call_number;
	
	node_t *Tmp_node;
/*
 * set temporarily the parent node of the search origin node to NULL
 */
	Tmp_node = Founds->Home_Node->parent;
/*
 * parse path location specification; IMP: do not forget destroy_pars_path(&parsed_path) once not needed
 */
 	if ( (parsed_path_loc = parse_path(path_loc)) == NULL){
		Error("Paths - failed");
		return (find_t *)NULL;
	}
/*
 * loop over all levels in path
 */
	for(j = 0; j< Founds->founds; j++){

		if(call == 1){
			call_number = 1;
		}
		else{
			call_number++;
		}
		
/*
* get lcoator arguments for each level in path
*/
		argsstr = get_arguments(parsed_path_loc->path[j]);
/*
 * loop over all Founds
 */
		for (i=0; i< Founds->founds; i++){
			
			

		}
	}
/*
 * free parsed_path 
 */	
	destroy_pars_path(&parsed_path_loc);
/*
 * upon leave, set the Founds origin node parent to what it was before
 */
	Founds->Home_Node->parent = Tmp_node;
		
	
//		parsed_path = parse_path("../../home/jka/ada//");
		
		
//		parsed_path = parse_path("~/../../home/jka/ada/   ");
//  		parsed_path = parse_path("~/../../*/N=1-3,5/SI_name=Wall/");
// 
// 		printf(" Number of segments is %ld\n",parsed_path->seg_count );
// 		for (i=0; i< parsed_path->seg_count; i++)
// 			printf(" Segment %d is %s\n", i, parsed_path->path[i]);
// 		
// 		argsstr = get_arguments(parsed_path->path[3]);
// 		printf("%c  %c   '%s'  '%s'\n", argsstr.first, argsstr.arg, argsstr.s_name, argsstr.args);
// 		argsstr = get_arguments(parsed_path->path[4]);
// 		printf("%c  %c   '%s'  '%s'\n", argsstr.first, argsstr.arg, argsstr.s_name, argsstr.args);
// 		argsstr = get_arguments(parsed_path->path[5]);
// 		printf("%c  %c   '%s'  '%s'\n", argsstr.first, argsstr.arg, argsstr.s_name, argsstr.args);
// 		
// 		destroy_pars_path(&parsed_path);
}