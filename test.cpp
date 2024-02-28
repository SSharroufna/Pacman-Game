#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "Board.h"
#include "Game.h"
#include "Player.h"

// Test cases for setting squre value, a sample test.
TEST_CASE("Set square value in the board", "[SetSquareValue]")
{
    // Initialize a board object
    Board board;

    // Set up the board with some values,
    // and then get the value at that position to check it.
    SECTION("Set square value for PowerfulPacman square")
    {
        board.SetSquareValue({0, 0}, SquareType::PowerfulPacman);
        REQUIRE(board.get_square_value({0, 0}) == SquareType::PowerfulPacman);
    }

    SECTION("Set getting square value for Pellet")
    {
        board.SetSquareValue({0, 1}, SquareType::Pellet);
        REQUIRE(board.get_square_value({0, 1}) == SquareType::Pellet);
    }

    SECTION("Set getting square value for Wall")
    {
        board.SetSquareValue({1, 0}, SquareType::Wall);
        REQUIRE(board.get_square_value({1, 0}) == SquareType::Wall);
    }
}

// Two test cases for movePlayer function.
TEST_CASE("MovePlayer with Valid Moves", "[MovePlayer]")
{
    // Setup
    Board board;
    Player player("Jack", true);
    player.SetPosition({0, 0});
    std::vector<Player *> enemies; // Empty vector

    // Test valid moves within bounds
    SECTION("Valid Moves Within Bounds")
    {
        Position validMoves[] = {{0, 1}, {1, 0}}; // Example valid moves
        for (const auto &newPos : validMoves)
        {
            bool moveSuccess = board.MovePlayer(&player, newPos, enemies);
            REQUIRE(moveSuccess == true);
            REQUIRE(player.get_position() == newPos);
            REQUIRE(board.get_square_value(newPos) == SquareType::Pacman); // Check square type
        }
    }
}
TEST_CASE("MovePlayer with Invalid Moves", "[MovePlayer]")
{
    // Setup board, player, and a position.
    Board board;
    Player player("Alex", true);
    player.SetPosition({9, 9});
    std::vector<Player *> enemies;

    // Test moving out of bounds
    SECTION("Move Out of Bounds")
    {
        Position invalidMoves[] = {{10, 9}, {9, 10}}; // Example out-of-bounds moves
        for (const auto &newPos : invalidMoves)
        {
            bool moveSuccess = board.MovePlayer(&player, newPos, enemies);
            REQUIRE(moveSuccess == false);
            REQUIRE(player.get_position() == Position{9, 9});              // Ensure player position unchanged
            REQUIRE(board.get_square_value({9, 9}) == SquareType::Pacman); // Check square type at original position
        }
    }

    // Test moving into walls
    SECTION("Move Into Walls")
    {
        // Create walls on the board
        board.SetSquareValue({8, 9}, SquareType::Wall);
        board.SetSquareValue({9, 8}, SquareType::Wall);

        Position wallMoves[] = {{8, 9}, {9, 8}};
        for (const auto &newPos : wallMoves)
        {
            bool moveSuccess = board.MovePlayer(&player, newPos, enemies);
            REQUIRE(moveSuccess == false);
            REQUIRE(player.get_position() == Position{9, 9});              // To ensure player position didnt change
            REQUIRE(board.get_square_value({9, 9}) == SquareType::Pacman); // Check the squre value at the same location.
        }
    }
}

// Test GetPositionFromChoice().
TEST_CASE("Get New Position From User's Choice", "[GetPositionFromChoice]")
{
    // Setup game, moveNames, and start position.
    Game game;
    std::vector<std::string> names = {"right", "left", "up", "down"};
    std::vector<Position> positions = {{0, 0}};

    // Test moving right
    SECTION("Move Right")
    {
        // Get the newPosition by calling the function, then check it.
        Position newPosition = game.GetPositionFromChoice("right", names, positions);
        REQUIRE(newPosition == Position{0, 1});
    }

    // Test moving left
    SECTION("Move Left")
    {
        // Get the newPosition by calling the function, then check it.
        Position newPosition = game.GetPositionFromChoice("left", names, positions);
        REQUIRE(newPosition == Position{0, -1});
    }

    // Test moving up
    SECTION("Move Up")
    {
        // Get the newPosition by calling the function, then check it.
        Position newPosition = game.GetPositionFromChoice("up", names, positions);
        REQUIRE(newPosition == Position{-1, 0});
    }

    // Test moving down
    SECTION("Move Down")
    {
        // Get the newPosition by calling the function, then check it.
        Position newPosition = game.GetPositionFromChoice("down", names, positions);
        REQUIRE(newPosition == Position{1, 0});
    }
}

// Test cases for handle interaction.
TEST_CASE("Handle interaction between player and occupant", "[HandleOccupant]")
{
    // Initialize a player object
    Player *human = new Player("Sarah", true);
    Player *enemy = new Player("Enemy", false);
    Game game;

    SECTION("Player encounters a Pellet")
    {
        SquareType occupant = SquareType::Pellet;
        game.HandleOccupant(human, occupant);
        REQUIRE(human->get_points() == 1); // Assert that points increased by 1
    }

    SECTION("Player encounters a Treasure")
    {
        SquareType occupant = SquareType::Treasure;
        game.HandleOccupant(human, occupant);
        REQUIRE(human->get_points() == 100);
        REQUIRE(human->get_has_treasure() == true);
    }

    SECTION("Player encounters an Enemy without Treasure")
    {
        human->SetLives(3);
        SquareType occupant = SquareType::Enemies;
        game.HandleOccupant(human, occupant);
        REQUIRE(human->get_lives() == 2);
        REQUIRE(human->get_is_dead() == false);
    }

    SECTION("Player encounters an Enemy with Treasure")
    {
        human->SetHasTreasure(); // To Set treasure to true
        human->SetLives(3);      // Set initial lives
        SquareType occupant = SquareType::Enemies;
        game.HandleOccupant(human, occupant);
        REQUIRE(human->get_lives() == 2);
        REQUIRE(human->get_has_treasure() == false);
        REQUIRE(human->get_is_dead() == false);
    }

    SECTION("Enemy encounters Pacman")
    {
        SquareType occupant = SquareType::Pacman;
        REQUIRE(game.HandleOccupant(enemy, occupant) == true); // Returns true since the enemy is enounter with a player.
    }
}