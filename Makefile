CC = gcc
COMPILE = -Iinclude -fPIC -O3 -Wall -Wpedantic -Werror -Wno-unused
LINKER = -fPIC -Wall
TARGET = libbson.so

SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst src/%.c,%.o,$(SOURCES))

all: $(OBJECTS) $(TARGET) clean

$(OBJECTS): $(SOURCES)
	$(CC) -c $(SOURCES) $(COMPILE)

$(TARGET): $(OBJECTS)
	$(CC) -shared -o $(TARGET) $(OBJECTS) $(LINKER)

install:
	cp $(TARGET) /usr/local/lib/
	cp -r include /usr/local/include/bson

uninstall:
	rm /usr/local/lib/$(TARGET)
	rm -r /usr/loca/include/bson

clean:
	rm *.o


