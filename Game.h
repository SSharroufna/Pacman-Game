#ifndef _GAME_H_
#define _GAME_H_

#include <vector>
#include <string>
#include "Player.h"
#include "Board.h"

class Game
{
public:
	// TODO: implement these functions
	Game(); // constructor

	// initialize a new game, given one human player and
	// a number of enemies to generate
	void NewGame(Player *human, std::vector<Player *> enemylist, const int enemies);

	// Get the names of the possible moves for a player at a given position
	std::vector<std::string> GetMoveNames(Player *p, const std::vector<Position> &positions);

	// Get the position chosen by the player from the given choice and available names and positions
	Position GetPositionFromChoice(const std::string choice, const std::vector<std::string> names, const std::vector<Position> positions);

	// Handle the interaction between a player and an occupant of a square
	bool HandleOccupant(Player *p, SquareType occupant);

	// have the given Player take their turn
	bool TakeTurn(Player *p, const std::string &choice, std::vector<Player *> &enemylist);

	// have the enemy take turn
	bool TakeTurnEnemy(Player *p);

	// Check if game is over.
	bool IsGameOver(Player *p);

	// return true if the dots in game are over
	bool CheckifdotsOver();

	// You probably want to implement these functions as well
	// string info about the game's conditions after it is over
	std::string GenerateReport(Player *p);

	int get_moves() const
	{
		return moves_;
	}

	void IncrementMoves()
	{
		moves_++;
	}

	// To handle attacks between humanPlayer and Enemy.
	void HandlePlayerAttacksEnemy(Player *player, Player *enemy);
	void HandleEnemyAttacksPlayer(Player *player, Player *enemy);

	// Generates a random position on the board.
	Position GenerateRandomPosition();
	friend std::ostream &operator<<(std::ostream &os, const Game &m);

private:
	Board *board_;
	std::vector<Player *> players_;
	int turn_count_;
	int dots_count_;
	bool game_over_;
	int moves_;

	// you may add more fields, as needed
	Board board;
	Player player;
	// To store the player move choice
	std::string choice_;
	// To store enemy list
	std::vector<Player *> enemyList_;

}; // class Game

#endif // _GAME_H_
