# SFML_PATH=/usr/local/SFML-2.2/
# SFML_PATH=/usr/local/Cellar/sfml/2.5.1/
CXXFLAGS= -std=c++14 -Wall -Wpedantic -I${SFML_PATH}include/
LDFLAGS=-L${SFML_PATH}lib/
CFLAGS=-g -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -pthread
CPPFLAGS=
LDLIBS=
LIBS=
CPP=g++

all: client

client: main.o entity.o tile.o builder.o world.o game.o client.o myserver.o 
	$(CPP) $(CXXFLAGS) $(LDFLAGS) $(LIBS) $^ -o $@ $(CFLAGS)

server: accepter.o receiver.o myserver.o message.o network.o
	$(CPP) $(CXXFLAGS) $(LDFLAGS) $(LIBS) $^ -o $@ $(CFLAGS)
	
clean:
	\rm -f *.o client server
