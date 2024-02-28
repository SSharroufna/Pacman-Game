#include "Player.h"
#include "Game.h"
#include <iostream>
#include <string>

// Name: Sarah Sharroufna

// Step 2 — improving the game: I chose the #1
// Once the enemy is destroyed by the player,
// spawn a new enemy at a random location on the board.

// Do "make all" to run main and test.
// Do "make main" to run main only. "make test" for test.

int main()
{
    Game game;
    Player *human = new Player("Sarah", true);

    // A vector to hold enemy players
    std::vector<Player *> enemyList;

    // Allow for different random number each game.
    srand(time(nullptr));
    // Create and set properties for each enemy
    for (int i = 0; i < 3; i++)
    {
        // Create a new enemy player
        Player *enemy = new Player("Enemy", false);

        // Set position for the enemy
        Position enemyPosition = game.GenerateRandomPosition();
        enemy->SetPosition(enemyPosition);

        // Add the enemy to the enemy list
        enemyList.push_back(enemy);
    }

    // Starting a new game: in
    game.NewGame(human, enemyList, 3);

    while (!game.IsGameOver(human))
    {
        std::cout << game;
        std::string choice_;

        game.TakeTurn(human, choice_, enemyList);

        // Print the player remaining lives, points, # of moves.
        std::cout << "Lives: " << human->get_lives() << std::endl;
        std::cout << "Points: " << human->get_points() << std::endl;
        std::cout << "Number of moves: " << game.get_moves() << std::endl;
    }
    std::string GenerateReport(Player * p);
    std::string report = GenerateReport(human);

    std::cout << report << std::endl;

    return 0;
}