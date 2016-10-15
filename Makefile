# CONFIG

#compiler
CC = gcc


FILE 	= linear_bf

OBJECTS  = $(FILE).o
#PROGS    = $(FILE).e



INCLUDE_FLAGS_C = -I/usr/local/include
INCLUDE_FLAGS_CUDA =  -I/usr/local/cuda/include

LINK_FLAGS_C = -L/usr/local/lib -lgsl -lgslcblas -lm 
LINK_FLAGS_CUDA = -L/usr/local/cuda/lib64 -lcuda -lcudart -lcurand

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

#cuda_functions.o:	cuda_functions.cu
#			nvcc --gpu-architecture=sm_20 -c -o cuda_functions.o cuda_functions.cu
