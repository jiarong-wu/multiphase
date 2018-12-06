CC = g++

EXE = Possion

SRC_DIR = source
OBJ_DIR = object
INC_DIR = include
LIB_DIR = ../eigen


SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
INC = $(wildcard $(INC_DIR)/*.h)

CFLAGS = -I$(INC_DIR) -I$(LIB_DIR)

run: $(EXE)
	./$(EXE)

$(EXE): $(OBJ) 
	$(CC) $^  -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean	

clean:
	rm $(EXE) $(OBJ)
