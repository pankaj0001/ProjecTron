#ifndef TRON_H
#define TRON_H

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
    Tron(int,int,string,int);
    string get_username();
    status getStatus();
    void setStatus(status);
    void increment_Score();
    long long int get_score();
    void set_speed(int,sf::RenderWindow&);
    void init_board();
    bool move(direction);
    void draw(sf::RenderWindow&);
    void clear_trails();
};
