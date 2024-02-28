#ifndef _PLAYER_H_
#define _PLAYER_H_
#include <string>
#include <sstream>
#include <iostream>

enum class SquareType
{
	Wall,
	Pacman,
	Treasure,
	Enemies,
	PowerfulPacman,
	Pellet,
	White
};

struct Position
{
	int row;
	int col;
	// already implemented for you!
	bool operator==(const Position &other) const
	{
		return row == other.row && col == other.col;
	}
};

class Player
{
public:
	// TODO: implement
	Player(const std::string name, const bool is_human); // constructor

	// These are already implemented for you
	std::string get_name() const { return name_; }			// inline member function
	int get_points() const { return points_; }				// inline member function
	Position get_position() const { return pos_; }			// inline member function
	bool get_is_human() const { return is_human_; }			// inline member function
	bool get_has_treasure() const { return has_Treasure_; } // inline member function
	int get_lives() const { return lives_; }
	bool get_is_dead() const { return is_dead_; } // inline member function
	// getter I've added.
	SquareType get_Pre_Square_Value() { return preSquareValue_; }

	// TODO: implement the following functions
	// You MUST implement the following functions
	void ChangePoints(const int x);

	// set a new position for this player
	void SetPosition(Position pos);
	void SetHasTreasure();
	void SetIsDead(bool isdead);
	void SetLives(int lives);

	// To stores the squreType of the new position,
	// before moving the player into it.
	void SetPreSquareValue(SquareType value)
	{
		preSquareValue_ = value;
	}

	void DecreaseLife(Player *p);

	// You may want to implement these functions as well
	// ToRelativePosition is a function we used to translate positions
	// into direction s relative to the player (up, down, etc)
	std::string ToRelativePosition(Position other);

	// Convert this player to a string representation of their name and points
	std::string Stringify()
	{
		std::stringstream ss;
		ss << "<<<< Game Report >>>>" << std::endl;
		ss << "Player name: " << name_ << ", Points earned: " << points_;
		return ss.str();
	}

	// You may add other functions as needed/wanted

private:
	std::string name_;
	int points_;
	Position pos_;
	bool is_human_;
	bool has_Treasure_;
	bool is_dead_;
	int lives_;
	// You may add other fields as needed
	SquareType preSquareValue_; // I have added this field

}; // class Player

#endif // _PLAYER_H_
