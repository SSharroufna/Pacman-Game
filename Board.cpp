#include "Board.h"
#include <iostream>

////////// Enum ////////
std::string SquareTypeStringify(SquareType sq)
{
    switch (sq)
    {
    case SquareType::Wall:
        return "🧱";
    case SquareType::Pacman:
        return "😃";
    case SquareType::Enemies:
        return "👻";
    case SquareType::PowerfulPacman:
        return "😇";
    case SquareType::Pellet:
        return "🟨";
    case SquareType::White:
        return "⬜️";
    case SquareType::Treasure:
        return "🎁";
    default:
        return "Unknown";
    }
}

////////////// Board  /////////////
Board::Board()
{
    // Static layout for the board
    SquareType layout[10][10] = {
        {SquareType::Pacman, SquareType::Pellet, SquareType::Pellet, SquareType::Pellet, SquareType::Pellet, SquareType::Wall, SquareType::Wall, SquareType::Wall, SquareType::Wall, SquareType::Pellet},
        {SquareType::Pellet, SquareType::Pellet, SquareType::Wall, SquareType::Wall, SquareType::Pellet, SquareType::Wall, SquareType::Pellet, SquareType::Pellet, SquareType::Wall, SquareType::Pellet},
        {SquareType::Pellet, SquareType::Pellet, SquareType::Wall, SquareType::Wall, SquareType::Pellet, SquareType::Wall, SquareType::Pellet, SquareType::Pellet, SquareType::Wall, SquareType::Pellet},
        {SquareType::Pellet, SquareType::Pellet, SquareType::Pellet, SquareType::Treasure, SquareType::Pellet, SquareType::Wall, SquareType::Wall, SquareType::Pellet, SquareType::Wall, SquareType::Pellet},
        {SquareType::Pellet, SquareType::Wall, SquareType::Wall, SquareType::Pellet, SquareType::Pellet, SquareType::Pellet, SquareType::Pellet, SquareType::Pellet, SquareType::Pellet, SquareType::Pellet},
        {SquareType::Pellet, SquareType::Wall, SquareType::Wall, SquareType::Wall, SquareType::Wall, SquareType::Pellet, SquareType::Pellet, SquareType::Pellet, SquareType::Pellet, SquareType::Pellet},
        {SquareType::Pellet, SquareType::Pellet, SquareType::Pellet, SquareType::Pellet, SquareType::Pellet, SquareType::Pellet, SquareType::Pellet, SquareType::Pellet, SquareType::Pellet, SquareType::Pellet},
        {SquareType::Pellet, SquareType::Pellet, SquareType::Pellet, SquareType::Pellet, SquareType::Pellet, SquareType::Wall, SquareType::Wall, SquareType::Pellet, SquareType::Treasure, SquareType::Pellet},
        {SquareType::Pellet, SquareType::Pellet, SquareType::Pellet, SquareType::Pellet, SquareType::Pellet, SquareType::Wall, SquareType::Wall, SquareType::Wall, SquareType::Wall, SquareType::Wall},
        {SquareType::Pellet, SquareType::Pellet, SquareType::Pellet, SquareType::Pellet, SquareType::Pellet, SquareType::Wall, SquareType::Wall, SquareType::Wall, SquareType::Wall, SquareType::Wall}};

    // Print the board
    for (int row = 0; row < 10; ++row)
    {
        for (int col = 0; col < 10; ++col)
        {
            arr_[row][col] = layout[row][col];
        }
    }
};

// Get and Set square_value
SquareType Board::get_square_value(Position pos) const
{
    return arr_[pos.row][pos.col];
};
void Board::SetSquareValue(Position pos, SquareType value)
{
    this->arr_[pos.row][pos.col] = value;
};

// Move a player to a new position on the board. Return
// true if they moved successfully, false otherwise.
bool Board::MovePlayer(Player *p, Position pos, std::vector<Player *> enemylist)
{
    if (pos.row < 0 || pos.row >= 10 || pos.col < 0 || pos.col >= 10)
    {
        std::cout << "This move is out of bounds." << std::endl;
        return false;
    }

    if (arr_[pos.row][pos.col] == SquareType::Wall)
    {
        std::cout << "There is a wall. The player can't move there." << std::endl;
        return false; // New position is a wall, cannot move there
    }

    Position currPosition = p->get_position();

    // Set the square value at the current position to White
    SetSquareValue(currPosition, SquareType::White);
    p->SetPreSquareValue(SquareType::White);

    // Set the square value at the new position to Pacman
    if (p->get_has_treasure())
    {
        SetSquareValue(pos, SquareType::PowerfulPacman);
    }
    else
    {
        SetSquareValue(pos, SquareType::Pacman);
    }

    // Update the player's position
    p->SetPosition(pos);

    return true;
};

// Move a player to a new position on the board. Return
// true if they moved successfully, false otherwise.
bool Board::MoveEnemy(Player *p, Position pos)
{

    if (pos.row < 0 || pos.row >= 10 || pos.col < 0 || pos.col >= 10)
    {
        return false; // New position is out of bounds
    }

    if (arr_[pos.row][pos.col] == SquareType::Wall || arr_[pos.row][pos.col] == SquareType::Enemies)
    {
        return false; // New position is a wall, cannot move there
    }

    Position currEnemyPosition = p->get_position();

    // Set the curr position of the enemy to the square value we stored prevousli.
    SetSquareValue(currEnemyPosition, p->get_Pre_Square_Value());

    // before moving the enemy to new location, store the squre type of the new location
    p->SetPreSquareValue(get_square_value(pos));

    // now move the enemy to the new location,
    // Set the square value at the new position to enemy
    SetSquareValue(pos, SquareType::Enemies);
    //  Update the enemy's position
    p->SetPosition(pos);

    return true;
};

// get the possible Positions that a Player could move to
// (not off the board or into a wall
std::vector<Position> Board::GetMoves(Player *p)
{
    std::vector<Position> possiblePositions;

    // Get the current position of the player
    Position currentPosition = p->get_position();
    int currentRow = currentPosition.row;
    int currentCol = currentPosition.col;

    // Check if the player can move right
    if (currentCol < 9 && arr_[currentRow][currentCol + 1] != SquareType::Wall)
    {
        possiblePositions.push_back({currentRow, currentCol + 1});
    }

    // Check if the player can move left
    if (currentCol > 0 && arr_[currentRow][currentCol - 1] != SquareType::Wall)
    {
        possiblePositions.push_back({currentRow, currentCol - 1});
    }

    // Check if the player can move up
    if (currentRow > 0 && arr_[currentRow - 1][currentCol] != SquareType::Wall)
    {
        possiblePositions.push_back({currentRow - 1, currentCol});
    }

    // Check if the player can move down
    if (currentRow < 9 && arr_[currentRow + 1][currentCol] != SquareType::Wall)
    {
        possiblePositions.push_back({currentRow + 1, currentCol});
    }

    return possiblePositions;
};

// get the possible Positions that a Enemy could move to
//  (not off the board or into a wall)
std::vector<Position> Board::GetEnemyMoves(Player *p)
{
    std::vector<Position> possiblePositions;

    // Get the current position of the player
    Position currentPosition = p->get_position();
    int currentRow = currentPosition.row;
    int currentCol = currentPosition.col;

    // Check if the player can move right
    if (currentCol < 9 && arr_[currentRow][currentCol + 1] != SquareType::Wall && currentCol < 9)
    {
        possiblePositions.push_back({currentRow, currentCol + 1});
    }

    // Check if the player can move left
    if (currentCol > 0 && arr_[currentRow][currentCol - 1] != SquareType::Wall)
    {
        possiblePositions.push_back({currentRow, currentCol - 1});
    }

    // Check if the player can move up
    if (currentRow > 0 && arr_[currentRow - 1][currentCol] != SquareType::Wall)
    {
        possiblePositions.push_back({currentRow - 1, currentCol});
    }

    // Check if the player can move down
    if (currentRow < 9 && arr_[currentRow + 1][currentCol] != SquareType::Wall)
    {
        possiblePositions.push_back({currentRow + 1, currentCol});
    }

    return possiblePositions;
};
