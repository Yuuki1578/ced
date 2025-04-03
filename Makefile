PROGRAM = ced

SOURCE = ./src
INCLUDES = ./include
BUILD = ./build

OBJECT = $(BUILD)/main.o \
		 $(BUILD)/memory.o \
		 $(BUILD)/io.o \
		 $(BUILD)/string.o

CC = gcc
CFLAGS = -O3 \
		 -Wall \
		 -Werror \
		 -Wpedantic \
		 -std=c23 \

.PHONY: clean

$(BUILD)/$(PROGRAM): $(OBJECT)
	@echo [INFO] Linking $^ to $@
	@$(CC) $(CFLAGS) -s $^ -o $@

$(BUILD)/%.o: $(SOURCE)/%.c
	@echo [INFO] Compiling $? to $@
	@if [ ! -d $(BUILD) ]; then mkdir $(BUILD); fi
	@$(CC) $(CFLAGS) -I$(INCLUDES) -c $< -o $@

clean: $(BUILD)
	@echo [INFO] Removing $< directory
	@rm -rf $<
