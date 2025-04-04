LIBRARY = libced.a

SOURCE = ./src
INCLUDES = ./include
BUILD = ./build

C_FILES = $(SOURCE)/memory.c \
		  $(SOURCE)/bufio.c \
		  $(SOURCE)/string.c \

C_OBJECTS = $(patsubst $(SOURCE)/%.c, $(BUILD)/%.o, $(C_FILES))

CC = gcc
AR = ar

CFLAGS = -O3 \
		 -Wall \
		 -Werror \
		 -Wpedantic \
		 -std=c23 \

AR_FLAGS = rcs

.PHONY: clean

$(BUILD)/$(LIBRARY): $(C_OBJECTS)
	@echo [INFO] Creating static library from $^
	@$(AR) $(AR_FLAGS) $(BUILD)/$(LIBRARY) $^

$(BUILD)/%.o: $(SOURCE)/%.c
	@echo [INFO] Compiling $? to $@
	@if [ ! -d $(BUILD) ]; then mkdir $(BUILD); fi
	@$(CC) $(CFLAGS) -I$(INCLUDES) -c $< -o $@

clean: $(BUILD)
	@echo [INFO] Removing $< directory
	@rm -rf $<
