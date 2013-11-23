CC	= g++
EXE     = game
C_FLAGS	= -Wall -std=c++11 
L_FLAGS	= -lcrypt -lz

# Source Files
SRC_FILES := $(wildcard *.cpp)

# Directories
OBJ_DIR_D = ../obj/Debug
BIN_DIR_D = ../bin/Debug
OBJ_DIR_R = ../obj/Release
BIN_DIR_R = ../bin/Release

# Object Files
OBJ_FILES_D := $(patsubst %.cpp,$(OBJ_DIR_D)/%.o,$(SRC_FILES))
OBJ_FILES_R := $(patsubst %.cpp,$(OBJ_DIR_R)/%.o,$(SRC_FILES))

all:: debug live

debug:	$(OBJ_FILES_D) | $(BIN_DIR_D)
	rm -f $(BIN_DIR_D)/$(EXE)
	$(CC) -O -ggdb -o $(BIN_DIR_D)/$(EXE) $(OBJ_FILES_D) $(L_FLAGS)

live:	$(OBJ_FILES_R) | $(BIN_DIR_R)
	rm -f $(BIN_DIR_R)/$(EXE)
	$(CC) -O2 -o $(BIN_DIR_R)/$(EXE) $(OBJ_FILES_R) $(L_FLAGS)

$(OBJ_DIR_D)/%.o: %.cpp | $(OBJ_DIR_D)
	$(CC) -O -ggdb $(C_FLAGS) -c -o $@ $<

$(OBJ_DIR_R)/%.o: %.cpp | $(OBJ_DIR_R)
	$(CC) -O2 $(C_FLAGS) -c -o $@ $<

$(OBJ_DIR_D):
	mkdir -p $(OBJ_DIR_D)

$(OBJ_DIR_R):
	mkdir -p $(OBJ_DIR_R)

$(BIN_DIR_D):
	mkdir -p $(BIN_DIR_D)

$(BIN_DIR_R):
	mkdir -p $(BIN_DIR_R)

install: $(BIN_DIR_R)/$(EXE)
	cp -f $(BIN_DIR_R)/$(EXE) ../../$(EXE)

clean:
	rm -f *.o *.*~ *.bak *.orig *.save
	rm -f $(OBJ_DIR_D)/*
	rm -f $(OBJ_DIR_R)/*
	rm -f $(BIN_DIR_D)/*
	rm -f $(BIN_DIR_R)/*
