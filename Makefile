CPP=g++
CPPFLAGS=-Iinclude -Llibs
CPP_FILES=src/main.cpp
LIB_FILES=-lsfml-system -lsfml-window -lsfml-graphics -lsfml-network

GameClient: src/main.cpp
	$(CPP) $(CPPFLAGS) -o release/GameClient $(CPP_FILES) $(LIB_FILES)
