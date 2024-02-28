#ifndef _BOARD_H_
#define _BOARD_H_

#include "Player.h"
#include <iostream>
#include <string>

// TODO: implement
std::string SquareTypeStringify(SquareType sq);

class Board
{
public:
    // TODO: implement
    Board();

    // already implemented in line
    int get_rows() const { return 10; } // you should be able to change the size of your
    int get_cols() const { return 10; } // board by changing these numbers and the numbers in the arr_ field

    // TODO: you MUST implement the following functions
    SquareType get_square_value(Position pos) const;
    // set the value of a square to the given SquareType
    void SetSquareValue(Position pos, SquareType value);

    // get the possible Positions that a Player could move to
    // (not off the board or into a wall)
    std::vector<Position> GetMoves(Player *p);

    // get the possible Positions that a Enemy could move to
    // (not off the board or into a wall)
    std::vector<Position> GetEnemyMoves(Player *p);

    // Move a player to a new position on the board. Return
    // true if they moved successfully, false otherwise.
    bool MovePlayer(Player *p, Position pos, std::vector<Player *> enemylist);

    // Move a player to a new position on the board. Return
    // true if they moved successfully, false otherwise.
    bool MoveEnemy(Player *p, Position pos);

    friend std::ostream &
    operator<<(std::ostream &os, const Board &b);

private:
    SquareType arr_[10][10];
    int rows_ = 10; // might be convenient but not necessary
    int cols_ = 10;
    // you may add more fields, as needed
}; // class Board

#endif // _BOARD_H_
