# makefile help http://www.cs.swarthmore.edu/~newhall/unixhelp/howto_makefiles.html

# Compiler for C program
CC = g++

# Complier flag to be set
CFLAGS = -Wall

# Target program to build
TARGET = main
T_TYPE = cpp

# Output location
MKDIR_P    = mkdir -p
OUTPUT_DIR = ./out
OUTPUT     = $(OUTPUT_DIR)/$(TARGET)

all: out_dir $(TARGET) 

out_dir:
	$(MKDIR_P) $(OUTPUT_DIR)

$(TARGET): $(TARGET).$(T_TYPE)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(TARGET).$(T_TYPE)

clean:
	$(RM) $(TARGET)

