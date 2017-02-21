/*

new game
high score
options
quit


->


New_Game_menu
Create Game
Join Game

Options_menu
See Controls
Set Speed

Are You Sure(Y/N)

Score: Player1:    Player2:    Player3:   Player4:


g++ -c Window.cpp
g++ Window.o -o Window.out -lsfml-graphics -lsfml-window -lsfml-system


*/


#include <SFML/Graphics.hpp>
#include <array>
#include <vector>

enum direction {up=0, right=1, down=2, left=3};
enum status {dead=0, alive=1};

#define Size_x 184
#define Size_y 105
int bound_x = Size_x-1;
int bound_y = Size_y-1;

int main()
{
    //____________ VARIABLES _____________
    sf::Texture texture;
    sf::Text welcome_text,End_Text,score_text;
    sf::Font font;
    sf::Sprite sprite(texture);
    font.loadFromFile("arial.ttf");
    //_____________ VARIABLES ___________



    //Initialize Arena
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Game Window");
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(30);
    //______________ TEXT _______________
    //Welcome Text
    welcome_text.setFont(font);
    welcome_text.setString("Welcome To ProjecTron");
    welcome_text.setColor(sf::Color::Green);
    welcome_text.setPosition(600,450);
    welcome_text.setCharacterSize(20);
	//End Text
    End_Text.setFont(font);
    End_Text.setString("You Died");
    End_Text.setColor(sf::Color::Green);    
    End_Text.setPosition(300,450);
    End_Text.setCharacterSize(100);
/*	// Score Print
    score_text.setFont(font);
    score_text.setString("Yoou Died");
    score_text.setColor(sf::Color::Red);
    score_text.setPosition(350,450);
    score_text.setCharacterSize(100);
    // New_Game_Option
    new_game_text.setFont(font);
    new_game_text.setColor(sf::Color::Green);
    new_game_text.setPosition(140,250);
    new_game_text.setCharacterSize(20);
    new_game_text.setString("New Game");
    // Settings_Option
    settings_text.setFont(font);
    settings_text.setColor(sf::Color::Green);
    settings_text.setPosition(170,250);
    settings_text.setCharacterSize(20);
    settings_text.setString("Settigns");
    // Quit_text
    quit_text.setFont(font);
    quit_text.setColor(sf::Color::Green);
    quit_text.setPosition(200,250);
    quit_text.setCharacterSize(20);
    quit_text.setString("Quit");
    // Set_Speed
    set_speed_text.setFont(font);
    set_speed_text.setColor(sf::Color::Green);
    set_speed_text.setPosition(200,250);
    set_speed_text.setCharacterSize(20);
    set_speed_text.setString("New Game");
    // Check Controls
    see_keys_text.setFont(font);
    see_keys_text.setColor(sf::Color::Green);
    see_keys_text.setPosition(200,250);
    see_keys_text.setCharacterSize(20);
    see_keys_text.setString("Check Controls");
    // Connect Server Option
    connect_server_text.setFont(font);
    connect_server_text.setColor(sf::Color::Green);
    connect_server_text.setPosition(170,250);
    connect_server_text.setCharacterSize(20);
    connect_server_text.setString("Connect Server");
    // Create Server
    create_server_text.setFont(font);
    create_server_text.setColor(sf::Color::Green);
    create_server_text.setPosition(200,250);
    create_server_text.setCharacterSize(20);
    create_server_text.setString("Create Server");
    //Enter Ip of Server
    create_server_text.setFont(font);
    create_server_text.setColor(sf::Color::Green);
    create_server_text.setPosition(200,250);
    create_server_text.setCharacterSize(20);
    create_server_text.setString("Enter Server's Ip");
*/    //______________ TEXT _____________



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
        }

        window.clear();
        window.draw(sprite);
    	window.draw(End_Text);
    	*-window.draw(welcome_text);
	    window.display();
    }
    //______________ Window _______________



    return EXIT_SUCCESS;
}

