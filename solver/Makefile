CC = g++

EXE = Flow

SRC_DIR = source
OBJ_DIR = object
INC_DIR = include
LIB_DIR = ../eigen
OUT_DIR = output

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
INC = $(wildcard $(INC_DIR)/*.h)

OUT = $(wildcard $(OUT_DIR)/*.vtk)

CFLAGS = -I$(INC_DIR) -I$(LIB_DIR) -std=c++11

# cell.o: $(SRC_DIR)/cell.cpp
# 	$(CC) $(CFLAGS) -c $< -o $@

run: $(EXE)
	./$(EXE)

$(EXE): $(OBJ) 
	$(CC) $^  -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean	

clean:
	rm $(EXE) $(OBJ) $(OUT)
