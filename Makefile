#OBJS specifies which files to compile as part of the project
OBJS = main.c prim.c shipgenmaths.c UVOPS.c

#CC specifies which compiler
CC = gcc

#COMPILER FLAGS specifies the additional compilation options
#-w supresses all warnings
COMPILER_FLAGS = -w

#LINKER FLAGS specifies the libraries we're linking
LINKER_FLAGS = -lm

#OBJ_NAME specifies the name of the executable
OBJ_NAME = RNGSHIPLin

#Target to compile executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
