#*****************************************************************              
# File:   Makefile                                                              
# Author:
# Author:
# Author:
# Author:
# Date:   
# Coms:       
#*****************************************************************              

#---------------------------------------------------------
# macros
RM = /bin/rm -rf
CC = g++
#---------------------------------------------------------
# vars
EXEC = assignment3

#PARSER = ParseInput
#OPT = Optimizer
#APPR = Approximation

#LIBRARIES = ${PARSER}.hpp ${OPT}.hpp ${APPR}.hpp
SRC_DIR = src
BUILD_DIR = build
TARGET = bin

SOURCES = ${EXEC}.o
OBJECTS = $(patsubst %.o,$(BUILD_DIR)/%.o,$(SOURCES))

DOCS_CONFIG = doxygen-docs
GEN_DOCS = html

#---------------------------------------------------------
# flags
CPPFLAGS= -std=gnu++17 -O3 -fopenmp #0 -g  -fsanitize=address

all: $(EXEC)
#---------------------------------------------------------
$(EXEC): $(OBJECTS)
	@mkdir -p $(TARGET)
	${CC} $^ -o $(TARGET)/$@ ${CPPFLAGS}
#---------------------------------------------------------
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) -c $(CPPFLAGS) $< -o $@
#---------------------------------------------------------
test: $(EXEC)
	${PYTHON} testTSP.py
#---------------------------------------------------------
docs: $(EXEC)
	${DOCS} ${DOCS_CONFIG}
#---------------------------------------------------------
clean:
	$(RM) $(TARGET) $(BUILD_DIR) ${GEN_DOCS}