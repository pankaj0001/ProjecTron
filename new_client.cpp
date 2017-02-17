#include <SFML/Network.hpp>
#include <iostream>




int main()
{
	sf::UdpSocket socket;
	socket.bind(55001);
	sf::Packet server_send_initital, server_recieve_initital;
	sf::Uint16 id, positionx, positiony, temp = (sf::Uint16)1;
 	server_send_initital << temp;
 	sf::IpAddress temp1;
 	unsigned short port;
 	

 	std::cout<<"MyPort = "<<socket.getLocalPort()<<std::endl;
	std::cout<<(sf::Socket::Done == socket.send(server_send_initital, sf::IpAddress::getLocalAddress(), 55002))<<std::endl;
	std::cout<<"Sent Temp"<<std::endl;
	

	socket.receive(server_recieve_initital, temp1, port);
	std::cout<<"Recieve_1"<<std::endl;
	server_recieve_initital >> temp >>id >> positionx >> positiony;
	std::cout<<id<<" "<<positionx<<" "<<positiony;



	
	return 0;
}