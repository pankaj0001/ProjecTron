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
    long long int score;
    int x,y;
    status player_status;

  public:
    Tron();
    status getStatus();
    void setStatus(status);
    void increment_Score();
}
