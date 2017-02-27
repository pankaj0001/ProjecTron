#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <array>
#include <vector>
#include <thread>
#include <mutex>
#include <unistd.h>
#define Size_x 184
#define Size_y 105
#define max_players 4

sf::Uint16 player_count, speed;
sf::UdpSocket socket_client;
// std::mutex mutex_client;
sf::IpAddress server_ip;
unsigned short server_port = 55002;
int bound_x = Size_x-1;
int bound_y = Size_y-1;
int my_id;
sf::Uint16 new_position_x[max_players], new_position_y[max_players];
sf::Uint16 old_position_x[max_players], old_position_y[max_players];
sf::Uint16 player_status[max_players], players_direction[max_players], dead[max_players];
sf::Uint16 dir=1, next_x, next_y;





class Tron{
  private:
    std::array<std::array<int, Size_x>, Size_y> board;
    std::vector<sf::Vector2f> TronPartLocations;
    long long int player_score;
    int player_count, speed;
    int my_id;
    std::string player_username, player_ip;

  public:
    Tron(int,int,std::string,int,int,int);
    std::string get_username();
    int getStatus();
    void setStatus(int);
    void increment_Score();
    long long int get_score(); 
    void set_speed(sf::RenderWindow&);
    void init_board();
    void move(sf::Uint16);
    void draw(sf::RenderWindow&);
    void clear_trails();
    void update_board();
    void Remove_player(int);
    void draw_initials();
    bool check_collision();

};

Tron::Tron(int x, int y, std::string username, int id, int p_count, int spd)
{
    player_score = 1;
    player_username = username;
    my_id = id;
    player_status[my_id] = 1;
    player_ip = "172.16.4.5"; //Change it later
    player_count = p_count;
    speed = spd;
}

void Tron::draw_initials()
{   
    for(int i=0;i<player_count;i++)
        board.at(new_position_y[i]).at(new_position_x[i]) = my_id + 1;
}

std::string Tron::get_username()
{
    return player_username;
}

int Tron::getStatus()  
{
    return player_status[my_id];
}

void Tron::setStatus(int curr_status)
{
    player_status[my_id] = curr_status;
}

void Tron::increment_Score()
{
    this->player_score++;
}

long long int Tron::get_score()
{
    return this->player_score;
}

void Tron::set_speed(sf::RenderWindow& targetWindow)
{
    targetWindow.setFramerateLimit(30);
}

void Tron::init_board()
{
    for (int y = 0; y < signed(board.size()); y++)
    {
        for (int x = 0; x < signed(board.at(0).size()); ++x)
        {
            board.at(y).at(x) = 0;
        }
    }
    for (int x = 0; x < signed(board.at(0).size()); ++x)
    {
        board.at(0).at(x) = 5;
    }

    for (int x = 0; x < signed(board.at(0).size()); ++x)
    {
        board.at(bound_y).at(x) = 5;
    }
    for (int x = 0; x < signed(board.size()); ++x)
    {
        board.at(x).at(0) = 5;
    }
    for (int x = 0; x < signed(board.size()); ++x)
    {
        board.at(x).at(bound_x) = 5;
    }
}

void Tron::move(sf::Uint16 Dir)
{
    dir = Dir;
    if      (Dir == 0) { next_y = new_position_y[my_id] - 1; next_x = new_position_x[my_id];}
    else if (Dir == 1) { next_x = new_position_x[my_id] + 1; next_y = new_position_y[my_id];}
    else if (Dir == 2) { next_y = new_position_y[my_id] + 1; next_x = new_position_x[my_id];}
    else if (Dir == 3) { next_x = new_position_x[my_id] - 1; next_y = new_position_y[my_id];}
}
bool Tron::check_collision()
{
    //Check for Collisions - Wall and Player
    if(board.at(new_position_y[my_id]).at(new_position_x[my_id]) != 0)
    {
        this->clear_trails();
        return true;
    }
    return false;
}

void Tron::draw(sf::RenderWindow& targetWindow)
{
    //Go through the board:
    for (int y = 0; y < signed(board.size()); y++)
    {
        for (int x = 0; x < signed(board.at(0).size()); ++x)
        {
            //0 - empty , 1-4 players , 5 = boundary
            if (board.at(y).at(x) != 0)
            {
                //Create a rectangle:
                sf::RectangleShape block;
                block.setSize(sf::Vector2f(10, 10));
                block.setPosition(x * 10, y * 10);

                //Colour the rectangle accordingly:
                if (board.at(y).at(x) == 1)
                {
                    block.setFillColor(sf::Color::Green);
                }
                else if (board.at(y).at(x) == 2)
                {
                    block.setFillColor(sf::Color::Red);
                }
                else if (board.at(y).at(x) == 3)
                {
                    block.setFillColor(sf::Color::Cyan);
                }
                else if (board.at(y).at(x) == 4)
                {
                    block.setFillColor(sf::Color::Blue);
                }
                else
                {
                    block.setFillColor(sf::Color::White);
                }
                targetWindow.draw(block);
            }
        }
    }
}

void Tron::clear_trails()
{
    Remove_player(my_id);
}

void Tron::Remove_player(int id)
{
	player_status[id] = (sf::Uint16)0;
    for (int y = 0; y < signed(board.size()); y++)
    {
        for (int x = 0; x < signed(board.at(0).size()); ++x)
        {
            if(board.at(y).at(x) = id + 1)// || (board.at(y).at(x) = id + 1))
                board.at(y).at(x) = 0;
        }
    }
}

void Tron::update_board()
{
    for(int i=0; i<player_count && player_status[i] == (sf::Uint16)1; i++)
    {
        board.at(new_position_y[i]).at(new_position_x[i]) = i + 1;
		//board.at(old_position_y[i]).at(old_position_x[i]) = i + 1;
    }
	for(int i=0; i<player_count && player_status[i] == (sf::Uint16)0 && dead[i] == (sf::Uint16)0; i++)
    {
        dead[i] = (sf::Uint16)1;
		this->Remove_player(i);
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
	// std::cout<<"printing locations: ";
	for(int i=0;i<player_count;i++)
	{
		std::cout << " " << i << " " << player_status[i] << " " << dir << " " << new_position_x[i] << " " << new_position_y[i] << std::endl;
	}

}

namespace client
{

    void connect_server(sf::IpAddress ip, unsigned short port, sf::String username)
    {
        sf::Packet packet;
        packet << username;
        socket_client.send(packet, ip, port);
        std::string s_username = username;
        //std::cout<<s_username<<std::endl;
    }


    void get_locations()
    {
    	sf::Uint16 temp;
        sf::IpAddress ip;
        sf::Packet packet;
        unsigned short port;
        int id;
        socket_client.receive(packet, ip, port);
        for(int i=0;i<player_count;i++)
        {
            packet >> id;
            // sf::Uint16 temp
            // packet >> temp >> id;
            // std::cout<<temp;
            packet >> player_status[id] >> players_direction[id] >> new_position_x[id] >> new_position_y[id];
        }
        if(new_position_x[my_id] == old_position_x[my_id] && new_position_y[my_id] == old_position_y[my_id])
        {
            get_locations();
        }
        // mutex_client.lock();
        // std::cout << "Recieved locations "<<std::endl;
        // for(int i=0;i<player_count;i++)
        // {
        //    std::cout << i << " " << player_status[id] << " " << players_direction[i] << " " << new_position_x[i] << " " << new_position_y[i] << std::endl;
        // }
        // mutex_client.unlock();
    }

    void recieve_location()
    {
        while(true)
        {
            get_locations();
        }
    }


    sf::Uint16 get_inititals()
    {
        sf::IpAddress ip;
        sf::Packet packet; 
        unsigned short port;
        socket_client.receive(packet, ip, port);
        packet >> my_id >> player_count >> speed;
		
		for(int i=0;i<player_count;i++)
		{
        	player_status[i] = 1;
		}

        std::cout<<my_id<<player_count<<speed<<std::endl;
    }


    void send_my_location(sf::IpAddress ip, unsigned short port, sf::Uint16 dir, sf::Uint16 x, sf::Uint16 y)
    {
        sf::Packet packet;
        packet << my_id << player_status[my_id] << dir << x << y;
        // mutex_client.lock();
        std::cout<<"Send : "<< my_id <<" " << player_status[my_id] <<" "<<dir<<" "<< x << " " <<y<<std::endl;
        // mutex_client.unlock();
        socket_client.send(packet, ip, port);
    }

    void send_location(){
        while(true)
        {
            send_my_location(server_ip, server_port, dir , next_x, next_y);

        }
    }
}



int main()
{

    sf::Uint16 initial_position_x = rand()%Size_x, initial_position_y = rand()%Size_y; //temp
    sf::Texture texture;
    sf::Sprite sprite(texture);

    std::string f_str, server_ip_std;

    sf::Font font;
    font.loadFromFile("arial.ttf");
    // Score Print
    sf::Text score_text;
    score_text.setFont(font);
    score_text.setColor(sf::Color::Red);
    score_text.setPosition(350,450);
    score_text.setCharacterSize(100);
    //Ask Username
    //Send Connect Req To Server
    //Get ID
    //Get All Initial Locations
    //Game Start
    sf::String my_username = "^[P]ROTO[N]^";
    socket_client.bind(sf::Socket::AnyPort);
    server_ip = sf::IpAddress("192.168.43.207");
    client::connect_server(server_ip, server_port, my_username);
    client::get_inititals();
    client::get_locations();
    std::cout<<"Init : "<<std::endl;
    print_location();
    Tron *player2 = new Tron(new_position_x[my_id], new_position_y[my_id], my_username, my_id, player_count, speed);
    Tron player1 = *player2;
    
    // std::thread t1;
  	//  t1 = std::thread(client::recieve_location);
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Game Window");
    window.setVerticalSyncEnabled(true);
    player1.set_speed(window);
    player1.init_board();
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (player1.getStatus() == 1 && event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                    case sf::Keyboard::Up:
                        dir = 0;
                        break;
                    case sf::Keyboard::Right:
                        dir = 1;
                        break;
                    case sf::Keyboard::Down:
                        dir = 2;
                        break;
                    case sf::Keyboard::Left:
                        dir = 3;
                        break;
                    default:
                        break;
                }
            }
        }

        window.clear();
        window.draw(sprite);

        //if player is alive
        if(player1.getStatus() == 1)
        {
        	player1.move(dir);
        	client::send_my_location(server_ip, server_port, dir , next_x, next_y);
            copy_location(old_position_y, new_position_y);
            copy_location(old_position_x, new_position_x);
        	client::get_locations();
            std::cout<<"Recv :";
        	print_location();
            if(player1.check_collision())
            {
                std::cout<<"Coli :";
                print_location();
                player1.setStatus(0);
                f_str = std::to_string(player1.get_score());
                score_text.setString("Your Score is "+f_str);
            }
            else
            {
            	player1.update_board();
                player1.increment_Score();
                player1.draw(window);
            }
        }
        //If Player Died
        else
        {
            window.draw(score_text);
            window.display();
        }
        window.display();
    }
    // t1.join();
    // t2.join();
    return EXIT_SUCCESS;
}
