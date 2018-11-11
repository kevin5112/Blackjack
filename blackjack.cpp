#include "blackjack.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>

void Card::PrintCard(Card card)const
{
    std::cout << card;
}

char Card::TurnSuit(SUIT suit) const
{
    char s;

    switch(suit)
    {
        case DIAMOND:
            s = 'D';
            break;
        case HEART:
            s = 'H';
            break;
        case CLUB:
            s = 'C';
            break;
        case SPADE:
            s = 'S';
            break;
    }

    return s;
}

char Card::TurnValue(VALUE value) const
{
    char v;

    switch(value)
    {
        case ACE:
            v = 'A';
            break;
        case TWO:
            v = '2';
            break;
        case THREE:
            v = '3';
            break;
        case FOUR:
            v = '4';
            break;
        case FIVE:
            v = '5';
            break;
        case SIX:
            v = '6';
            break;
        case SEVEN:
            v = '7';
            break;
        case EIGHT:
            v = '8';
            break;
        case NINE:
            v = '9';
            break;
        case TEN:
            v = 'T';
            break;
        case JACK:
            v = 'J';
            break;
        case QUEEN:
            v = 'Q';
            break;
        case KING:
            v = 'K';
            break;
    }
    return v;
}

int  Card::IntValue(VALUE value)const
{
    int v;

    switch(value)
    {
        case ACE:
            v = 11;
            break;
        case TWO:
            v = 2;
            break;
        case THREE:
            v = 3;
            break;
        case FOUR:
            v = 4;
            break;
        case FIVE:
            v = 5;
            break;
        case SIX:
            v = 6;
            break;
        case SEVEN:
            v = 7;
            break;
        case EIGHT:
            v = 8;
            break;
        case NINE:
            v = 9;
            break;
        case TEN:
            v = 10;
            break;
        case JACK:
            v = 10;
            break;
        case QUEEN:
            v = 10;
            break;
        case KING:
            v = 10;
            break;
    }
    return v;
}

std::ostream& operator<<(std::ostream& os, const Card& card)
{
    os << card.TurnSuit(card.suit) << card.TurnValue(card.value);
    return os;
}

Deck::Deck()
{
    currency = 100.00;
    size = DECK_SIZE;
    for(int suit = Card::DIAMOND; suit <= Card::SPADE; ++suit)
    {
        for(int value = Card::ACE; value <= Card::KING; ++value)
        {
            Card tempCard((Card::SUIT(suit)), (Card::VALUE(value)));
            deck.push_back(tempCard);
        }
    }
}

void Deck::PrintDeck()
{
    for(auto it = deck.begin(); it != deck.end(); ++it)
    {
        it->PrintCard(*it);
        std::cout << std::endl;
    }
}

void Deck::ShuffleDeck()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);

    // auto rng = std::default_random_engine {};
    std::shuffle(std::begin(deck), std::end(deck), e);
    
}

void Deck::DisplayCurrency()
{
    
    std::cout.precision(2);
    std::cout << "Currency: $" << std::fixed << currency << std::endl << std::endl;
}

void Deck::EnterGame()
{
    int response;
    do {
        DisplayGameMenu();
        std::cin >> response;
        if(std::cin.fail())
        { 
            std::cout << "Not an int.. Please try again.\n\n";

            // clears cin error state
            std::cin.clear();

            //disgards 'bad' characters
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        switch(response)
        {
            case(1):   
                // start game
                GameStart();
                break;
            case(2):
                // Reset deck
                ResetDeck();
                break;
            case(3):
                //Display Deck
                PrintDeck();
                break;
            case(4):
                //Shuffle Deck
                ShuffleDeck();
                break;
            case(5):
                // Check currency
                DisplayCurrency();
                break;
            case(6):
                // return to main menu
                std::cout << "Returning back to main menu...\n";
                return;
                break;
            default:
                std::cout << "Invalid Response...\n\n";
                break;
        }

    } while(true);
   
}

void Deck::DisplayGameMenu()
{
    std::cout << "\n------- Blackjack Game Menu -------\n"
         << "1. Start Game\n"
         << "2. New Deck\n"
         << "3. Display Deck\n"
         << "4. Shuffle Deck\n"
         << "5. Check Currency\n"
         << "6. Return to Main Menu\n"
         << "Enter Response: ";
}

void Deck::ResetGame()
{
    deck.clear();
    currency = 100.00;
    ResetDeck();
}

void Deck::ResetDeck()
{
    size = DECK_SIZE;
    deck.clear();
    for(int suit = Card::DIAMOND; suit <= Card::SPADE; ++suit)
    {
        for(int value = Card::ACE; value <= Card::KING; ++value)
        {
            Card tempCard((Card::SUIT(suit)), (Card::VALUE(value)));
            deck.push_back(tempCard);
        }
    }
}

void Deck::GameStart()
{
    std::vector<Card> dealersHand;
    std::vector<Card> usersHand;
    std::string quitResponse = "";
    std::string userChoice = "";
    double      betAmount = 0.00;
    bool        roundWin = true;
    bool        hitAndStay = true;
    int         userHandTotal = 0;
    int         dealerHandTotal = 0;
    // ran into a problem where Calculate bust was changing
    // total value too many times
    int         userAceCounter = 0;
    int         dealerAceCounter = 0;
    int         aceCounter = 0;
    bool        isUserBlackjack = false;
    bool        isDealerBlackjack = false;
    using namespace std::this_thread;
    using namespace std::chrono;

    std::cout << "\nShuffling Deck...\n";
    ShuffleDeck();

    while(quitResponse != "quit")
    {
        if(deck.size() < 10)
        {
            ResetDeck();
            std::cout << "Reshuffling Deck... \n";
            ShuffleDeck();
        }

        dealersHand.clear();
        usersHand.clear();

        // std::cout << deck.back() << std::endl;
        
        // passing out the cards to dealer and user
        for(int i = 0; i < 2; ++i)
        {
            //std::cout << "U: " << deck.back();
            usersHand.push_back(deck.back());
            if(deck.back().GetValue() == Card::ACE)
                ++userAceCounter;
            // puts value into total
            userHandTotal += deck.back().IntValue(deck.back().GetValue());
            deck.pop_back();

            //std::cout << "D: " << deck.back();
            dealersHand.push_back(deck.back());
            if(deck.back().GetValue() == Card::ACE)
                ++dealerAceCounter;
            // puts value into total
            dealerHandTotal += deck.back().IntValue(deck.back().GetValue());
            deck.pop_back();
        }

         // used to check to make sure total value was returning from function
        // std::cout << "userHandTotal: " << userHandTotal << std::endl;
        // std::cout << "dealerHandTotal: " << dealerHandTotal << std::endl;

        betAmount = BettingAction();
        // betting done...

           
        std::cout << "Dealer: " << dealersHand.front() << ' ' << "? ?" << std::endl;
        std::cout << "User: " << usersHand.front() << ' ' << usersHand.back() << std::endl;

        // check for blackjack
        if(CheckBlackjack(userHandTotal))
            isUserBlackjack = true;

        if(CheckBlackjack(dealerHandTotal))
            isDealerBlackjack = true;

        // loop for if the user wants to hit and get more cards
        while(hitAndStay)
        {
            DisplayInGameMenu();
            std::cin >> userChoice;

            if(userChoice == "hit")
            {
                if(deck.back().GetValue() == Card::ACE)
                    ++userAceCounter;
                usersHand.push_back(GetCard());
                std::cout << "Dealer: " << dealersHand.front() << ' ' << "? ?" << std::endl;
                std::cout << "User's Hand: ";
                DisplayHand(usersHand);
                hitAndStay = CalculateBust(usersHand, userHandTotal, roundWin, aceCounter, userAceCounter);
                // std::cout << "userHandTotal: " << userHandTotal << std::endl;
                if(userHandTotal > 21)
                    std::cout << "You bust!\n";
                // std::cout << "userHandTotal: " << userHandTotal << std::endl;
            }
            else if(userChoice == "stay")
                hitAndStay = false;
            else
                std::cout << "Invalid Response..";
        }

        std::cout << "Dealer's Hand: ";
        DisplayHand(dealersHand);
        std::cout << "User's Hand: ";
        DisplayHand(usersHand);
        sleep_until(system_clock::now() + seconds(1));

        aceCounter = 0;
        if((roundWin == true) && ((!isDealerBlackjack) && (!isUserBlackjack)))
        {
            // std::cout << "Enter roundwin/dealerblackjack/userblackjack if block \n";
            while(dealerHandTotal < 17)
            {
                // std::cout << "Enter DealerwhileLoop\n";
                if(deck.back().GetValue() == Card::ACE)
                    ++dealerAceCounter;
                dealersHand.push_back(GetCard());
                std::cout << "Dealer's Hand: ";
                DisplayHand(dealersHand);
                std::cout << "User's Hand: ";
                DisplayHand(usersHand);
                CalculateBust(dealersHand, dealerHandTotal, roundWin, aceCounter, dealerAceCounter);
                // std::cout << "dealerHandTotal: " << dealerHandTotal << std::endl;
                sleep_until(system_clock::now() + seconds(1));
            }
            if((dealerHandTotal >= 17) && (dealerHandTotal <= 21))
            {
                // std::cout << "Enter CheckWinner block\n";
                roundWin = CheckWinner(userHandTotal, dealerHandTotal);
            }
            else if(dealerHandTotal > 21)
            {
                std::cout << "Dealer bust!\n";
                roundWin = true;
            }
            else
                std::cout << "You fucked up kid\n";
        }

        if(userHandTotal == dealerHandTotal)
        {
            std::cout << "Tie!!\n";
            DisplayCurrency();
        }
        else if(isUserBlackjack || isDealerBlackjack)
        {
            std::cout << "Blackjack!!\n";
            if(isUserBlackjack)
                AddBlackjackCurrency(betAmount);
            else if(isDealerBlackjack)
                DeductCurrency(betAmount);
        }
        else if(roundWin)
        {
            AddCurrency(betAmount);
        }
        else
        {
            DeductCurrency(betAmount);
        }

        roundWin = true;
        hitAndStay = true;
        userHandTotal = 0;
        dealerHandTotal = 0;
        isDealerBlackjack = false;
        isUserBlackjack = false;
        aceCounter = 0;
        // ask if user would like to play again
        std::cout << "Would you like to continue?(Type 'quit' to quit) ";
        std::cin >> quitResponse;
        std::transform(quitResponse.begin(), quitResponse.end(), quitResponse.begin(), ::tolower);
        if(quitResponse != "quit")
        {
            std::cout << "You did not type 'quit'... We shall continue.\n";
        }
    }
}

Card Deck::GetCard()
{   
    Card newCard = deck.back();
    deck.pop_back();
    return newCard;
}

double Deck::BettingAction()
{
    // bet amount and check to see if it's a valid amount
    double amount = 0.00;
    bool amountCheck = true;
    do{
        std::cout << "How much do you want to bet? ";
        std::cin >> amount;

        if(std::cin.fail())
        { 
            std::cout << "Not an int.. Please try again.\n\n";

            // clears cin error state
            std::cin.clear();

            //disgards 'bad' characters
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if(amount <= currency)
            amountCheck = false;
        else if(amount > currency)
        {
            std::cout << "You do not have enough money..\n";
            amountCheck = true;
        }

    } while(amountCheck);
    return amount;
}

void Deck::DisplayHand(std::vector<Card> hand)
{
    for(auto it = hand.begin(); it != hand.end(); ++it)
    {
        std::cout << *it << ' ';
    }
    std::cout << std::endl;
}

void Deck::DisplayInGameMenu()
{
    std::cout << "\n----- Options -----\n"
              << "hit.\n"
              << "stay.\n"
              << "Pick an option: ";
}

void Deck::DeductCurrency(double amount)
{
    double newAmount = amount * 0.66;
    std::cout.precision(2);
    std::cout << "Get rekt! You lost! You lose $" << std::fixed << newAmount << std::endl;
    
    currency -= newAmount;
    std::cout << "Your new balance is $" << std::fixed << currency << std::endl;
}

void Deck::AddCurrency(double amount)
{
    double newAmount = amount * 1.33;
    std::cout.precision(2);
    std::cout << "Congrats! You won.. this round. You won $" << std::fixed << newAmount << std::endl;
    
    currency += newAmount;
    std::cout << "Your new balance is $" << std::fixed << currency << std::endl;
}

void Deck::AddBlackjackCurrency(double amount)
{
    double newAmount = amount * 1.50;
    std::cout.precision(2);
    std::cout << "Congrats! You won.. this round. You won $" << std::fixed << newAmount << std::endl;
    
    currency += newAmount;
    std::cout << "Your new balance is $" << std::fixed << currency << std::endl;

}

bool Deck::CalculateBust(std::vector<Card> hand, int &totalValue, bool &roundWin, int &aceCounter, int handAceCounter)
{
    int newValue = hand.back().IntValue(hand.back().GetValue());
    int newTotalValue = newValue + totalValue;

    if(newTotalValue > 21)
    {
        for(auto it = hand.begin(); it != hand.end(); ++it)
        {
            if(it->GetValue() == Card::ACE)
            {
                // std::cout << "I found an ace\n";
                if(aceCounter != handAceCounter)
                {
                    newTotalValue -= 10;
                    ++aceCounter;
                    break;
                }
            }
        }
    }

    totalValue = newTotalValue;

    if(newTotalValue > 21)
    { 
        roundWin = false;
        return false;
    }
    else
        return true;
}

bool Deck::CheckWinner(int userTotal, int dealerTotal)
{
    if(userTotal > dealerTotal)
        return true;
    else
        return false;

}

bool Deck::CheckBlackjack(int total)
{
    if(total == 21)
        return true;
    else 
        return false;
}


