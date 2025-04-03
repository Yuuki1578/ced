PROGRAM = ced

SOURCE = ./src
INCLUDES = ./include
BUILD = ./build

C_FILES = $(SOURCE)/main.c \
		  $(SOURCE)/memory.c \
		  $(SOURCE)/bufio.c \
		  $(SOURCE)/string.c

C_OBJECTS = $(patsubst $(SOURCE)/%.c, $(BUILD)/%.o, $(C_FILES))

CC = gcc
CFLAGS = -O3 \
		 -Wall \
		 -Werror \
		 -Wpedantic \
		 -std=c23 \

.PHONY: clean

$(BUILD)/$(PROGRAM): $(C_OBJECTS)
	@echo [INFO] Linking $^ to $@
	@$(CC) $(CFLAGS) -s $^ -o $@

$(BUILD)/%.o: $(SOURCE)/%.c
	@echo [INFO] Compiling $? to $@
	@if [ ! -d $(BUILD) ]; then mkdir $(BUILD); fi
	@$(CC) $(CFLAGS) -I$(INCLUDES) -c $< -o $@

clean: $(BUILD)
	@echo [INFO] Removing $< directory
	@rm -rf $<
