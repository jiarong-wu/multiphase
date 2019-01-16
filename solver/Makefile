CC = g++

EXE = RUN

SRC_DIR = source
OBJ_DIR = object
INC_DIR = include
LIB_DIR = ../eigen
OUT_DIR_SOL = output/solutions
OUT_DIR_ANA = output/analysis

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
INC = $(wildcard $(INC_DIR)/*.h)

OUT_SOL = $(wildcard $(OUT_DIR_SOL)/*.vtk)
OUT_ANA = $(wildcard $(OUT_DIR_ANA)/*.out)

CFLAGS = -pg -I$(INC_DIR) -I$(LIB_DIR) -std=c++11

$(EXE): $(OBJ) 
	$(CC) $(CFLAGS) $^  -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean	

clean:
	rm $(EXE) $(OBJ) $(OUT_SOL) $(OUT_ANA)
