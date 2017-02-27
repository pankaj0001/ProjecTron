// Server Code
#include <SFML/Network.hpp>
#include <iostream>
#include <vector>
#include <thread>
#include <unistd.h>
#include <mutex>
#define max_players 4

#define Size_x 184
#define Size_y 105

//Common Variables.
sf::UdpSocket socket;	
sf::Uint16 new_position_x[max_players], new_position_y[max_players], new_direction[max_players];
sf::Uint16 old_position_x[max_players], old_position_y[max_players], old_direction[max_players];
sf::Uint64 Player_scores[max_players];
sf::Uint16 speed=30, player_count, player_status[max_players];
sf::String usernames[max_players];
std::string s_usernames[max_players];
sf::IpAddress Players_IP[max_players];
bool recieved_position[max_players];
unsigned short Players_PORT[max_players];
std::mutex m;

void accept_connections()
{
	sf::Packet packet;
	sf::IpAddress Client_IP;
	unsigned short Client_Port;
	//std::cout<<"Players Connected : "<<std::endl;
	while(player_count != 2)
	{
		socket.receive(packet, Client_IP, Client_Port);
		packet >> usernames[player_count];
		s_usernames[player_count] = usernames[player_count]; 
		Players_IP[player_count] = Client_IP;
		Players_PORT[player_count] = Client_Port;
		//std::cout <<  player_count << s_usernames[player_count] << " - " << Client_IP << ":" << Client_Port << std::endl;
		player_count++;
	}
}

void copy_location(sf::Uint16 old_location[], sf::Uint16 new_location[])
{
	for(int i=0; i<player_count; i++)
	{
		old_location[i] = new_location[i]; 
	}
}

bool check_input()
{
	int temp;
	std::cout << "Press \"1\" to stop recieving connections" << std::endl;
	std::cin >> temp;
	return(temp == 1);
}

void print_location()
{	
	std::cout<<"printing locations"<<std::endl;
	for(int i=0;i<player_count;i++)
	{
		std::cout << " " << i << " " << player_status[i] << " " << new_direction[i] << " " << new_position_x[i] << " " << new_position_y[i] << std::endl;
	}

}

void broadcast_players()
{
	sf::Packet packet;
	sf::Uint16 temp,temp1,temp2,temp3;
	// /print_location();
	for(int i=0;i<player_count;i++)
	{
		packet << i << player_status[i] << new_direction[i] << new_position_x[i] << new_position_y[i];
	}
	for(int i=0; i<player_count; i++)
	{
		socket.send(packet, Players_IP[i], Players_PORT[i]);
	}

}

void initialize_and_send()
{
	sf::Packet packet;
	for(int i=0; i<player_count; i++)
	{
		packet.clear();
		new_position_x[i] = (sf::Uint16)(rand()%(Size_x/2) + Size_x/4);
		new_position_y[i] = (sf::Uint16)(rand()%(Size_y/2) + Size_y/4);
		new_direction[i] = (rand()%4);
		player_status[i] = 1;
		packet << i << player_count << speed;
		recieved_position[i] = true;
		socket.send(packet, Players_IP[i], Players_PORT[i]);
		//std::cout << "sent_initial_details to " << i << " " << Players_IP[i] << ":" << Players_PORT[i] << std::endl;
	}
}

void recieve_locations()
{
	sf::Packet packet;
	sf::IpAddress ip;
	sf::Uint16 x,y,id,dir,temp,status;
	unsigned short port;
	bool recieved = false;
	while(true)
	{
		packet.clear();
		socket.receive(packet, ip, port);
		packet >> temp;
		packet  >> id >> status >> dir >> x >> y;
		//m.lock();
		//std::cout<<"Recieved : ";
		//std::cout << id <<  " " << status << " " << dir << " " << x << " "<< y << std::endl;
		//m.unlock();
		player_status[id] = status;
		new_position_x[id] = x;
		new_position_y[id] = y;
		recieved_position[id] = true;
	}
}

bool check_recieved_all()
{
	for(int i=0;i<player_count;i++)
	{
		if(!recieved_position[i])
		{	
			switch(new_direction[i])
			{
				case 0:
					new_position_y[i] = old_position_y[i] + 1;
					new_position_x[i] = old_position_x[i];
					break;

				case 1:
					new_position_y[i] = old_position_y[i];
					new_position_x[i] = old_position_x[i] + 1;
					break;

				case 2:
					new_position_y[i] = old_position_y[i] -1;
					new_position_x[i] = old_position_x[i];
					break;

				case 3:
					new_position_y[i] = old_position_y[i];
					new_position_x[i] = old_position_x[i] - 1;
					break;

				case 4:
					new_position_x[i] = -1;
					new_position_y[i] = -1;
					player_status[i] = 0;
					break;
			}
		}
	}
}



void send_location(){
//	for(int temp=0;temp<5;temp++)
	while(true)
	{
		sleep(1);
		check_recieved_all();
		copy_location(old_position_x, new_position_x);
		copy_location(old_position_y, new_position_y);
		copy_location(old_direction, new_direction);
		broadcast_players();
	}
}


int main()
{

	int i;
	sf::IpAddress server_IP = sf::IpAddress::getLocalAddress();
	if(socket.bind(55002) != sf::Socket::Done)
	{
		std::cout << "Error in Port Binding port tried = 55002 " << std::endl;
		return 0;
	}
	std::cout << "Connect to "<< server_IP.toString() << ":55002" << " For Incomming Connections" << std::endl;



	accept_connections();
	initialize_and_send();
	print_location();

	std::thread t,s;
	t = std::thread(recieve_locations);
	s = std::thread(send_location);
	t.join();
	s.join();


}