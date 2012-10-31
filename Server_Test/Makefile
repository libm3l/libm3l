PATHL  = $(shell pwd)

all:
	gcc -c -g ACK.c
	gcc -c -g CrtSEMSHM.c
	gcc -c -g SignalC.c
	gcc -c -g Transfer_Fce.c

	gcc  -g -o Server_Example.out Server_Example.c ACK.o CrtSEMSHM.o  SignalC.o Transfer_Fce.o -L/$(PATHL)  -lm3l -Wl,-rpath=$(PATHL)
	gcc  -g -o Client1.out Client1.c ACK.o -L/$(PATHL)  -lm3l -Wl,-rpath=$(PATHL)
	gcc  -g -o Client2.out Client2.c ACK.o -L/$(PATHL)  -lm3l -Wl,-rpath=$(PATHL)

#	gcc  -g -o Client2.out Client2.c ACK.o libm3l.a

# 	gcc -static -o Server_Example.out Server_Example.c  libm3l.a
# 	gcc -static -o Client_Example.out Client_Example.c  libm3l.a


clean:
	rm *.o *.out
