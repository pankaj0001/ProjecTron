#include <SFML/Network.hpp>
#include <iostream>
using namespace std;
using namespace sf;
int ID = 0;

int main()
{

	sf::UdpSocket socket;
	socket.bind(55005);
	int input, i;
	sf::IpAddress temp1, sender, server_ip = sf::IpAddress::getLocalAddress();//Change it to cin>>" Enter Serve IP - "<<endl;
	unsigned short port,server_port = 55002;
	sf::Packet server_send_initital, server_send, server_recieve_initital, server_recieve;
	sf::Uint16 temp, positionx, positiony, id, next_position_x = 0, next_position_y = 0;
	sf::Uint16 position_x[4], position_y[4], players_id[4];


 	server_send_initital << (sf::Uint16)1; 	
 	std::cout<<"My Port = "<<socket.getLocalPort()<<std::endl;
	std::cout<<(sf::Socket::Done == socket.send(server_send_initital, sf::IpAddress::getLocalAddress(), 55002))<<std::endl;
	std::cout<<"sent_1"<<std::endl;


	//Blocking Receive
	server_recieve_initital.clear();
	socket.receive(server_recieve_initital, temp1, port);
	std::cout << "Recieved Initial Location : ";
	server_recieve_initital >> temp >> id >> positionx >> positiony;
	ID = (int)id;
	std::cout << id << " " << positionx << " " << positiony << std::endl;
	

	//New Position
	positionx++; 
	positiony++;



	int loopcount=1;
	while(loopcount != 10)
	{
		server_send.clear();
		id =1;
		server_send << id << positionx << positiony;
		socket.send(server_send, server_ip, server_port);
		std::cout << "sent in loop " << loopcount <<": " << id << " " << positionx << " " << positiony << std::endl;

		server_recieve.clear();
		socket.receive(server_recieve, sender, port);
		std::cout <<" Recieved     : ";
		for(i=0;i<2;i++)
		{
			server_recieve >> temp;
			server_recieve >> id;
			server_recieve >> position_x[id] >> position_y[id];		
			std::cout << id << " " << position_x[id] << " " << position_y[id]<<", ";
		}
		std::cout<<std::endl;
		positionx++;
		positiony++;
		loopcount++;

	}
	return 0;
}
	
