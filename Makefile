CC		:= gcc
C_FLAGS := 
BIN		:= .
SRC		:= src
INCLUDE		:= include

LIBRARIES	:= -lm

EXECUTABLE	:= dynamic_donut

all: $(BIN)/$(EXECUTABLE)

clean:
	-$(RM) $(BIN)/$(EXECUTABLE)

run: all
	./$(BIN)/$(EXECUTABLE)

donut: all
	./$(BIN)/$(EXECUTABLE) donut

heart: all
	./$(BIN)/$(EXECUTABLE) heart

$(BIN)/$(EXECUTABLE): $(SRC)/*.c
	$(CC) $(C_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)