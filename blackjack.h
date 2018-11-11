#ifndef BLACKJACK_H
#define BLACKJACK_H
#include <vector>
#include <iostream>

const int DECK_SIZE = 52;
const int BLACKJACK_MAX = 21;

class Card 
{
public:
    enum SUIT {DIAMOND, CLUB, HEART, SPADE};
    enum VALUE {ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};

    // constructor
    Card();
    Card(SUIT suit, VALUE value)
    {
        this->suit = suit;
        this->value = value;
    }

    SUIT GetSuit() { return suit; }
    void SetSuit(SUIT suit) { this->suit = suit; }
    VALUE GetValue() { return value; }
    void SetValue(VALUE value) { this->value = value; }
    void PrintCard(Card value)const;
    char TurnSuit(SUIT suit)const;
    char TurnValue(VALUE value)const;
    int  IntValue(VALUE value)const;
    int  ChangeAce() { return 1; }

    friend std::ostream& operator<<(std::ostream& os, const Card& card);

private:
    SUIT suit;
    VALUE value;
}; // end of Card class

class Deck
{
public:
    Deck();

    void PrintDeck();
    void ShuffleDeck();
    void DisplayCurrency(); 
    void ResetCurrency() { currency = 100.00; }
    void EnterGame();
    void DisplayGameMenu();
    void ResetGame();
    void GameStart();
    void ResetDeck();
    void DisplayInGameMenu();
    void DeductCurrency(double amount);
    void AddCurrency(double amount);
    double BettingAction();
    Card GetCard();
    void DisplayHand(std::vector<Card> hand);
    bool CalculateBust(std::vector<Card> hand, int &totalValue, bool &roundWin, int &aceCounter, int handAceCounter);
    bool CheckWinner(int userTotal, int dealerTotal);
    bool CheckBlackjack(int total);
    void AddBlackjackCurrency(double amount);


private:
    std::vector<Card> deck;
    int size;
    double currency;
}; // end of Deck class



#endif