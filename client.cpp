#include <SFML/Network.hpp>
#include <iostream>
using namespace std;
using namespace sf;

int main()
{

	sf::UdpSocket socket;
	socket.bind(55001);
	string server_ip = "172.16.4.28";
	unsigned short server_port = 55002;

	
	
	//Stor in packet
	sf::Packet packet_server_recieve;

	//Send To Server
	socket.send(packet, server_ip, server_port);


		
	//Recieve From Server
	char buffer[1024];
	std::size_t received = 0;
	sf::IpAddress sender;
	unsigned short port;
	socket.receive(packet, sender, port);
	sf::Uint16 temp;
	packet>>temp;
	std::cout << sender.toString() << " said: " << temp << std::endl;
	return 0;


}
	
