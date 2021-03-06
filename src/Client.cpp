#include "Client.h"

#include <SFML/Window/Event.hpp>
#include <SFML/System/String.hpp>

#include <iostream>

#include "net/NetHeader.h"

void Client::run()
{
	init();

	while (m_window.isOpen()) {
		m_loopTimer.tick();

		handleInput();
		update(m_loopTimer.getDeltaTime());
		receive();
		render();
	}
}

void Client::init()
{
	// bind socket used for connection with server
	sf::Socket::Status status;
	status = m_socket.bind(sf::Socket::AnyPort);
	if (status != sf::Socket::Done) {
		std::cout << "Could not bind socket" << std::endl;
	}

	std::cout << "bound to:: " << m_socket.getLocalPort() << std::endl;

	// enter serverip to connect to
	std::cout << "server ip: ";
	//std::string input;
	//std::getline(std::cin, input);
	m_serverIp = "10.0.0.2";

	//register to server
	sf::Uint8 header = static_cast<int>(NetHeader::Register);
	m_packet << header;
	m_socket.send(m_packet, m_serverIp, 9966);
	m_packet.clear();
	//waiting for server to register us
	sf::IpAddress sender;
	unsigned short port;
	std::cout << "waiting for response..." << std::endl;
	status = m_socket.receive(m_packet, sender, port);
	if (status != sf::Socket::Done) {
		std::cout << "Could not register to server" << std::endl;
	}

	m_packet >> header;
	if (static_cast<NetHeader>(header) == NetHeader::Assign) {
		m_packet >> m_myObject;
		std::cout << "Assign to object: " << m_myObject << std::endl;
	}
	m_packet.clear();

	m_window.create(sf::VideoMode(1200, 800), "Window 1337");
	m_window.setFramerateLimit(60.0f);

	m_camera = m_window.getDefaultView();

	m_socket.setBlocking(false);

	m_loopTimer.init();
}

void Client::handleInput()
{
	sf::Event event;
	while (m_window.pollEvent(event)) {
		switch (event.type) {
			case sf::Event::Closed:
				m_window.close();
				break;

			case sf::Event::Resized:
				m_camera.setSize(sf::Vector2f((float)event.size.width, (float)event.size.height));
				m_window.setView(m_camera);
				break;

			case sf::Event::LostFocus:
			case sf::Event::GainedFocus:
			case sf::Event::TextEntered:
			case sf::Event::KeyPressed:
			case sf::Event::KeyReleased:
			case sf::Event::MouseWheelMoved:
			case sf::Event::MouseWheelScrolled:
			case sf::Event::MouseButtonPressed:
			case sf::Event::MouseButtonReleased:
			case sf::Event::MouseMoved:
			case sf::Event::MouseEntered:
			case sf::Event::MouseLeft:
			case sf::Event::JoystickButtonPressed:
			case sf::Event::JoystickButtonReleased:
			case sf::Event::JoystickMoved:
			case sf::Event::JoystickConnected:
			case sf::Event::JoystickDisconnected:
			case sf::Event::TouchBegan:
			case sf::Event::TouchMoved:
			case sf::Event::TouchEnded:
			case sf::Event::SensorChanged:
			case sf::Event::Count:
				break;
		}
	}
}

void Client::update(float deltaTime)
{
	sf::Uint8 movement = 0;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
		movement += 1;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		movement += 2;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		movement += 4;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		movement += 8;
	}

	if (movement != 0) {
		sf::Uint8 header = static_cast<int>(NetHeader::Action);
		sf::Uint8 action = 10;
		m_packet << header << action << movement;
		m_socket.send(m_packet, m_serverIp, 9966);
		m_packet.clear();
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Vector2i windowPosition = sf::Mouse::getPosition(m_window);
		sf::Vector2f mouseDirection = sf::Vector2f(
			windowPosition.x + m_camera.getCenter().x - m_camera.getSize().x / 2, 
			windowPosition.y + m_camera.getCenter().y - m_camera.getSize().y / 2);
		
		sf::Uint8 header = static_cast<int>(NetHeader::Action);
		sf::Uint8 action = 20;
		m_packet << header << action << mouseDirection.x << mouseDirection.y;
		m_socket.send(m_packet, m_serverIp, 9966);
		m_packet.clear();
	}
}

void Client::receive()
{
	sf::IpAddress sender;
	unsigned short port;
	sf::Uint8 header;
	int objectId;
	DrawableObject obj;

	while (m_socket.receive(m_packet, sender, port) == sf::Socket::Done) {
		m_packet >> header;

		switch (static_cast<NetHeader>(header)) {
			case NetHeader::Draw:
				m_packet >> objectId >> obj.x >> obj.y >> obj.width >> obj.data >> 
					obj.color.r >> obj.color.g >> obj.color.b;
				m_drawableObjects[objectId] = obj;
				break;

			case NetHeader::Death:
				m_packet >> objectId;
				m_drawableObjects.erase(objectId);
				break;

			case NetHeader::Register:
			case NetHeader::Action:
			case NetHeader::Assign:
				break;
		}
	}
}

void Client::render()
{
	m_window.clear();
	
	if (m_drawableObjects.find(m_myObject) != m_drawableObjects.end()) {
		sf::Vector2f pos(m_drawableObjects[m_myObject].x, m_drawableObjects[m_myObject].y);

		if (m_camera.getCenter() != pos) {
			m_camera.setCenter(pos);
			m_window.setView(m_camera);
		}
	}

	sf::RectangleShape rect;
	for (std::pair<int, DrawableObject> obj : m_drawableObjects) {
		for (int y = 0; y < obj.second.width; y++) {
			for (int x = 0; x < obj.second.width; x++) {
				if (obj.second.data[(y * obj.second.width) + x] == '0') {
					continue;
				}
				rect.setPosition(obj.second.x + (x * 15), obj.second.y + (y * 15));
				rect.setFillColor(obj.second.color);
				rect.setSize(sf::Vector2f(15.0f, 15.0f));
				m_window.draw(rect);
			}
		}
	}

	m_window.display();
}
