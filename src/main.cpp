#include <iostream>
#include <string>

#include "Server.h"
#include "Client.h"

int main(int argv, char* argc[])
{
	std::cout << "(1) Server" << std::endl;
	std::cout << "(2) Client" << std::endl;
	
	std::string input;
	std::getline(std::cin, input);

	if (input == "1")
	{
		Server server;
		server.run();
	}
	else
	{
		Client client;
		client.run();
	}

	return 0;
}
