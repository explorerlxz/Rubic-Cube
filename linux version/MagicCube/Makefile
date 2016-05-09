CC = gcc
CFLAGS = -Wall
GLLIBS = -lGL -lGLU -lglut
TARGET = MagicCube
$(TARGET): main.o glFunc.o Cube.o image.o
	$(CC) $(CFLAGS) -o $(TARGET) $^ $(GLLIBS)
main.o:main.c glFunc.o
	$(CC) $(CFLAGS) -c main.c  $(lib)
glFunc.o:glFunc.c image.o Cube.o glFunc.h
	$(CC) $(CFLAGS) -c glFunc.c $(GLLIBS)
Cube.o:Cube.c Cube.h
	$(CC) $(CFLAGS) -c Cube.c  $(GLLIBS)
image.o:image.c image.h
	$(CC) $(CFLAGS) -c image.c  $(GLLIBS)
clean:
	rm -rf *.o
	rm -rf *.gch
	rm $(TARGET)
