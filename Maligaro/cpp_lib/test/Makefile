# This is my big Makefile which is used to compile all my c code


TOOL_DIR = ../01_cpp_lib
TOOL_CPP = $(wildcard ../c01_tools/*.cpp)
TOOL_OBJ = $(TOOL_CPP:.cpp=.o)

TOMO_DIR = ../02_forward_tomo
TOMO_CPP = $(wildcard ../02_forward_tomo/*.cpp)
TOMO_OBJ = $(TOMO_CPP:.cpp=.o)

ESF_DIR = ../../c_lib/c02_empirical_wavelet
ESF_CPP = $(wildcard ../c02_empirical_wavelet/*.c)
ESF_OBJ = $(ESF_CPP:.c=.o)

SACHOME = /opt/sac
#SOURCE = ./empirical_wavelet.c
SOURCE = ./main.cpp
OBJ = $(SOURCE:.cpp=.o)

CC = g++ -std=c++11 -Wno-unused-variable -Wunused-but-set-variable
#DEBUG = -g 
DEBUG = 
LIBS = libHongyucpp.a 


CFLAGS = -Wall -c ${DEBUG} -I$(TOOL_DIR)  -Wparentheses -Wno-unused-variable  \
		 -I$(SACHOME)/include -I./  -Wunused-but-set-variable   \
		 -lm -I$(TOMO_DIR) -I$(ESF_DIR)
		 #-I$(SACHOME)/include -I./  -Wunused-but-set-variable  -L$(TOOL_DIR) 
LFLAGS = -Wall ${DEBUG} -m64 -L$(TOOL_DIR) -L$(SACHOME)/lib -L./  -L. \
		 -I$(TOOL_DIR)  -I. -lsacio -lm -Wunused-but-set-variable -L$(TOMO_DIR) \
		 -L$(ESF_DIR)


all: test 

test: test
	./test

test: $(OBJ) libHongyucpp.a 
	$(CC) -o $@ $^  $(LFLAGS)

$(OBJ): $(SOURCE)
	$(CC) -c -o $@ $< $(CFLAGS)

libHongyucpp.a: $(TOOL_OBJ)  $(TOMO_OBJ) $(ESF_OBJ)
	ar cr $@ $^

%.o: $(TOOL_DIR)/%.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: $(TOMO_DIR)/%.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: ${ESF_DIR}/%.c
	gcc -c -o $@ $< $(CFLAGS)



clean :
	\rm *.o *~ EWM *.a $(TOOL_OBJ) 
