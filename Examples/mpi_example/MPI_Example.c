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



#include "mpi.h"
#include "libm3l.h"
#include "libm3l_mpi.h"

lmint_t main(int argc, char ** argv) 
{	
    node_t *Gnode=NULL, *NewList;
    
    lmint_t rank, size;
    lmsize_t dim[1], i;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    
    printf(" Partition %d %d\n", rank, size);

    Gnode = m3l_Mklist("main", "DIR", 0, 0, (node_t **)NULL, (const char *)NULL, (const char *)NULL, (char *)NULL);
    
    dim[0] = 5;
    
    NewList = m3l_Mklist("Float", "F", 1, dim, &Gnode, "/main/", "./", (lmchar_t *)NULL);
    
    for(i=0; i<5; i++)
        NewList->data.f[i] = 0.;
        
    NewList->data.f[rank]=rank;  
    
    m3l_mpi_sum(MPI_COMM_WORLD, NewList);
    
    for(i=0; i<5; i++)
       printf(" Rank, value %d  %ld %f\n", rank, i,  NewList->data.f[i]);
    
    MPI_Finalize();
    
    
  return 0;

}
