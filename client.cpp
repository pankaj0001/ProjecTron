#include <SFML/Network.hpp>
#include <iostream>
using namespace std;
using namespace sf;

int main()
{

	sf::UdpSocket socket;
	socket.bind(55001);
	sf::IpAddress server_ip = sf::getRemoteAddress();//Change it to cin>>" Enter Serve IP - "<<endl;
	unsigned short server_port = 55002;
	sf::Packet server_send_initital, server_send, server_recieve_initital, server_recieve;
	sf::Uint16 temp, positionx, positiony, id, next_position_x = 0, next_position_y = 0;
	
	//Stor in packet
	std::cout << "Do you want to Connect to server 1=Yes, ^1 =No ? -> ";
	cin>>temp;
 	server_send_initital<<temp
	socket.send(server_send_initital, server_ip, server_port);


	socket.receive(server_recieve_initital, sender, port);
	server_recieve_initital>>id>>positionx>>positiony;
	next_position_x = positionx++;
	next_position_y = positiony++;
	// " Initial Positions are positionx and positiony and myuserid = id "

	while(true)
	{
		server_send << id << positionx << positiony;
		socket.send(server_send, server_ip, server_port);
		server_sende.clear();
		socket.receive(server_recieve, sender, port);
		server_recieve >> id >> positionx >> positiony;
		next_position_x = positionx++;
		next_position_y = positiony++;

	}
	return 0;


}
	
