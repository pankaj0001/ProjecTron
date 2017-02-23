// Server Code
#include <SFML/Network.hpp>
#include <iostream>
#include <vector>
#include <thread>
#define max_players 4
enum direction {up=0, right=1, down=2, left=3};
enum status {dead=0, alive=1};
#define Size_x 184
#define Size_y 105

//Common Variables.
sf::UdpSocket socket;	
sf::Uint16 new_position_x[max_players], new_position_y[max_players], new_direction[max_players];
sf::Uint16 old_position_x[max_players], old_position_y[max_players], old_direction[max_players];
sf::String usernames[max_players];
direction players_direction[max_players];
bool recieved_position[max_players];
sf::IpAddress Players_IP[max_players];
unsigned short Players_PORT[max_players];
int player_count=0;


void accept_connections()
{
	bool input;
	int temp;
	sf::Packet packet;
	sf::IpAddress Client_IP;
	unsigned short Client_Port;
	std::cout<<"Players Connected : ";
	while(player_count != 2)
	{
		socket.receive(packet, Client_IP, Client_Port);
		packet >> usernames[player_count]; 
		Players_IP[player_count] = Client_IP;
		Players_PORT[player_count] = Client_Port;
		std::cout<<player_count<<" - "<<Client_IP<<":"<<Client_Port << endl;
		player_count++;
	}
}

void copy_location(sf::Uint16 [] old_location, sf::Uint16 [] new_location)
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

void broadcast_players()
{
	sf::Packet packet;
	for(i=0;i<player_count;i++)
	{
		packet << i << players_direction[i] << new_position_x[i] << new_position_y[i];
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
		players_direction[i] = (direction)(rand()%4);
		packet << i << players_direction[i] << new_position_x[i] << new_position_y[i];
		socket.send(packet, Players_IP[i], Players_PORT[i]);
		std::cout << "sent_initial_details to " << Players_IP[i] << ":" << Players_PORT[i] << std::endl;
		std::cout << i << " " << players_direction[i] << " " << new_position_x[i] << " " << new_position_y[i] << std::endl;
	}
}

void recieve_locations()
{
	sf::Packet packet;
	sf::IpAddress ip;
	direction dir;
	sf::Uint16 x,y,id;
	int temp;
	unsigned short port;
	bool recieved = false;
	while(true)
	{
		socket.receive(packet, ip, port);
		std::cout<<"Recieved Player Location : ";
		packet >> temp >> id >> dir >> x >> y;
		std::cout << id <<  " " << dir << " " << x << " "<< y << std::endl;
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
			return false;
	}
	return true;	
}

void location_update_by_server()
{
	for(int i=0;i<player_count;i++)
	{
		if(!recieved_position[i])
		{	
			switch(direction)
			{
				case up:
					new_position_y[i] = old_position_y[i] + 1;
					new_position_x[i] = old_position_x[i];
					break;

				case right:
					new_position_y[i] = old_position_y[i];
					new_position_x[i] = old_position_x[i] + 1;
					break;

				case down:
					new_position_y[i] = old_position_y[i] -1;
					new_position_x[i] = old_position_x[i];
					break;

				case left:
					new_position_y[i] = old_position_y[i];
					new_position_x[i] = old_position_x[i] - 1;
					break;
			}
		}
	}
}


int main()
{

	int i,player_count = 0;
	unsigned short Client_Port, temp1, port=55002;
	bool take=true;

	sf::IpAddress Client_IP, server_IP = sf::IpAddress::getLocalAddress();
	sf::Uint16 temp, positionx, positiony, id;
	sf::Time t1 = sf::seconds(0.1f), t2 = sf::seconds(0.f);;
	sf::Clock clock;
	sf::Packet packet_server_recieve, packet_server_initial_recieve, packet_server_send_all, packet_client_initial_send;

	if(socket.bind(55002) != sf::Socket::Done)
	{
		std::cout << "Error in Port Binding port tried = 55002 " << std::endl;
		return 0;
	}
	std::cout << "Connect to "<< server_IP.toString() << ":" << port << " For Incomming Connections" << std::endl;

	accept_connections();
	initialize_and_send();
	
	copy_location(old_position_x, new_position_x, player_count);
	copy_location(old_position_y, new_position_y, player_count);
	copy_location(old_direction, new_direction, player_count);



}

/*
1- C -> S (thread - "s" to stop waiting)
2- S -> all C = their id and initial position and direction 
3- S <- C = update locations / direction (thread - till game over)
4- S -> C = All client Location
repeat 3-4


















	/*
	while(true)
	{
		//Recieve Next Move of all( time wait 100 ms) (blocking Recieve so in thread)
		//sf::Time time1 = clock.getElapsedTime();
		//while(time1 <= t1)
		//sf::Time time1 = clock.getElapsedTime();


		//Thread To Recieve Permanently
		
			socket.receive(packet_server_recieve, Client_IP, Client_Port);
			std::cout<<"Recieve: ";
			packet_server_recieve >> temp >> id >> positionx >> positiony;
			std::cout << id << " " << positionx << " "<< positiony << std::endl;
			new_position_x[id] = positionx;
			new_position_y[id] = positiony;


			sf::Time time1 = clock.getElapsedTime();
			//if(time1 >= t1)
			//{
				for(i=0;i<player_count;i++)
				{
					packet_server_send_all << i << new_position_x[i] << new_position_y[i];
				}
				for(i=0;i<player_count;i++)
				{
					socket.send(packet_server_send_all, Players_IP[i], Players_PORT[i]);
				}
				//sf::Time time2 = clock.restart();

				packet_server_send_all.clear();
				packet_server_recieve.clear();
			//}
	}
	std::cout << endl;
	return 0;
}




















 
 
Temp Not Working Sometimes 



	//Print Server IP address
	server_IP = sf::IpAddress::getLocalAddress();
	cout << "Server Ip Address is "<< server_IP.toString() << endl;



	//Sender = IP of Client who sent data
	socket.receive(packet_server_recieve, Client_IP, Client_Port);
	packet_server_recieve >> new_position_x >> new_position_y >> player_id;
	std::cout << Client_IP.toString() << " said - " << player_id << " " << new_position_y << " " << new_position_x << std::endl;




	//Send Back To Same Client
	socket.send(packet, Client_IP, Client_PORT);
*/

