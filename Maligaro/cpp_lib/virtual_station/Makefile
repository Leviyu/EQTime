# This is my big Makefile which is used to compile all my c code


TOOL_DIR = ../01_cpp_lib
TOOL_CPP = $(wildcard ../01_cpp_lib/*.cpp)
TOOL_OBJ = $(TOOL_CPP:.cpp=.o)

TOMO_DIR = ../02_forward_tomo
TOMO_CPP = $(wildcard ../02_forward_tomo/*.cpp)
TOMO_OBJ = $(TOMO_CPP:.cpp=.o)

#ESF_DIR = ../../c_lib/c02_empirical_wavelet
#ESF_CPP = $(wildcard ../../c_lib/c02_empirical_wavelet/*.c)
#ESF_OBJ = $(ESF_CPP:.c=.o)

SACHOME = /usr/local/sac
#SOURCE = ./empirical_wavelet.c
SOURCE = ./main.cpp
OBJ = $(SOURCE:.cpp=.o)

#CC = g++ -std=c++11 -Wno-unused-variable -Wunused-const-variable -Wunused-command-line-argument
CC = c++ 
#DEBUG = -g 
DEBUG = 
LIBS = libHongyucpp.a 

CXXFLAGS = -Wall -c ${DEBUG} -I$(TOOL_DIR)  \
		 -I$(SACHOME)/include -I./  		\
		  -I$(TOMO_DIR)  -L$(TOOL_DIR)  \
		  -std=c++11 -Wunused-but-set-variable \
		  -Wunused-but-set-variable

#CXXFLAGS = -Wall -c ${DEBUG} -I$(TOOL_DIR)  -Wparentheses -Wno-unused-variable  \
		 -I$(SACHOME)/include -I./  -Wunused-but-set-variable   \
		  -I$(TOMO_DIR)  -L$(TOOL_DIR) 
		 #-I$(SACHOME)/include -I./  -Wunused-but-set-variable  -L$(TOOL_DIR)  -I$(ESF_DIR)
LFLAGS = -Wall ${DEBUG} -m64 -L$(TOOL_DIR) -L$(SACHOME)/lib -L./  -L. \
		 -I$(TOOL_DIR)  -I. -lsacio -lm -Wunused-but-set-variable -L$(TOMO_DIR)  
		 #-L$(ESF_DIR) 


all: virtual_station

virtual_station: virtual_station
	#./virtual_station

virtual_station: $(OBJ) libHongyucpp.a $(TOOL_DIR)/hongyulibcpp.h $(TOMO_DIR)/forward_tomography.h
	$(CC) -o $@ $^  $(LFLAGS)

$(OBJ): $(SOURCE) 
	$(CC) -c -o $@ $^ $(CXXFLAGS)

#libHongyucpp.a: $(TOOL_OBJ)  $(TOMO_OBJ) $(ESF_OBJ)
libHongyucpp.a: $(TOOL_OBJ)  $(TOMO_OBJ)
	ar cr $@ $^

#%.o: $(TOOL_DIR)/%.cpp
$(TOOL_DIR)/%.o: $(TOOL_DIR)/%.cpp $(TOOL_DIR)/%.h
	$(CC) -c -o $@ $^ $(CXXFLAGS) 
#$(TOOL_OBJ): $(TOOL_DIR)/%.cpp $(TOOL_DIR)/%.h
#$(TOOL_OBJ): $(TOOL_CPP) $(TOOL_DIR)/%.h
#$(TOOL_OBJ): $(TOOL_CPP) 

$(TOMO_DIR)/%.o: $(TOMO_DIR)/%.cpp $(TOMO_DIR)/%.h
#$(TOMO_OBJ): $(TOMO_DIR)/%.cpp  $(TOMO_DIR)/%.h
#$(TOMO_OBJ): $(TOMO_CPP)  $(TOMO_DIR)/%.h
#$(TOMO_OBJ): $(TOMO_CPP)  
	$(CC) -c -o $@ $^ $(CXXFLAGS) 
	#g++ -c -o -std=c++11 $@ $^ $(CXXFLAGS)

#$(ESF_DIR)/%.o: ${ESF_DIR}/%.c
#$(ESF_OBJ): ${ESF_DIR}/%.c
	#gcc -c -o $@ $< $(CXXFLAGS)



clean :
	\rm *.o *~ EWM *.a $(TOOL_OBJ) $(TOMO_OBJ) 
	#\rm *.o *~ EWM *.a $(TOOL_OBJ) $(TOMO_OBJ) $(ESF_OBJ)
