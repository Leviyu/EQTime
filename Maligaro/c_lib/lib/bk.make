# This is my big Makefile which is used to compile all my c code


TOOL_DIR = ../c01_tools
TOOL_CPP = $(wildcard ../c01_tools/*.c)
TOOL_OBJ = $(TOOL_CPP:.c=.o)

ESF_DIR = ../c02_empirical_wavelet
ESF_CPP = $(wildcard ../c02_empirical_wavelet/*.c)
ESF_OBJ = $(ESF_CPP:.c=.o)

SACHOME = /opt/sac
#SOURCE = main.c
SOURCE = ./empirical_wavelet.c 
OBJ = $(SOURCE:.c=.o)

CC = gcc
DEBUG = -g 
LIBS = libHongyu.a 


CFLAGS = -Wall -c ${DEBUG} -I$(TOOL_DIR) -I$(ESF_DIR) -Wparentheses -Wno-unused-variable  \
		 -I$(SACHOME)/include -I./  -Wunused-but-set-variable  -L$(TOOL_DIR) -L$(ESF_DIR) \
		 -lm
LFLAGS = -Wall ${DEBUG} -m64 -L$(TOOL_DIR) -L$(SACHOME)/lib -L./ -L$(ESF_DIR) -L. \
		 -I$(TOOL_DIR) -I$(ESF_DIR) -I. -lsacio -lm -lHongyu
		 #-I$(TOOL_DIR) -I$(ESF_DIR) -I. -lsacio -lm -lHongyu

all: test

test: EWM
	./EWM


#EWM: $(OBJ)  
#EWM: $(OBJ)  libHongyu.a libESF.a
#EWM: $(OBJ)  $(TOOL_DIR)/hello2.o
EWM: $(OBJ) ${LIBS}
	$(CC) $(LFLAGS)  -o $@ $^ 


#$(OBJ): $(SOURCE) 
	#$(CC) $(CFLAGS) -o $@ $< -I../c01_tools -L. -I.

#empirical_wavelet.o: empirical_wavelet.c
$(OBJ):$(SOURCE)
	$(CC) $(CFLAGS) -o $@ $<  

#$(TOOL_DIR)/hello2.o: $(TOOL_DIR)/hello2.c
	#$(CC) $(CFLAGS) -o $@ $< 

#%.o: ./%.c 
	#${CC} ${CFLAGS} -c -o $@ $< -lm


$(TOOL_OBJ): $(TOOL_CPP)
	${CC} ${CFLAGS} -c -o $@ $< 

#$(ESF_OBJ): $(ESF_CPP)
	#${CC} ${CFLAGS} -c -o $@ $< -lm

libHongyu.a: $(TOOL_OBJ)
	ar cr $@ $^ 

#libESF.a: $(ESF_OBJ)
	#ar cr $@ $^

clean :
	\rm *.o *~ EWM *.a $(TOOL_OBJ) $(ESF_OBJ)
