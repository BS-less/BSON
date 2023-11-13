CC = gcc
COMPILE = -I../include -O3 -Wall -Wpedantic -Werror -Wno-unused
LINKER = -L../ -Wl,-rpath='$$ORIGIN/..' -lbson
TARGET = run

SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))

all: $(OBJECTS) $(TARGET) clean

$(OBJECTS): $(SOURCES)
	$(CC) -c $(SOURCES) $(COMPILE)

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(LINKER)

clean:
	rm *.o
