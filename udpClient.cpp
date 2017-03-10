#include <SFML/Network.hpp>
#include <iostream>

using namespace std;
const unsigned short PORT = 50001;



int main()
{
// ----- The client -----
// Create a socket and bind it to the port 55001
sf::UdpSocket socket;
socket.bind(55001);
// Send a message to 192.168.1.50 on port 55002
std::string message = "Hi, I am " + sf::IpAddress::getLocalAddress().toString();
socket.send(message.c_str(), message.size() + 1, "172.16.4.28", 55002);
// Receive an answer (most likely from 192.168.1.50, but could be anyone else)
char buffer[1024];
std::size_t received = 0;
sf::IpAddress sender;
unsigned short port;
socket.receive(buffer, sizeof(buffer), received, sender, port);
std::cout << sender.toString() << " said: " << buffer << std::endl;
  	cout << endl;
  return 0;
}