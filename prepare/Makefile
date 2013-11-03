# custom path
#PATH1 = /home/f85/syi/.local

# set inc and lib
#INC=-I$(PATH1)/include/
#LIB=$(PATH1)/lib64/libpbc.a \
	 $(PATH1)/lib64/libgmp.a \
	-lssl \
	-lcrypto \

#LIB=-L$(PATH1)/lib64/libgmp.a \



#LIB=-lpbc \
	-lgmp \
	-lssl \
	-lcrypto \


#LIB=-L$(PATH1)/lib64 -Wl,-rpath $(PATH1)/lib64

# set environment optiom
CC = gcc
CFLAGS=-g -Wall
TARGET=demo


SOURCE= strmap_experiement.c strmap.c
OBJS=$(addsuffix .o, $(basename $(SOURCE)))



$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB) $(INC)


default: $(TARGET)

%.o:%.c
	$(CC) $(CFLAGS) -c $< $(INC) 

clean :
	rm -rf *.o $(TARGET) 
	
run :
	./$(TARGET)
