CC = gcc
LIBS = `pkg-config --libs gtk+-3.0`
CFLAGS = `pkg-config --cflags gtk+-3.0`
DEPS = hello.h util.h
OBJ = hello.o util.o

# DEPS is all the .h files
# OBJ is all the .o files

# left side says the .o depends on the .c and the DEPS
# generate the .o by compiling the .c using CC
# -c says to generate the object file
# -o $@ says to put the ouput of the compilation in the file left of :
# the $< is the first item in the DEPS list
# the $^ is all the items in the DEPS list

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

hello: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS) 

clean: $(OBJ) hello
	rm $(OBJ) hello *.txt



