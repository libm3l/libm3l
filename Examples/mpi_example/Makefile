PATHL  = $(shell pwd )

main:

	ln -sf ../../Source/data_util/libm3l.* .
	ln -sf ../../Source/mpi_util/libm3l_mpi* .
	mpicc  -g -o MPI_Example.out MPI_Example.c -L$(PATHL)  -lm3l -Wl,-rpath=$(PATHL) -lm   -L$(PATHL)  -lm3l_mpi -Wl,-rpath=$(PATHL)

clean:
	rm -f *.o
