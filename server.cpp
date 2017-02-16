//Server Code
#include <SFML/Network.hpp>
#include <iostream>
#include <vector>

using namespace std;
int main()
{
	cout << "Starting the server" << endl;
	int , player_count = 0;
	sf::IpAddress server_IP, Client_IP, Players_IP[4];
	unsigned short Client_Port,Players_PORT[4];
	bool take=true;


	//Defining Packets
	sf::Uint16 temp, positionx, positiony, position_x[4], position_y[4], id;
	sf::Packet packet_server_recieve, packet_server_send_all, packet_client_initial_recieve, packet_client_initial_send;


	//Waititng For All Players To Join
	//Client: Hi Server! I am a player
	while(take && player_count != 4)
	{
		cout<<" Waiting For Connections \n Press s to stop waiting \n -> ";
		//Thread Here which check if input was there

		//blocking Recieve so in thread
		socket.receive(packet_client_initial_recieve, Client_IP, Client_Port);
		packet_client_initial_recieve>>temp;
		if(temp == 1)
		{
			Players_IP[player_count] = Client_IP;
			Players_PORT[player_count++] = Client_Port;
			cout<<" Connected: "<<player_count<<endl;
		}
	}


	//Sending Each User Their ID and Initial Location
	//id is used as direction too
	for(i=0;i<player_count;i++)
	{
		position_x[i] = (sf::Uint16)(rand()%184);
		position_y[i] = (sf::Uint16)(rand()%105);
		packet_client_initial_send << id << position_x[i] << position_y[i];

		//-------> wait for send complete or time >= wait_limit


		if(socket.send(packet_client_initial_send, Players_IP[i], Players_PORT[i]))
		{
			//-------> Connection Lost To Players_IP[i] so clear this ip from list
			//player_count--;
			//i++;
		}
		//------------> packet_client_initial_send clear it before next Loop
	}

	while(true)
	{
		//Recieve Next Move of all( time wait 100 ms) (blocking Recieve so in thread)
		while(time <= 100ms) //     <-------------------
		{
			socket.receive(packet_server_recieve, Client_IP, Client_Port)
			packet_server_recieve>> id >> positionx >> positiony;
			position_x[id] = positionx;
			position_y[id] = positiony;
		}

		for(i=0;i<player_count;i++)
		{
			packet_server_send_all << i << position_x[i] << position_y[i];
		}
		
		//Send Next Move to all
		for(i=0;i<player_count;i++)
		{
			//check if sent
			if(socket.send(packet_server_send_all, Players_IP[i], Players_PORT[i]))
			{

			}
		}

		//--------> packet_server_send_all clear this packet

	}
	cout << endl;
	return 0;
}

/**



	//Print Server IP address
	server_IP = sf::IpAddress::getLocalAddress();
	cout << "Server Ip Address is "<< server_IP.toString() << endl;

	sf::UdpSocket socket;
	socket.bind(55002);


	//Sender = IP of Client who sent data
	socket.receive(packet_server_recieve, Client_IP, Client_Port);
	packet_server_recieve >> position_x >> position_y >> player_id;
	std::cout << Client_IP.toString() << " said - " << player_id << " " << position_y << " " << position_x << std::endl;




	//Send Back To Same Client
	socket.send(packet, Client_IP, Client_PORT);
*/

