#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include <unistd.h>
using namespace std;

#define max_players 4
enum direction {up=0, right=1, down=2, left=3} my_direction;
int my_id = 0, player_count = 2; // PlayerCount Sent By Server
sf::String username = "Pankaj";
sf::UdpSocket socket;
sf::Uint16 position_x[max_players], position_y[max_players];


//Change it to cin>>" Enter Serve IP - "<<endl;
sf::IpAddress server_ip = sf::IpAddress::getLocalAddress();
unsigned short server_port = 55002;


void connect_server(sf::IpAddress ip, unsigned short port)
{
	sf::Packet packet;
 	packet << username;
	socket.send(packet, ip, port);
}


void get_locations()
{
	sf::IpAddress ip;
	sf::Packet packet;
	unsigned short port;
	int temp;
	sf::Uint16 id;
	socket.receive(packet, ip, port);
	std::cout << "Recieved locations "<<std::endl;
	for(int i=0;i<player_count;i++)
	{
		packet >> temp >> id;
		packet >> position_x[id] >> position_y[id];
	}
}


void receive_location(){
	while(true){
		get_locations();
	}
}

sf::Uint16 get_initials()
{
	sf::IpAddress ip;
	sf::Packet packet;
	sf::Uint16 id;
	unsigned short port;
	socket.receive(packet, ip, port);
	packet >> id;
	return id;
}


void send_my_location(sf::Uint16 dir, sf::Uint16 x, sf::Uint16 y)
{
	sf::Packet packet;
	packet << my_id << dir << x << y;
	socket.send(packet, server_ip, server_port);
}

void send_location(){
	while(true){
		usleep(50000);
		send_my_location(my_direction, position_x[my_id], position_y[my_id]); //Send In thread
	}
}




int main()
{
	cout<<"Enter Server's IP address" << endl;
	cin >> server_ip;
	socket.bind(sf::Socket::AnyPort);
	thread t,s;
 	connect_server(server_ip, server_port);
	std::cout<<"here"<<std::endl;
 	my_id = get_initials();
	t = thread(receive_location);
 	//get_locations()	// In Thread and till A Player wins

 	//Update My Location
	s = thread(send_location);
	s.join();
	t.join();
	//send_my_location(my_direction, position_x[my_id], position_y[my_id]); //Send In thread

	return 0;
}


/*Old Working Client


#include <SFML/Network.hpp>
#include <iostream>
using namespace std;
using namespace sf;
int ID = 0;
int main()
{

	sf::UdpSocket socket;
	socket.bind(55001);
	int input, i;
	sf::IpAddress temp1, sender, server_ip = sf::IpAddress::getLocalAddress();//Change it to cin>>" Enter Serve IP - "<<endl;
	unsigned short port,server_port = 55002;
	sf::Packet server_send_initital, server_send, server_recieve_initital, server_recieve;
	sf::Uint16 temp, positionx, positiony, id, next_position_x = 0, next_position_y = 0;
	sf::Uint16 position_x[4], position_y[4];


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
		server_send << ID << positionx << positiony;
		socket.send(server_send, server_ip, server_port);
		std::cout << "Sent in loop " << loopcount << ": " << ID << " " << positionx << " " << positiony << std::endl;

		server_recieve.clear();
		socket.receive(server_recieve, sender, port);
		std::cout << "Recieved      : ";
		for(i=0;i<2;i++)
		{
			server_recieve >> temp;
			server_recieve >> id;
			server_recieve >> position_x[id] >> position_y[id];
			std::cout << id << " " << position_x[id] << " " << position_y[id]<<" , ";
		}
		std::cout<<std::endl;
		positiony++;
		positionx++;
		loopcount++;

	}
	return 0;
}



*/
