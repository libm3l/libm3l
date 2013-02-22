PATHL  = $(shell pwd)

all:

	ln -sf ../../Source/libm3l.* .
	gcc -c ACK.c
	gcc -c CrtSEMSHM.c
	gcc -c SignalC.c
	gcc -c Transfer_Fce.c
	
	gcc -o Server_Example.out Server_Example.c ACK.o CrtSEMSHM.o  SignalC.o Transfer_Fce.o -L$(PATHL)  -lm3l -Wl,-rpath=$(PATHL)
	gcc -o Client1.out Client1.c ACK.o -L$(PATHL)  -lm3l -Wl,-rpath=$(PATHL)
	gcc -o Client2.out Client2.c ACK.o -L$(PATHL)  -lm3l -Wl,-rpath=$(PATHL)


clean:
	rm *.o *.out
