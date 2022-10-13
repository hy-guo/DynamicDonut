CC		:= gcc
C_FLAGS := 
BIN		:= .
SRC		:= src
INCLUDE		:= include

LIBRARIES	:= -lm

EXECUTABLE	:= donut

all: $(BIN)/$(EXECUTABLE)

clean:
	-$(RM) $(BIN)/$(EXECUTABLE)

run: all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.c
	$(CC) $(C_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)