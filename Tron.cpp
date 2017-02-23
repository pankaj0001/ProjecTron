#include "Tron.h"
using namespace std;
using namespace sf;

Tron::Tron(int x, int y, std::string username, int id)
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

string Tron::get_username()
{
  return player_username;
}
status Tron::getStatus()  {
    return player_status;
}

void Tron::setStatus(status curr_status)
{
  this->player_status = curr_status;
}

void Tron::increment_Score()
{
  this->player_score++;
}

long long int Tron::get_score()
{
  return this->player_score;
}

void Tron::set_speed(int speed, sf::RenderWindow& targetWindow)
{
  targetWindow.setFramerateLimit(speed);
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

bool Tron::move(direction Dir)
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

void Tron::draw(sf::RenderWindow& targetWindow)
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

void Tron::clear_trails()
{
  for (int i = 0; i < signed(TronPartLocations.size()); ++i)
  {
      board.at(TronPartLocations.at(i).y).at(TronPartLocations.at(i).x) = 0;
  }
  TronPartLocations.clear();
}
