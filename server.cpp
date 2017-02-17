//Server Code
#include <SFML/Network.hpp>
#include <iostream>
#include <vector>

using namespace std;
int main()
{
	cout << "Starting the server" << endl;
	int i,player_count = 0;
	sf::UdpSocket socket;
	sf::IpAddress server_IP, Client_IP, Players_IP[4];
	unsigned short Client_Port,Players_PORT[4],port=55002, temp1;
	bool take=true;

	if(socket.bind(55002) != sf::Socket::Done)
	{
		std::cout << "Error in Port Binding port tried = "<< port << " " << std::endl;
		return 0;
	}
	std::cout << "Using Port = "<< port << " For Incomming Connections" << std::endl;


	//Defining Packets
	sf::Uint16 temp, positionx, positiony, position_x[4], position_y[4], id;
	sf::Packet packet_server_recieve, packet_server_send_all, packet_client_initial_recieve, packet_client_initial_send;


	//Waititng For All Players To Join
	//Client: Hi Server! I am a player
	while(take && player_count != 2)
	{
		//cout<<" Waiting For Connections \n Press s to stop waiting \n -> ";
		//Thread Here which check if input was there

		//blocking Recieve so in thread


		socket.receive(packet_client_initial_recieve, Client_IP, Client_Port);
		cout<<"Recieve_connection";

		packet_client_initial_recieve>>temp;
		if(temp == 1)
		{
			Players_IP[player_count] = Client_IP;
			Players_PORT[player_count] = Client_Port;
			cout<<" Connected: "<<player_count<<" "<<Client_IP<<":"<<Client_Port << endl;
			player_count++;
		}
	}


	//Sending Each User Their ID and Initial Location
	//id is used as direction too
	for(i=0;i<player_count;i++)
	{
		position_x[i] = (sf::Uint16)(rand()%184);
		position_y[i] = (sf::Uint16)(rand()%105);
		packet_client_initial_send.clear();
		packet_client_initial_send << i << position_x[i] << position_y[i];
		cout<<socket.send(packet_client_initial_send, Players_IP[i], Players_PORT[i]);
		cout<<"sent_initial_details to "<<Players_IP[i]<<":"<<Players_PORT[i]<<endl;
		packet_client_initial_send >> i >> position_x[i] >> position_y[i];
		cout<<i<<" "<<position_x[i]<<" "<<position_y[i]<<endl;
		// packet_client_initial_send clear it before next Loop
		//-------> wait for send complete or time >= wait_limit
	}


	//sf::Time t1 = sf::seconds(0.1f);
	//sf::Clock clock;

	while(true)
	{
		//Recieve Next Move of all( time wait 100 ms) (blocking Recieve so in thread)
		//sf::Time time1 = clock.getElapsedTime();
		//while(time1 <= t1)
		//{
		//	sf::Time time1 = clock.getElapsedTime();



			socket.receive(packet_server_recieve, Client_IP, Client_Port);
			cout<<"Recieve: ";
			packet_server_recieve >> temp >> id >> positionx >> positiony;
			std::cout << id << " " << positionx << " "<< positiony << std::endl;
			position_x[id] = positionx;
			position_y[id] = positiony;
		//}

			for(i=0;i<player_count;i++)
			{
				packet_server_send_all << i << position_x[i] << position_y[i];
			}
		
		//Send Next Move to all

			//check if sent
			//sf::IpAddress::Broadcast To BroadCast to all Ips
			for(i=0;i<player_count;i++)
			{
				socket.send(packet_server_send_all, Players_IP[i], Players_PORT[i]);
			}
			packet_server_send_all.clear();
			packet_server_recieve.clear();
//			packet_server_recieve.clear();
//		packet_server_send_all.clear(); //packet_server_send_all clear this packet

	//sf::Time time2 = clock.restart();
	}
	cout << endl;
	return 0;
}




















 
/** 
Temp Not Working Sometimes 



	//Print Server IP address
	server_IP = sf::IpAddress::getLocalAddress();
	cout << "Server Ip Address is "<< server_IP.toString() << endl;



	//Sender = IP of Client who sent data
	socket.receive(packet_server_recieve, Client_IP, Client_Port);
	packet_server_recieve >> position_x >> position_y >> player_id;
	std::cout << Client_IP.toString() << " said - " << player_id << " " << position_y << " " << position_x << std::endl;




	//Send Back To Same Client
	socket.send(packet, Client_IP, Client_PORT);
*/

