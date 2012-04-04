
SFILES= \
add_list.c \
Cat.c \
cat_list.c \
Client_Open_Socket.c\
Find.c \
Find_Source.c \
FunctionsPrt.c \
IO.c \
Mount.c\
ReadData.c \
ReadDir.c \
ReadDescriptor.c \
Rm.c \
rm_list.c \
udf_rm.c \
Umount.c \
WriteData.c \
Write2Socket.c \


OFILES=$(SFILES:%.c=%.o)

CFLAGS = -fbounds-check -fstack-check -g 
LFLAGS = -fbounds-check -fstack-check -g 
#CFLAGS =
#LFLAGS =

all:
	for file in $(SFILES); do \
		echo $$file; \
		gcc $(CFLAGS) -c  $$file; \
	done

	gcc $(LFLAGS) -o LinkedStr_TEST.out LinkedStr_TEST.c $(OFILES)

clean:
	rm *.o LinkedStr_TEST.out
