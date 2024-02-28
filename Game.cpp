#include <iostream>
#include "Player.h"
#include "Board.h"
#include "Game.h"

////////////// Game /////////////
// Constructer
Game::Game() : player("Player1", true)
{
    Board board;
}; // constructor

std::ostream &operator<<(std::ostream &os, const Game &m)
{
    const Board &board = m.board;

    for (int row = 0; row < 10; ++row)
    {
        for (int col = 0; col < 10; ++col)
        {
            os << SquareTypeStringify(board.get_square_value({row, col})) << " ";
        }
        os << "\n";
    }
    return os;
}

// initialize a new game, given one human player and a number of enemies to generate
void Game::NewGame(Player *human, std::vector<Player *> enemylist, const int enemies)
{
    enemyList_ = enemylist;
    for (Player *enemies : enemylist)
    {
        enemies->SetPreSquareValue(board.get_square_value(enemies->get_position()));
        board.SetSquareValue(enemies->get_position(), SquareType::Enemies);
    }
};

// Get the names of the possible moves for a player at a given position
std::vector<std::string> Game::GetMoveNames(Player *p, const std::vector<Position> &positions)
{
    std::vector<std::string> possibleMoves;

    // Get the current position of the player
    Position currentPlayerPos = p->get_position();

    for (const Position &pos : positions)
    {
        if (pos.row < currentPlayerPos.row)
        {
            possibleMoves.push_back("UP");
        }
        else if (pos.row > currentPlayerPos.row)
        {
            possibleMoves.push_back("DOWN");
        }
        else if (pos.col < currentPlayerPos.col)
        {
            possibleMoves.push_back("LEFT");
        }
        else if (pos.col > currentPlayerPos.col)
        {
            possibleMoves.push_back("RIGHT");
        }
    }

    return possibleMoves;
};

// Get the position chosen by the player from the given choice and available names and positions
Position Game::GetPositionFromChoice(const std::string choice, const std::vector<std::string> names, const std::vector<Position> positions)
{
    int currentRow = positions[0].row; // Starting from position {0,0}
    int currentCol = positions[0].col;
    // Convert choice to lowercase
    std::string lowercaseChoice = choice;
    std::transform(lowercaseChoice.begin(), lowercaseChoice.end(), lowercaseChoice.begin(), ::tolower);

    // Calculate the new position based on the player's choice
    if (lowercaseChoice == "right")
    {
        // Move right: increment the column index
        currentCol++;
    }
    else if (lowercaseChoice == "left")
    {
        // Move left: decrement the column index
        currentCol--;
    }
    else if (lowercaseChoice == "up")
    {
        // Move up: decrement the row index
        currentRow--;
    }
    else if (lowercaseChoice == "down")
    {
        // Move down: increment the row index
        currentRow++;
    }

    // Create a new position object with the calculated row and column indices.
    Position newPosition = {currentRow, currentCol};
    return newPosition;
};

// Handle the interaction between a player and an occupant of a square
bool Game::HandleOccupant(Player *p, SquareType occupant)
{
    // if this is a human player.
    if (p->get_is_human())
    {
        if (occupant == SquareType::Pellet)
        {
            p->ChangePoints(1);
            return false;
        }
        else if (occupant == SquareType::Treasure)
        {
            // Set has treasure to true.
            if (!p->get_has_treasure())
            {
                p->SetHasTreasure();
            }

            p->ChangePoints(100);

            return false;
        }
        else if (occupant == SquareType::Enemies)
        {

            if (p->get_has_treasure())
            {
                p->SetHasTreasure();
            };

            // Decreases a life from teh player.
            player.DecreaseLife(p);

            return true;
        }
    }

    // Else, player is enemy and the squreType is pacman, then return true.
    if (occupant == SquareType::Pacman)
    {
        return true;
    }

    return false;
};

// have the given Player take their turn
bool Game::TakeTurn(Player *p, const std::string &choice, std::vector<Player *> &enemylist)
{
    // Check if player pointer is valid
    if (p == nullptr)
    {
        return false;
    }

    // Get current position of the player
    Position currentPlayerPos = p->get_position();
    std::vector<Position> positions = board.GetMoves(p);
    // Get the possible moves for the player
    std::vector<std::string> possibleMoves = GetMoveNames(p, positions);

    std::cout << "Please Choose a Move: ";
    for (const std::string &move : possibleMoves)
    {
        std::cout << " " << move;
    }
    std::cout << std::endl;
    std::getline(std::cin, choice_);

    bool moveSuccess;
    if (choice_.empty())
    {
        return moveSuccess = false;
    }

    // Get new position based on player's choice
    Position newPosition = GetPositionFromChoice(choice_, possibleMoves, {currentPlayerPos});

    // Stores the value squre type of the newPosition
    SquareType occupant = board.get_square_value(newPosition);

    // Retrun true if occupant is enemy, so player attacks an enemy.
    bool playerAttacksEnemy = HandleOccupant(p, occupant);
    // Return success if the player moves to the new position.
    moveSuccess = board.MovePlayer(p, newPosition, enemylist);

    if (moveSuccess)
    {
        IncrementMoves();

        if (playerAttacksEnemy)
        {
            for (Player *enemy : enemyList_)
            {
                // Comparing enemyPosition with the player Position,
                //  To know which enemy the player is attacking.
                if (enemy->get_position() == newPosition)
                {
                    std::cout << "Player attacks enemy" << std::endl;
                    HandlePlayerAttacksEnemy(p, enemy);
                }
            }
        }

        // Enemy player turn
        for (Player *enemy : enemyList_)
        {
            if (TakeTurnEnemy(enemy))
            {
                std::cout << "Enemy attacks player" << std::endl;
                HandleEnemyAttacksPlayer(p, enemy);
            }
        }
    }

    return moveSuccess;
};

// have the enemy take turn
bool Game::TakeTurnEnemy(Player *p)
{
    // Get the possible positions for the enemy
    std::vector<Position> possiblePositions = board.GetMoves(p);

    // To choose a random position for the enemy from the possible position
    int randomIndex = rand() % possiblePositions.size();
    Position newPosition = possiblePositions[randomIndex];

    // Handle enemy interaction with an occupant square
    SquareType occupant = board.get_square_value(newPosition);
    bool enemyAttacksPlayer = HandleOccupant(p, occupant); // Returns true, if the occupant is a player.

    board.MoveEnemy(p, newPosition);

    return enemyAttacksPlayer;
};

// Takes a player, returns trues if game is over.
bool Game::IsGameOver(Player *p)
{
    if (p->get_is_dead())
    {
        std::cout << "GAME IS OVER, YOU LOST." << std::endl;
        return true;
    }
    else if (CheckifdotsOver())
    {
        std::cout << "YOU WON!" << std::endl;
        return true;
    }

    return false;
};

// Takes a player, prints out player information.
std::string GenerateReport(Player *p)
{
    std::string playerInfo = p->Stringify();
    return playerInfo;
};

// Return true if the the pellets in the game are over
bool Game::CheckifdotsOver()
{
    // Iterate over all positions on the board
    for (int row = 0; row < 10; ++row)
    {
        for (int col = 0; col < 10; ++col)
        {
            // Check if the square at the current position contains pellets
            if (board.get_square_value({row, col}) == SquareType::Pellet)
            {
                // Pellets still exists
                return false;
            }
        }
    }
    // Pellets are over
    return true;
};

// Return a random Position on the board, not a wall.
Position Game::GenerateRandomPosition()
{
    // Gets num of rows and num of Cols
    int numRows = board.get_rows();
    int numCols = board.get_cols();

    while (true)
    {
        int randRow = rand() % numRows;
        int randCol = rand() % numCols;
        Position randPos = {randRow, randCol};

        // Check if the square type at the random position is not a wall, and not a player.
        SquareType randPosSqureType = board.get_square_value(randPos);
        if (randPosSqureType != SquareType::Wall && randPosSqureType != SquareType::Pacman && randPosSqureType != SquareType::PowerfulPacman)
        {
            return randPos;
        }
    }
};

// Takes an enemy player nad a human player and
// handles the case of a player moving into an enemy.
void Game::HandlePlayerAttacksEnemy(Player *p, Player *enemy)
{

    board.SetSquareValue(enemy->get_position(), enemy->get_Pre_Square_Value());

    Position randPos = GenerateRandomPosition();
    SquareType preSquare = board.get_square_value(randPos);
    enemy->SetPreSquareValue(preSquare);

    enemy->SetPosition(randPos);
    board.SetSquareValue(randPos, SquareType::Enemies);
    board.SetSquareValue(p->get_position(), SquareType::Pacman);

    player.DecreaseLife(p);
};

// Takes an enemy player nad a human player and
// handles the case of an enemy moving into an huamn player.
void Game::HandleEnemyAttacksPlayer(Player *p, Player *enemy)
{
    // board.SetSquareValue(enemy->get_position(), enemy->get_Pre_Square_Value());
    board.SetSquareValue(p->get_position(), SquareType::Pacman);

    Position randPos = GenerateRandomPosition();
    SquareType preSquare = board.get_square_value(randPos);
    enemy->SetPreSquareValue(preSquare);

    enemy->SetPosition(randPos);
    board.SetSquareValue(randPos, SquareType::Enemies);

    player.DecreaseLife(p);
};
