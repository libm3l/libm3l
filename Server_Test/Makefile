PATHL  = $(shell pwd)

all:
	gcc -c -g ACK.c
	gcc -c -g CrtSEMSHM.c
	gcc -c -g SignalC.c
	gcc -c -g Transfer_Fce.c

	gcc  -g -o Server_Example.out Server_Example.c ACK.o CrtSEMSHM.o  SignalC.o Transfer_Fce.o -L$(PATHL)  -lm3l -Wl,-rpath=$(PATHL)
	gcc  -g -o Client1.out Client1.c ACK.o -L$(PATHL)  -lm3l -Wl,-rpath=$(PATHL)
	gcc  -g -o Client2.out Client2.c ACK.o -L$(PATHL)  -lm3l -Wl,-rpath=$(PATHL)
	gcc  -g -o Client2_TRINITAS.out Client2_TRINITAS.c ACK.o -L$(PATHL)  -lm3l -Wl,-rpath=$(PATHL)
	gcc  -g -o Server_TRINITAS.out Server_TRINITAS.c ACK.o CrtSEMSHM.o  SignalC.o Transfer_Fce.o -L$(PATHL)  -lm3l -Wl,-rpath=$(PATHL)

#	gcc  -g -o Client2.out Client2.c ACK.o libm3l.a

# 	gcc -static -o Server_Example.out Server_Example.c  libm3l.a
# 	gcc -static -o Client_Example.out Client_Example.c  libm3l.a


clean:
	rm *.o *.out

#
#     Client1 shakes hand with Client2 
#     execure Server_Example.out #portno   + Client1.out hostIP Portno  + Client2.out hostIP Portno
#
#     Client1 shakes hand with Clinet2_TRINITAS - unlike Cline2 which opens socket gets data sends data 
#     Client2_TRINITAS opens socket, gets data, closes socket, opens socket, sends data and closes 
#     socket - prepared for scenarion that sending and receiving data in TRINITAS is not done at one point
#
