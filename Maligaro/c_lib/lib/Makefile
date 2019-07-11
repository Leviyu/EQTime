# This is my big Makefile which is used to compile all my c code


TOOL_DIR = ../c01_tools
TOOL_CPP = $(wildcard ../c01_tools/*.c)
TOOL_OBJ = $(TOOL_CPP:.c=.o)

ESF_DIR = ../c02_empirical_wavelet
ESF_CPP = $(wildcard ../c02_empirical_wavelet/*.c)
ESF_OBJ = $(ESF_CPP:.c=.o)

SACHOME = /opt/sac
#SOURCE = ./empirical_wavelet.c
SOURCE = ./main.c
OBJ = $(SOURCE:.c=.o)

CC = gcc -Wno-unused-variable
#DEBUG = -g 
DEBUG = 
LIBS = libHongyu.a 


CFLAGS = -Wall -c ${DEBUG} -I$(TOOL_DIR) -I$(ESF_DIR) -Wparentheses -Wno-unused-variable  \
		 -I$(SACHOME)/include -I./  -Wunused-but-set-variable  -L$(TOOL_DIR) -L$(ESF_DIR) \
		 -lm
LFLAGS = -Wall ${DEBUG} -m64 -L$(TOOL_DIR) -L$(SACHOME)/lib -L./ -L$(ESF_DIR) -L. \
		 -I$(TOOL_DIR) -I$(ESF_DIR) -I. -lsacio -lm -Wunused-but-set-variable
		 #-I$(TOOL_DIR) -I$(ESF_DIR) -I. -lsacio -lm -lHongyu


all: test 

test: EWM
	#./EWM

#EWM:  $(OBJ) $(LIBS)
#EWM:  empirical_wavelet.o  hello2.o libHongyu.a
#EWM:  empirical_wavelet.o   libHongyu.a
#EWM:  empirical_wavelet.o   libHongyu.a
	#gcc -o $@ $^ $(LFLAGS)  

EWM: $(OBJ) libHongyu.a 
	$(CC) -o $@ $^  $(LFLAGS)


#hello2.o: ../c01_tools/hello2.c
	#gcc -c  -o $@ $^ $(CFLAGS)

$(OBJ): $(SOURCE)
	gcc -c -o $@ $< $(CFLAGS)

libHongyu.a: $(TOOL_OBJ) $(ESF_OBJ)
	ar cr $@ $^

#libESF.a: $(ESF_OBJ)
	#ar cr $@ $^

#$(TOOL_DIR)/%.o: $(TOOL_DIR)/%.c
%.o: $(TOOL_DIR)/%.c
	gcc -c -o $@ $< $(CFLAGS)

$(ESF_DIR)/%.o: $(ESF_DIR)/%.c
	gcc -c -o $@ $< $(CFLAGS) 




clean :
	\rm *.o *~ EWM *.a $(TOOL_OBJ) $(ESF_OBJ)
