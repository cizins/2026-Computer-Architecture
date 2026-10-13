# CP1 BITREV C baseline
CC       = gcc
CFLAGS  ?= -std=c11 -O2
override CFLAGS += -Wall -Wextra -Werror
TARGET   = test_bitrev
SRCS     = bitrev.c test_bitrev.c

# Use native Windows commands for execution and cleanup.
ifeq ($(OS),Windows_NT)
SHELL := cmd.exe
.SHELLFLAGS := /C
TARGET := $(TARGET).exe
RUN_TARGET = .\$(TARGET)
CLEAN_TARGET = if exist "$(TARGET)" del /Q "$(TARGET)"
else
RUN_TARGET = ./$(TARGET)
CLEAN_TARGET = rm -f -- "$(TARGET)"
endif

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRCS) bitrev.h test_vectors.h
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

run: all
	$(RUN_TARGET)

clean:
	$(CLEAN_TARGET)
