# CONFIG

#compiler
CC = gcc


FILE 	= linear_bf

OBJECTS  = $(FILE).o



INCLUDE_FLAGS_C = -I/usr/local/include

LINK_FLAGS_C = -L/usr/local/lib -lgsl -lgslcblas -lm 

all:            	run

clean:
			rm -f *.o
clear:			
			rm *.*~
o:		
			open $(FILE).c

run:			$(FILE).o
			$(CC) $(LINK_FLAGS_C)  -o run $(FILE).o

$(FILE).o:		$(FILE).c
			$(CC) -Wall $(INCLUDE_FLAGS_C)  -c -o $(FILE).o $(FILE).c

