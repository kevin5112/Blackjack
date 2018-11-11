#include "blackjack.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

void DisplayMenu();

int     main()
{
   // std::srand(std::time(0));
    Card myCard(Card::HEART, Card::ACE);
    Deck myDeck;
    int  userResponse;

    do {
        DisplayMenu();
        std::cin >> userResponse;
        if(std::cin.fail())
        { 
            std::cout << "Not an int.. Please try again.\n\n";

            // clears cin error state
            std::cin.clear();

            //disgards 'bad' characters
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch(userResponse)
        {
            case(1):   
                // enter blackjack game
                myDeck.EnterGame();
                break;
            case(2):
                // display are you sure you want to start over, 
                // then start over if yes

                myDeck.ResetGame();
                std::cout << "Game has been reset!\n";
                break;
            case(3):
                // exit program
                std::cout << "Exiting program...\n";
                exit(EXIT_FAILURE);
                break;
            default:
                std::cout << "Invalid Response...\n\n";
                break;
        }
    } while(true);
    
    //myCard.PrintCard(myCard);
    myDeck.PrintDeck();
    myDeck.ShuffleDeck();
    std::cout << "Shuffling deck...\n";
    myDeck.PrintDeck();
    return 0;
}


void DisplayMenu()
{
    std::cout << "\n------- Blackjack Menu -------\n"
         << "1. Play Game\n"
         << "2. Reset Game\n"
         << "3. Quit\n"
         << "Enter Response: ";
}

