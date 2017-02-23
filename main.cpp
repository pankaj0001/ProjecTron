#include <SFML/Graphics.hpp>
#include <array>
#include <vector>

enum direction {up=0, right=1, down=2, left=3};
enum status {dead=0, alive=1};

#define Size_x 184
#define Size_y 105
int bound_x = Size_x-1;
int bound_y = Size_y-1;


class Tron{
private:
    std::array<std::array<int, Size_x>, Size_y> board;
    std::vector<sf::Vector2f> TronPartLocations;
    long long int player_score;
    int x,y,Player_id, player_color;
    status player_status;
    std::string player_username, player_ip;
public:
    Tron(int x, int y, std::string username, int id)
    {
        player_score = 1;
        player_status = alive;
        TronPartLocations.push_back(sf::Vector2f(x,y));
        board.at(TronPartLocations.at(0).y).at(TronPartLocations.at(0).x) = 1;
      	player_username = username;
    	Player_id = id;
    	player_ip = "172.16.4.5"; //Change it later
    	player_color = id;
    }

    std::string get_username()
    {
      return player_username;
    }

    status getStatus()
    {
    	return player_status;
    }

    void setStatus(status curr_status)
    {
    	this->player_status = curr_status;
    }

    void increment_Score()
    {
    	this->player_score++;
    }

    long long int get_score()
    {
    	return this->player_score;
    }

    void set_speed(int speed, sf::RenderWindow& targetWindow)
    {
	    targetWindow.setFramerateLimit(speed);    
    }

    // Initialize Arena With Boundary
    void init_board()
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
			board.at(0).at(x) = 3;
		}

		for (int x = 0; x < signed(board.at(0).size()); ++x)
		{
			board.at(bound_y).at(x) = 3;
		}

		for (int x = 0; x < signed(board.size()); ++x)
		{
			board.at(x).at(0) = 3;
		}

		for (int x = 0; x < signed(board.size()); ++x)
		{
			board.at(x).at(bound_x) = 3;
		}
    }

    // Return false if collision else True
    bool move(direction Dir)
    {
    	sf::Vector2f PlayerHead = TronPartLocations.at(TronPartLocations.size()-1);
    	board.at(PlayerHead.y).at(PlayerHead.x) = 1;
        if      (Dir == up)    { PlayerHead.y = PlayerHead.y - 1; }
        else if (Dir == right) { PlayerHead.x = PlayerHead.x + 1; }
        else if (Dir == down)  { PlayerHead.y = PlayerHead.y + 1; }
        else if (Dir == left)  { PlayerHead.x = PlayerHead.x - 1; }

        //Check for Collisions - Wall and Player
        if(board.at(PlayerHead.y).at(PlayerHead.x) != 0)
        {
        	this->clear_trails();
        	return false;
        }
        else
        {
	        TronPartLocations.push_back(PlayerHead);
    		board.at(PlayerHead.y).at(PlayerHead.x) = 2;
        }
        return true;
    }


    //Draw Arena with Players Location
    void draw(sf::RenderWindow& targetWindow)
    {
        //Go through the board:
        for (int y = 0; y < signed(board.size()); y++) 
        {
            for (int x = 0; x < signed(board.at(0).size()); ++x)
            {
                //0-Empty , 1-Players Body , 2-Players Head , 3-Boundary Wall
                if ((board.at(y).at(x) == 1) || (board.at(y).at(x) == 2) || (board.at(y).at(x) == 3))
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
						block.setFillColor(sf::Color::White);
					}
                    targetWindow.draw(block);
                }
            }
        }
    }

	//Clearing Player's Trail From Board & Body Part Data
    void clear_trails()
    {
    	for (int i = 0; i < signed(TronPartLocations.size()); ++i)
    	{
        	board.at(TronPartLocations.at(i).y).at(TronPartLocations.at(i).x) = 0;
    	}
    	TronPartLocations.clear(); 
    }
};




//_____________________MAIN___________________________
int main()
{

    //____________ VARIABLES _____________
    sf::Texture texture;
    std::string f_str;
    direction dir = right;
    bool temp = true;
    sf::Time t1 = sf::seconds(0.1f);
    sf::Text welcome_text,End_Text,score_text;
    sf::Font font;
    sf::Sprite sprite(texture);
    //_____________ VARIABLES ___________


    //Ask To setup Server or Connect to Server or 2 Players in this PC
	//Send Connect Request to Server
	//Get Initial Location and Player_id


	//_______________Temp________________
	int initial_position_x = rand()%Size_x;
	int initial_position_y = rand()%Size_y;
	std::string username = "Pankaj Kukreja";
	int player_id = 0;
	//________________Temp______________

    Tron *player2 = new Tron(initial_position_x, initial_position_y, username, player_id);
    Tron player1 = *player2;
    //Initialize Arena
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Game Window");
    window.setVerticalSyncEnabled(true);
    //Option To change Speed
    player1.set_speed(30,window);

    player1.init_board();

    //______________ TEXT _______________
    //Welcome Text
    font.loadFromFile("arial.ttf");
    welcome_text.setFont(font);
    welcome_text.setString("Welcome To ProjecTron");
    welcome_text.setColor(sf::Color::Green);
    welcome_text.setPosition(600,450);
    welcome_text.setCharacterSize(20);

	// Score Print
    score_text.setFont(font);
    score_text.setColor(sf::Color::Red);
    score_text.setPosition(350,450);
    score_text.setCharacterSize(100);
    //______________ TEXT _____________



    //______________ Window _______________
    //While Window is open
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (player1.getStatus() == alive && event.type == sf::Event::KeyPressed)
            {
            	switch (event.key.code)
            	{
	                case sf::Keyboard::Up:
	                {
	                    dir = (direction)0;
	                    break;
	                }
	                case sf::Keyboard::Right:
	                {
	                    dir = (direction)1;
	                    break;
	                }
	                case sf::Keyboard::Down:
	                {
	                    dir = (direction)2;
	                    break;
	                }
	                case sf::Keyboard::Left:
	                {
	                    dir = (direction)3;
	                    break;
	                }
	                default:
	                {
	                	break;
	                }
            	}
            }
        }

        window.clear();
        window.draw(sprite);

        //if player is alive
       	if(player1.getStatus() == alive)
       		{
       			if(!player1.move(dir))
       			{
       				player1.setStatus(dead);
       				f_str = std::to_string(player1.get_score());
       				score_text.setString("Your Score is "+f_str);
       			}
       			else
       			{
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
    //______________ Window _______________


    return EXIT_SUCCESS;
}

