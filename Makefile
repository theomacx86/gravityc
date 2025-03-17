CC=clang
CFLAGS= -I$(shell brew --prefix)/include -L$(shell brew --prefix)/lib -lglfw -framework OpenGL -I$(shell pwd)

SOURCES:=$(wildcard *.c)
OBJECTS:=$(patsubst %.c, %.o, $(SOURCES))
TARGET:=program

%.o: %.c 
	${CC} ${CFLAGS} -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

all: $(TARGET)

.PHONY: all clean
clean:
	rm -f $(OBJECTS) $(TARGET)
