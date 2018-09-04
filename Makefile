CPP=g++
CPPFLAGS=-Iinclude -Llibs
CPP_FILES=src/main.cpp src/Server.cpp src/Client.cpp src/utils/LoopTimer.cpp
LIB_FILES=-lsfml-system -lsfml-window -lsfml-graphics -lsfml-network

Game: src/main.cpp
	$(CPP) $(CPPFLAGS) -o release/Game $(CPP_FILES) $(LIB_FILES)
