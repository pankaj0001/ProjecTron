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
std::mutex mutex_client;
sf::IpAddress server_ip;
unsigned short server_port = 55002;
int bound_x = Size_x-1;
int bound_y = Size_y-1;
int my_id;
sf::Uint16 new_position_x[max_players], new_position_y[max_players], new_direction[max_players];
sf::Uint16 old_position_x[max_players], old_position_y[max_players], old_direction[max_players];
sf::Uint16 player_status[max_players], players_direction[max_players], dead[max_players];





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
    bool move(sf::Uint16);
    void draw(sf::RenderWindow&);
    void clear_trails();
    void update_board();
    void Remove_player(int);
    void draw_initials();

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
    targetWindow.setFramerateLimit(5);
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

bool Tron::move(sf::Uint16 Dir)
{
    board.at(old_position_y[my_id]).at(old_position_x[my_id]) = my_id + 1;
    if      (Dir == 0) { new_position_y[my_id] = new_position_y[my_id] - 1;}
    else if (Dir == 1) { new_position_x[my_id] = new_position_x[my_id] + 1;}
    else if (Dir == 2) { new_position_y[my_id] = new_position_y[my_id] + 1;}
    else if (Dir == 3) { new_position_x[my_id] = new_position_x[my_id] - 1;}
    else if (Dir == 4) { new_position_x[my_id] = -1; new_position_y[my_id] = -1; player_status[my_id] = 0;}

    //Check for Collisions - Wall and Player
    if(board.at(new_position_y[my_id]).at(new_position_x[my_id]) != 0)
    {
        this->clear_trails();
        return false;
    }
    else
    {
        board.at(new_position_y[my_id]).at(new_position_x[my_id]) = my_id + 1;
    }
    return true;
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
    for(int i=0; i<player_count; i++)
    {
    	if(player_status[i] == (sf::Uint16)1)
    	{
	        board.at(new_position_y[i]).at(new_position_x[i]) = my_id + 1;
	        board.at(old_position_y[i]).at(old_position_x[i]) = my_id + 1;
    	}
    	else
    	{
    		this->Remove_player(i);
    	}
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
        sf::Uint16 id;
        socket_client.receive(packet, ip, port);
        for(int i=0;i<player_count;i++)
        {
            packet >> temp >> id;
            // std::cout<<temp;
            packet >> player_status[id] >> players_direction[id] >> new_position_x[id] >> new_position_y[id];
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
        sf::Uint16 tempa[10];
        unsigned short port;
        socket_client.receive(packet, ip, port);
        packet >> my_id >> player_count >> speed;
		
		for(int i=0;i<player_count;i++)
		{
        	player_status[i] = 1;
		}

        // std::cout<<my_id<<player_count<<speed<<std::endl;
    }


    void send_my_location(sf::IpAddress ip, unsigned short port, sf::Uint16 dir, sf::Uint16 x, sf::Uint16 y)
    {
        sf::Packet packet;
        packet << my_id << player_status[my_id] << dir << x << y;
        // mutex_client.lock();
        // std::cout<<"sent - "<< my_id <<" " << dir<<" "<< x << " " <<y;
        // mutex_client.unlock();
        socket_client.send(packet, ip, port);
    }

    void send_location(){
        while(true)
        {
            sleep(1);
            send_my_location(server_ip, server_port, players_direction[my_id], new_position_x[my_id], new_position_y[my_id]);

        }
    }
}


int main()
{

    sf::Uint16 dir = 1, initial_position_x = rand()%Size_x, initial_position_y = rand()%Size_y; //temp
    sf::Texture texture;
    sf::Sprite sprite(texture);

    std::string f_str, server_ip_std;

    //Ask Username
    //Send Connect Req To Server
    //Get ID
    //Get All Initial Locations
    //Game Start
    sf::String my_username = "^[P]ROTO[N]^";
    server_ip = sf::IpAddress("172.16.0.199");
    socket_client.bind(sf::Socket::AnyPort);
    client::connect_server(server_ip, server_port, my_username);
    client::get_inititals();
    client::get_locations();

    std::thread t1,t2;
    t1 = std::thread(client::recieve_location);
    t2 = std::thread(client::send_location);
    Tron *player2 = new Tron(new_position_x[my_id], new_position_y[my_id], my_username, my_id, player_count, speed);
    Tron player1 = *player2;

    // std::cout<<"hereeee";



    sf::Font font;
    font.loadFromFile("arial.ttf");
    // Score Print
    sf::Text score_text;
    score_text.setFont(font);
    score_text.setColor(sf::Color::Red);
    score_text.setPosition(350,450);
    score_text.setCharacterSize(100);

    sleep(5);

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
                if(!player1.move(dir))
                {
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
    t1.join();
    t2.join();
    return EXIT_SUCCESS;
}
