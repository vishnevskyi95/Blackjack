#include <iostream>
#include <Windows.h>
#include <array>
#include <random>
#include <cstdint>

enum RankCard {
    RANK_2,
    RANK_3,
    RANK_4,
    RANK_5,
    RANK_6,
    RANK_7,
    RANK_8,
    RANK_9,
    RANK_10,
    RANK_JACK,
    RANK_QUEEN,
    RANK_KING,
    RANK_ACE,
    RANK_MAX
};
enum SuitCard {
    SUIT_HEART,
    SUIT_DIAMOND,
    SUIT_CLUB,
    SUIT_SPADE,
    SUIT_MAX
};
enum BlackjackResult
{
    BLACKJACK_PLAYER_WIN,
    BLACKJACK_DEALER_WIN,
    BLACKJACK_DRAW
};
enum Errors {
    ERROR_RANK_CARD,
    ERROR_SUIT_CARD,
    ERROR_VALUE_CARD,
    ERROR_RESULT_GAME,
    ERROR_MAIN_MENU
};
struct Card {
    RankCard rank;
    SuitCard suit;
};

void printCard(const Card& card) {
    using namespace std;
    switch (card.rank)
    {
    case RANK_2:
        cout << "2";
        break;
    case RANK_3:
        cout << "3";
        break;
    case RANK_4:
        cout << "4";
        break;
    case RANK_5:
        cout << "5";
        break;
    case RANK_6:
        cout << "6";
        break;
    case RANK_7:
        cout << "7";
        break;
    case RANK_8:
        cout << "8";
        break;
    case RANK_9:
        cout << "9";
        break;
    case RANK_10:
        cout << "10";
        break;
    case RANK_JACK:
        cout << "J";
        break;
    case RANK_QUEEN:
        cout << "Q";
        break;
    case RANK_KING:
        cout << "K";
        break;
    case RANK_ACE:
        cout << "A";
        break;
    default:
        cout << "Error: " << ERROR_RANK_CARD;
        break;
    }
    switch (card.suit) {
    case SUIT_HEART:
        cout << "H";
        break;
    case SUIT_DIAMOND:
        cout << "D";
        break;
    case SUIT_CLUB:
        cout << "C";
        break;
    case SUIT_SPADE:
        cout << "S";
        break;
    default:
        cout << "Error: " << ERROR_SUIT_CARD;
        break;
    }
}
void printDeck(const std::array<Card, 52>& deck) {
    using namespace std;
    for (const auto card : deck) {
        printCard(card);
        cout << " ";
    }
}
void swapCard(Card& cardOne, Card& cardTwo) {
    Card cardTemp = cardOne;
    cardOne = cardTwo;
    cardTwo = cardTemp;
}
void mixDeck(std::array<Card, 52>& deck) {
    using namespace std;
    random_device rd;
    mt19937 mersenne(rd());
    for (int card(0); card < 52; card++)
        swapCard(deck[card], deck[mersenne() % 51]);
}
int getCardValue(const Card& card) {
    using namespace std;
    switch (card.rank)
    {
    case RANK_2:
        return 2;
    case RANK_3:
        return 3;
    case RANK_4:
        return 4;
    case RANK_5:
        return 5;
    case RANK_6:
        return 6;
    case RANK_7:
        return 7;
    case RANK_8:
        return 8;
    case RANK_9:
        return 9;
    case RANK_10:
    case RANK_JACK:
    case RANK_QUEEN:
    case RANK_KING:
        return 10;
    case RANK_ACE:
        return 11;
    default:
        cout << "Error: " << ERROR_VALUE_CARD;
        return 0;
    }
}
char getPlayerChoice() {
    using namespace std;
    char choice('0');
    do {
        cout << "Взять карту? (y/n): ";
        if (cin.fail()) {
            cin.clear();
            cout << "Ошибка ввода;" << endl;
            cin.ignore(32767, '\n');
            continue;
        }
        cin >> choice;
        cin.ignore(32767, '\n');
    } while (choice != 'y' && choice != 'n');
    return choice;
}
int getMenuChoice(const uint32_t* const PtrPlayerCash) {
    using namespace std;
    int choice(0);
    do {
        if (*PtrPlayerCash != 0)
            cout << "Новая игра/Статистика сессии/Изменить ставку/Выход (1/2/3/4): ";
        else
            cout << "Новая игра/Статистика сессии/Взять в долг/Выход (1/2/3/4): ";
        if (cin.fail()) {
            cin.clear();
            cout << "Ошибка ввода;" << endl;
            cin.ignore(32767, '\n');
            continue;
        }
        cin >> choice;
        cin.ignore(32767, '\n');
    } while (choice != 1 && choice != 2 && choice != 3 && choice != 4);
    return choice;
}
void useAce(int* score, int* ace) {
    using namespace std;
    while (*ace > 0 && *score > 21) {
        --(*ace);
        *score -= 10;
        cout << "Сыграла мягкая рука!" << endl;
    }
}
float rounding(const float& number) {
    float roundNumber = number;
    roundNumber *= 10;
    roundNumber = floor(roundNumber + 0.5);
    roundNumber /= 10;
    return roundNumber;
}
void getCashe(const uint32_t* const PtrDealerCash, const uint32_t* const PtrPlayerCash, const unsigned int* const PtrRate) {
    using namespace std;
    cout << "- Банк: " << *PtrDealerCash << "$; Игрок: " << *PtrPlayerCash << "$; Ставка: " << *PtrRate << "$" << endl;
}
int getRate(const uint32_t* const PtrPlayerCash, const unsigned int* const PtrRate) {
    using namespace std;
    int rate(0);
    cout << "- Баланс: " << *PtrPlayerCash << "$; Текущая ставка: " << *PtrRate << "$; ";
    do {
        cout << "Новая ставка: ";
        cin >> rate;
        if (cin.fail()) {
            cin.clear();
            cout << "Ошибка ввода;" << endl;
            cin.ignore(32767, '\n');
        }
        cin.ignore(32767, '\n');
        if (rate > *PtrPlayerCash && rate > 0)
            cout << "У вас недостаточно денег! Новая ставка: ";
        if (rate < 0)
            cout << "Ставка не может быть отрицательной! Новая ставка:";
    } while (rate < 0 || rate > *PtrPlayerCash);
    return rate;
}
unsigned int getCredit(uint32_t* const PtrDealerCash, unsigned int* PtrCredit, boolean* PtrTakeCredit, unsigned int* PtrCreditIntersetNow, boolean* PtrTakeCreditNow) {
    using namespace std;
    int cash(0);
    cout << "- Вы можете взять денег у дилера, но не более 1000$ за один раз под 50%!" << endl;
    do {
        cout << "Я хочу взять: ";
        cin >> cash;
        if (cin.fail()) {
            cin.clear();
            cout << "Ошибка ввода;" << endl;
            cin.ignore(32767, '\n');
        }
        cin.ignore(32767, '\n');
        if (cash == 0) {
            cout << "- Вы ничего не взяли." << endl;
            break;
        }
        if (cash < 0)
            cout << "- Вы не можете взять отрицательную сумму!" << endl;
        if (cash > 1000)
            cout << "- Вы не можете взять больше 1000$!" << endl;
        if (cash > 0 && cash <= 1000) {
            *PtrCreditIntersetNow = cash * 0.5;
            *PtrCredit += cash + *PtrCreditIntersetNow;
            *PtrDealerCash -= cash;
            *PtrTakeCreditNow = true;
            *PtrTakeCredit = true;
            cout << "- Вы взяли " << cash << "$, вам придется отдать: " << cash + *PtrCreditIntersetNow << "$!" << endl;
            if (*PtrCredit != cash + *PtrCreditIntersetNow)
                cout << "- Ваш общий долг: " << *PtrCredit << "$!" << endl;
        }
    } while (cash < 0 || cash > 1000);
    return cash;
}
void dealerMessage(unsigned int* PtrCredit) {
    using namespace std;
    random_device rd;
    mt19937 mersenne(rd());
    if (mersenne() % 125 == 1 || mersenne() % 125 == 2)
        cout << "* Новое сообщение: Эй, ты задолжал нам " << *PtrCredit << "$! Лучше тебе не откладывать этот вопрос, у тебя что, две жизни?" << endl;
    if (mersenne() % 125 == 3 || mersenne() % 125 == 4)
        cout << "* Новое сообщение: Уважаемый должник, немедленно погасите свою задолжность в размере " << *PtrCredit << "$!" << endl;
    if (mersenne() % 125 == 5)
        cout << "* Новое сообщение: Привет сука, забыл про меня? Готовь " << *PtrCredit << "$ или пиши завещание." << endl;
    if (mersenne() % 125 == 6 || mersenne() % 125 == 7)
        cout << "* Новое сообщение: Добрый день, напоминаем вам о задолжности в размере " << *PtrCredit << "$. Погасите её в ближайшее время, ваш дилер." << endl;
}
BlackjackResult playBlackjack(std::array<Card, 52>& deck) {
    using namespace std;
    int playerScore(0);
    int dealerScore(0);
    int playerAce(0);
    int dealerAce(0);
    const Card* cardPtr = &deck[0];

    if (getCardValue(*cardPtr) == 11)
        ++dealerAce;
    dealerScore += getCardValue(*cardPtr++);
    useAce(&dealerScore, &dealerAce);
    cout << "^ ";
    printCard(*(cardPtr - 1));
    cout << " - Дилер: " << dealerScore << endl;

    if (getCardValue(*cardPtr) == 11)
        ++playerAce;
    playerScore += getCardValue(*cardPtr++);
    useAce(&playerScore, &playerAce);
    cout << "^ ";
    printCard(*(cardPtr - 1));
    cout << " - Игрок: " << playerScore << endl;

    if (getCardValue(*cardPtr) == 11)
        ++playerAce;
    playerScore += getCardValue(*cardPtr++);
    useAce(&playerScore, &playerAce);
    cout << "^ ";
    printCard(*(cardPtr - 1));
    cout << " - Игрок: " << playerScore << endl;

    while (true) {
        if (playerScore > 21)
            return BLACKJACK_DEALER_WIN;
        if (playerScore == 21)
            break;
        char fork = getPlayerChoice();
        if (fork == 'n')
            break;
        playerScore += getCardValue(*cardPtr++);
        useAce(&playerScore, &playerAce);
        cout << "^ ";
        printCard(*(cardPtr - 1));
        cout << " - Игрок: " << playerScore << endl;
    }
    while (dealerScore < 17 || dealerAce != 0) {
        if (dealerScore > playerScore)
            break;
        if (getCardValue(*cardPtr) == 11)
            ++dealerAce;
        dealerScore += getCardValue(*cardPtr++);
        useAce(&dealerScore, &dealerAce);
        cout << "^ ";
        printCard(*(cardPtr - 1));
        cout << " - Дилер: " << dealerScore << endl;
    }
    if (dealerScore > 21)
        return BLACKJACK_PLAYER_WIN;
    if (dealerScore == playerScore)
        return BLACKJACK_DRAW;
    if (dealerScore > playerScore)
        return BLACKJACK_DEALER_WIN;
    else
        return BLACKJACK_PLAYER_WIN;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    using std::cout;
    using std::cin;
    using std::endl;
    cout << "Привет, BlackJack!" << endl;
    cout << ".........." << endl;

    std::array<Card, 52> deck;
    int card(0);
    for (int rank(0); rank < RANK_MAX; rank++)
        for (int suit(0); suit < SUIT_MAX; suit++) {
            deck[card].rank = static_cast<RankCard>(rank);
            deck[card].suit = static_cast<SuitCard>(suit);
            ++card;
        }

    int numberWins(0);
    int numberLosses(0);
    int numberDraws(0);

    uint32_t playerCash(1000);
    uint32_t dealerCash(1000000);
    unsigned int rate(0);
    int balance(0);
    unsigned int credit(0);
    boolean takeCredit = false;
    boolean takeCreditNow = false;
    unsigned int interest(0);
    unsigned int creditIntersetNow(0);

    getCashe(&dealerCash, &playerCash, &rate);

    do {
        int menuChoice = getMenuChoice(&playerCash);
        switch (menuChoice)
        {
        case 1:
            mixDeck(deck);
            switch (playBlackjack(deck))
            {
            case 0:
                if (rate == 0)
                    cout << "# Игрок выйгарл!" << endl;
                else {
                    cout << "# Игрок выйграл " << rate << "$!" << endl;
                    playerCash += rate;
                    dealerCash -= rate;
                    balance += rate;
                }
                ++numberWins;
                break;
            case 1:
                if (rate == 0)
                    cout << "# Дилер выйгарл!" << endl;
                else {
                    cout << "# Игрок проиграл " << rate << "$!" << endl;
                    playerCash -= rate;
                    dealerCash += rate;
                    balance -= rate;
                }
                ++numberLosses;
                break;
            case 2:
                cout << "# Ничья!" << endl;
                ++numberDraws;
                break;
            default:
                cout << "Error: " << ERROR_RESULT_GAME;
                break;
            }
            if (playerCash == 0) {
                rate = 0;
                cout << "Вы проиграли все свои деньги! Вы можете продолжить игру, но с пустыми карманами... или возьмите денег у дилера!" << endl;
            }
            if (credit != 0 && playerCash >= credit + 200) {
                cout << "Пришло время платить по долгам! Вы отдаёте дилеру: " << credit << "$; Ваш баланс: " << playerCash - credit << "$; Ставка обнулена." << endl;
                playerCash -= credit;
                dealerCash += credit;
                interest += creditIntersetNow;
                creditIntersetNow = 0;
                credit = 0;
                rate = 0;
                takeCreditNow = false;
            }
            if (takeCreditNow)
                dealerMessage(&credit);
            if (playerCash < rate) {
                cout << "У вас нехватает денег на игру по той же ставке! Ставка обнулена!" << endl;
                rate = 0;
            }
            break;
        case 2:
            if (numberWins + numberLosses + numberDraws == 0)
                cout << "- Вы не сыграли ни одной игры!" << endl;
            else
                cout << "- Игр: " << numberWins + numberLosses + numberDraws << "; Побед/Проигрышей/Ничьих ("
                << numberWins << " " << rounding(static_cast<float>(numberWins) / (numberWins + numberLosses + numberDraws) * 100) << "%/"
                << numberLosses << " " << rounding(static_cast<float>(numberLosses) / (numberWins + numberLosses + numberDraws) * 100) << "%/"
                << numberDraws << " " << rounding(static_cast<float>(numberDraws) / (numberWins + numberLosses + numberDraws) * 100) << "%)"
                << endl;
            getCashe(&dealerCash, &playerCash, &rate);
            if (balance < 0)
                cout << "- Вы в проигрыше на " << abs(balance) << "$" << endl;
            if (balance > 0)
                cout << "- Вы в выигрыше на " << balance << "$" << endl;
            if (balance == 0)
                cout << "- Ваш баланс не изменился!" << endl;
            if (credit != 0)
                cout << "- Вы должны " << credit << "$!" << endl;
            if (takeCredit && interest != 0)
                cout << "- Вы выплатили % по кредиту на сумму: " << interest << "$!" << endl;
            break;
        case 3:
            if (playerCash == 0)
                playerCash = getCredit(&dealerCash, &credit, &takeCredit, &creditIntersetNow, &takeCreditNow);
            else
                rate = getRate(&playerCash, &rate);
            break;
        case 4:
            return 0;
        default:
            cout << "Error: " << ERROR_MAIN_MENU;
            break;
        }
    } while (true);

    cin.clear();
    cin.ignore(32767, '\n');
    cin.get();
}
