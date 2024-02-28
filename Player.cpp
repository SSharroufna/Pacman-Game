#include "Player.h"
#include "Game.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

Player::Player(const std::string name, const bool is_human)
    : name_(name), points_(0), pos_{0, 0}, is_human_(is_human), has_Treasure_(false), is_dead_(false), lives_(3){};

void Player::ChangePoints(const int x)
{
    points_ += x;
}

// set a new position for this player
void Player::SetPosition(Position pos)
{
    pos_ = pos;
}

void Player::SetHasTreasure()
{
    has_Treasure_ = !(has_Treasure_);
}

void Player::SetIsDead(bool isdead) { is_dead_ = isdead; }

void Player::SetLives(int lives)
{
    lives_ = lives;
}

// Decreases a life from the player
void Player::DecreaseLife(Player *p)
{

    int lives = p->get_lives() - 1;
    p->SetLives(lives);

    if (lives == 0)
    {
        p->SetIsDead(true);
    }
}