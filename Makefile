CPP=g++
CPPFLAGS=-Iinclude -Llibs
CPP_FILES=src/main.cpp src/Server.cpp src/Client.cpp src/utils/LoopTimer.cpp src/ecs/Systems/PhysicsSystem.cpp src/math/VectorMath.cpp src/ecs/Systems/DeathSystem.cpp src/managers/ClientManager.cpp src/factories/ObjectFactory.cpp src/ecs/Systems/ActionSystem.cpp
LIB_FILES=-lsfml-system -lsfml-window -lsfml-graphics -lsfml-network

Game: src/main.cpp
	$(CPP) $(CPPFLAGS) -o release/Game $(CPP_FILES) $(LIB_FILES)
