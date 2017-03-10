/*
POPL-II Project-I
Multiplayer Game - ProjecTron
Group - 12
*/

// Server Code
#include <SFML/Network.hpp>
#include <iostream>
#include <vector>
#include <thread>
#include <unistd.h>
#include <mutex>
#include <ctime>
#include <condition_variable>
#define max_players 4
#define Size_x 120
#define Size_y 70

//Common Variables.
sf::UdpSocket socket;
sf::Uint16 new_position_x[max_players], new_position_y[max_players], new_direction[max_players];
sf::Uint16 old_position_x[max_players], old_position_y[max_players], old_direction[max_players];
sf::Uint16 speed = 30, player_count, player_status[max_players];
sf::String usernames[max_players];
std::string s_usernames[max_players];
sf::IpAddress Players_IP[max_players];
unsigned short Players_PORT[max_players];
int deadth_count = 0;
// bool recieved_position[max_players];
int alive_count;
bool ready = false;
// std::unique_lock<std::mutex> lk(m);
std::mutex m_print, m_location;
std::condition_variable cv;
bool modified = false;



//Functions
void accept_connections()
{
	sf::Packet packet;
	sf::IpAddress Client_IP;
	int count;
	unsigned short Client_Port;
	std::cout << "Players Connected:" << std::endl;
	std::cout << "Enter Player Count ? ->  ";
	std::cin >> count;
	while(count > max_players)
	{
		std::cout << "Maximum Players Allowed is \"" << max_players << "\" \nEnter Player count Again -> ";
		std::cin >> count;
	}
	while(player_count < count)
	{
		socket.receive(packet, Client_IP, Client_Port);
		packet >> usernames[player_count];
		s_usernames[player_count] = usernames[player_count]; 
		Players_IP[player_count] = Client_IP;
		Players_PORT[player_count] = Client_Port;
		std::cout <<  player_count << " " << s_usernames[player_count] << "   -   " << Client_IP << ":" << Client_Port << std::endl;
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

void print_location()
{
	m_print.lock();
	for(int i=0;i<player_count;i++)
	{
		std::cout << i << " " << player_status[i] << " " << new_direction[i] << " " << new_position_x[i] << " " << new_position_y[i] << std::endl;
	}
	m_print.unlock();
}


void initialize_and_send()
{
	sf::Packet packet;
	srand(time(NULL));
	for(int i=0; i<player_count; i++)
	{
		packet.clear();
		new_position_x[i] = (sf::Uint16)(rand()%(Size_x/2) + Size_x/4);
		new_position_y[i] = (sf::Uint16)(rand()%(Size_y/2) + Size_y/4);
		new_direction[i] = (rand()%4);
		player_status[i] = 1;
		packet << i << player_count << speed;
		socket.send(packet, Players_IP[i], Players_PORT[i]);
	}
}

void broadcast_players()
{
	sf::Packet packet;
	std::cout<<"Send : ";
	m_print.lock();

	for(int i=0;i<player_count;i++)
	{
		packet << i << player_status[i] << new_direction[i] << new_position_x[i] << new_position_y[i];
		std::cout << i << " "<< player_status[i] << " "<< new_direction[i]<< " " << new_position_x[i] << " "<< new_position_y[i]<<"    ";

	}

	alive_count = player_count;
	std::cout<<std::endl;
	m_print.unlock();
	for(int i=0; i<player_count; i++)
	{
		socket.send(packet, Players_IP[i], Players_PORT[i]);
		if(player_status[i] == (sf::Uint16)0)
			alive_count--;
	}
}

void recieve_locations()
{
	while(alive_count != 1)
	{
		sf::Packet packet;
		sf::IpAddress ip;
		unsigned short port;
		int id, ignored;
		sf::Uint16 x,y,dir,status;
		
		packet.clear();

		
		socket.receive(packet, ip, port);
		packet  >> id >> status >> dir >> x >> y;
		m_print.lock();
		std::cout<<"Recv : "<< id <<  " " << status << " " << dir << " " << x << " "<< y << std::endl;
		m_print.unlock();

		m_location.lock();
		player_status[id]  = status;

		bool temp1 = new_position_x[id] != x;
		bool temp2 = new_position_y[id] != y;
		//if got updated value then accept
		if((temp1 && !temp2) || (!temp1 && temp2))
		{
			new_position_x[id] = x;
			new_position_y[id] = y;
			new_direction[id]  = dir;
		}
		else //reject if olld value
		{
			ignored = 1;
		}

		modified = true;
		for(int i=0;i<player_count; i++)
		{
			if(player_status[i] == (sf::Uint16)1 && new_position_x[i] == old_position_x[i] && new_position_y[i] == old_position_y[i])
			{	
				modified = false;
				break;
			}
		}
		m_location.unlock();
	}
}



// bool check_recieved_all()
// {
// 	bool temp = false;
// 	for(int i=0;i<player_count; i++)
// 	{
// 		if(player_status[i] == (sf::Uint16)1)
// 			temp = temp || !recieved_position[i];
// 	}
// 	std::cout<<temp<<std::endl;
// 	return temp;
// }



void send_locations()
{
	while(alive_count != 1)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		m_location.lock();

		if(!modified)
		{
			for(int i=0;i<player_count;i++)
			{
				if(new_position_x[i] == old_position_x[i] && new_position_y[i] == old_position_y[i])
				{
					std::cout<<"modified"<<std::endl;	
					switch(new_direction[i])
					{
						case 0:
							new_position_y[i] = new_position_y[i] - 1;
							new_position_x[i] = new_position_x[i];
							break;

						case 1:
							new_position_y[i] = new_position_y[i];
							new_position_x[i] = new_position_x[i] + 1;
							break;

						case 2:
							new_position_y[i] = new_position_y[i] + 1;
							new_position_x[i] = new_position_x[i];
							break;

						case 3:
							new_position_y[i] = new_position_y[i];
							new_position_x[i] = new_position_x[i] - 1;
							break;
					}
				}
			}
		}
		broadcast_players();
		copy_location(old_position_x, new_position_x);
		copy_location(old_position_y, new_position_y);
		copy_location(old_direction, new_direction);
		modified = false;
		m_location.unlock();
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
	
	copy_location(old_position_x, new_position_x);
	copy_location(old_position_y, new_position_y);
	copy_location(old_direction, new_direction);
    
    std::cout<<"Init : "<<std::endl;
	print_location();
	broadcast_players();

	sleep(1); // start in 3/2/1 ....


	std::thread t_send, t_recieve;
	
	t_send = std::thread(send_locations);
	t_recieve = std::thread(recieve_locations);

	t_send.join();
	t_recieve.join();



	// while(deadth_count < player_count - 1)
	// {
	// 	while(check_recieved_all() && (deadth_count != player_count - 1))
	// 	{
	// 		recieve_locations();
	// 	}
	// 	broadcast_players();
	// }
	// std::cout<<"Here"<<std::endl;


	for(int i=0;i<player_count ; i++)
	{
		if(player_status[i]==(sf::Uint16)1)
			std::cout<<"User = "<<s_usernames[i]<<" Won"<<std::endl;
	}
	print_location();
	return 0;
}