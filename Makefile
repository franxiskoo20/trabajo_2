prefix=/usr/local
CC = g++

CFLAGS = -g -Wall

SRC = miprograma.cpp
OBJ = miprograma.o
APP = miprograma

all: $(OBJ)
	$(CC) $(CFLAGS) -o $(APP) $(OBJ) $(LIBS) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) $(OBJ) $(APP)

install: $(APP)
	install -m 0755 $(APP) $(prefix)/bin

uninstall: $(APP)
	$(RM) $(prefix)/bin/$(APP)
