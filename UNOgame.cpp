#include <iostream>  
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <fstream>
#include <conio.h>
#define BLACK 0
#define RED 4
#define GREEN 2
#define YELLOW 6
#define BLUE 1
#define WHITE 7
using namespace std;

// Game constants
const int colors = 4; // Number of card colors
const int numtypes = 15; // Number of card types per color
const int handSize = 7; // Initial hand size for each player
const int totalcards = 108; // Total number of cards in the deck

// Game variables
int deck[colors][numtypes]; // 2D array representing the deck
int discardPile[totalcards]; // Array representing the discard pile
int discardTop = -1; // Index of the top card in the discard pile
int player1Hand[handSize + 20]; // Player 1's hand
int player2Hand[handSize + 20]; // Player 2's hand
int handSize1 = handSize; // Size of Player 1's hand
int handSize2 = handSize; // Size of Player 2's hand
int currentplayer = 1; // Current player's turn (1 or 2)
int direction = 1; // Direction of play (1 for clockwise)

// Function prototypes
void initializeDeck(int deck[][numtypes]);
void shuffleDeck(int deck[][numtypes]);
void dealCards(int playerHand[], int &handSize, int &deckIndex);
void displayHand(int hand[], int handSize);
void playTurn(int playerHand[], int &handSize, bool &gameWon, int &deckIndex);
bool isValidPlay(int playerCard, int topCard);
void drawCard(int playerHand[], int &handSize, int &deckIndex);
void handleSpecialCard(int specialCardType, bool &gameWon, int &index, int playerHand[], int &handsize);
void displayCard(int card);
void clearScreen();
void logGameResult(const string &result);
void printBoard(int card, int chosenColor = -1);
void setColor(int textColor, int bgColor);
int selectColor();
void applyWildCardEffect(int wildType, bool &gameWon, int playerHand[], int &handSize, int &deckIndex);
void skipTurn();
void reverseTurn();
void drawTwoCards(int playerHand[], int &handSize, int &deckIndex);
bool isSpecialCard(int card);
void loadGameHistory();
bool unoCall(int currentPlayer, int handSize1, int handSize2);
void displaySpecialCards(int deck[][numtypes]);

// Sets text and background color in the console
void setColor(int textColor, int bgColor) 
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}

int main() 
{
    char choice;
    displaySpecialCards(deck);
    while (true) 
    {
        // Main menu
        cout << endl << "============================== UNO GAME =============================" << endl << endl << endl;
        cout << "                              1. Play Game" << endl << endl;
        cout << "                              2. Score History" << endl << endl;
        cout << "                              3. Exit" << endl << endl;

        // User choice
        label:
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == '1') 
        {
            // Game setup
            clearScreen();
            int index = 0;
            bool gameWon = false;

            initializeDeck(deck); // Initialize the deck
            shuffleDeck(deck); // Shuffle the deck
            dealCards(player1Hand, handSize1, index); // Deal cards to Player 1
            dealCards(player2Hand, handSize2, index); // Deal cards to Player 2

            // Initialize discard pile
            do 
            {
                discardPile[++discardTop] = deck[index / numtypes][index % numtypes];
                index++;
            } while (isSpecialCard(discardPile[discardTop])); // Ensure the top card is not special

            // Main game loop
            while (!gameWon) 
            {
                clearScreen();
                if (currentplayer == 1) 
                {
                    // Player 1's turn
                    cout << "Player 1's Turn: " << endl;
                    displayHand(player1Hand, handSize1);
                    playTurn(player1Hand, handSize1, gameWon, index);
                } 
                else 
                {
                    // Player 2's turn
                    cout << "Player 2's Turn: " << endl;
                    displayHand(player2Hand, handSize2);
                    playTurn(player2Hand, handSize2, gameWon, index);
                }

                // Check for win condition
                if (handSize1 == 0) 
                {
                    cout << "Player 1 Wins!!!!" << endl;
                    cout << "Congratulations!!!!!!" << endl;
                    logGameResult("Player 1 Wins");
                    gameWon = true;
                } 
                else if (handSize2 == 0) 
                {
                    cout << "Player 2 Wins!!!!" << endl;
                    cout << "Congratulations!!!!!!" << endl;
                    logGameResult("Player 2 Wins");
                    gameWon = true;
                }

                // Switch player turn
                if (!gameWon) 
                {
                    currentplayer = (currentplayer % 2) + 1;
                }
            }
        } 
        else if (choice == '2') 
        {
            // Display score history
            clearScreen();
            loadGameHistory();
        } 
        else if (choice == '3') 
        {
            // Exit the game
            cout << "Exiting the Game. GOODBYE!" << endl;
            break;
        } 
        else 
        {
            // Handle invalid input
            cout << "Invalid Choice. Enter Your Choice Again." << endl;
            goto label;
        }
    }
    return 0;
}/*
void displaySpecialCards(int deck[][numtypes]) 
{
    cout << "Special Cards in the Deck:\n";

    for (int i = 0; i < colors; i++) {
        for (int j = 0; j < numtypes; j++) 
		{// Extract the card type
            if (j >=10 &&j<16) 
			{
            	int card=i*100+j;
                printBoard(card); // Display the card
            }
        }
    }
}*/
void initializeDeck(int deck[][numtypes]) //initializing deck
{
    for (int i=0;i<colors;i++) 
	{
        for (int j=0;j<numtypes;j++) 
		{
            deck[i][j]=i*100+j;
            cout<<deck[i][j];
        }
    }
}

void shuffleDeck(int deck[][numtypes]) //shuffling of deck
{
    srand(time(0));
    for (int i=0;i<colors;i++) 
	{
        for (int j=0;j<numtypes;j++) 
		{
            int randcolor;
            int randtype;
            int temp;
            randtype=rand()%numtypes;
            temp=deck[i][j];
            randcolor=rand()%colors;
            deck[i][j]=deck[randcolor][randtype];
            deck[randcolor][randtype]=temp;
        }
    }
}

void dealCards(int playerHand[], int &handSize, int &index) //function to deal the card during each turn
{
    for (int i=0;i<handSize; i++) 
	{
        playerHand[i]=deck[index/numtypes][index%numtypes];
        index++;
    }
}

void clearScreen() //clearing screen
{
    system("cls");
}

void displayHand(int playerHand[], int handsize) //diplaying the hand of player
{
    for (int i=0;i<handsize;i++) 
	{
        cout<<"Card "<<i+1<< ": "<<endl;
        printBoard(playerHand[i]);
    }
}

void printBoard(int card,int chosencolor) //printing of card
{
    int color;
    int value;
    color=(chosencolor==-1)?card/100:chosencolor;
    value=card % 100;
    switch (color) 
	{
        case 0: 
		{
			setColor(BLACK,RED); 
			break;   
		}
        case 1: 
		{
			setColor(BLACK,BLUE); 
			break;  
		}
        case 2: 
		{
			setColor(BLACK,GREEN); 
			break;
		}
        case 3: 
		{
			setColor(BLACK,YELLOW); 
			break;
		}
        case 4: 
		{
			setColor(WHITE,BLACK); 
			break; 
		}
        default: 
		{
			setColor(WHITE, BLACK); 
			break;
		}
    }
	if (value==13||value==14)
	{
		setColor(BLACK,WHITE);
	}
    cout<<" --------- "<<endl;
    cout<<"|         |"<<endl;
    if (value<=9&&value>=0)
    {
        cout<<"|    "<<value<<"    |"<<endl;
    }
    else if (value==10)
    {
        cout<<"|   Skip  |"<<endl;
    }
    else if (value==11)
    {
        cout<<"| Reverse |"<<endl;
    }
    else if (value==12)
    {
        cout<<"|  Draw 2 |"<<endl;
    }
    else if (value==13)
    {
        cout<<"|   WILD  |"<<endl;
    }
    else if (value==14)
    {
        cout<<"| WILD +4 |"<<endl;
    }
    cout<<"|         |"<<endl;
    cout<<" --------- "<<endl;
    setColor(WHITE, BLACK);
}
bool isSpecialCard(int card) //special card function
{
    int value;
    bool special=false;
    value=card%100;
    if (value>=10&&value<=14)
    {
    	special=true;
	}
    return  special;
}

void playTurn(int playerHand[], int &handSize, bool &gameWon, int &index) 
{
	int choice;
    cout<<"Top Card in Discard Pile: "<<endl;
    printBoard(discardPile[discardTop]);
    label:
    cout<<endl<<"Choose a Card to Play (1 to "<<handSize<<"), or Enter 0 to Draw a Card: ";
    cin>>choice;

    if (choice==0) 
	{
        drawCard(playerHand, handSize,index);
    } 
	else if (choice>0&&choice<=handSize) 
	{
        int selectedcard=playerHand[choice-1];
        if (isValidPlay(selectedcard, discardPile[discardTop])) 
		{
            discardPile[++discardTop]=selectedcard;
            for (int i=choice-1;i<handSize-1;i++) 
			{
                playerHand[i]=playerHand[i+1];
            }
            handSize--;
            if ((currentplayer==1&&handSize==2)||(currentplayer==2&&handSize==2)) 
			{
				bool unocall;
				unocall=unoCall(currentplayer, handSize1, handSize2);
                if (!unocall) 
				{
                    cout<<"Penalty: "<<(currentplayer==1?"Player 1":"Player 2")<<" Got Two Cards!!!!"<<endl;
                    if (currentplayer==1) 
					{
                        drawTwoCards(player1Hand,handSize1,index);
                    } 
					else 
					{
                        drawTwoCards(player2Hand, handSize2,index);
                    }
                }
            }
            handleSpecialCard(selectedcard%100,gameWon,index,playerHand,handSize);
            if (handSize==0) 
			{
                gameWon=true;
                if (currentplayer==1) 
				{
                    cout<<"Player 1 Wins!"<<endl;
                } 
				else 
				{
                    cout<<"Player 2 Wins!"<<endl;
                }
        	}
            char choice1;
            label1:
            cout<<"Do You Want to Exit the GAme (Y/N)? ";
            cin>>choice1;
            if (choice1=='Y') 
			{
				clearScreen();
                main();
            } 
			else if (choice1=='N') 
			{
                cout<<"Game Continues!!!!"<<endl;
            }
			else 
			{
                cout<<"Invalid Choice!Please Try Again."<<endl;
                goto label1;
            }

        } 
		else 
		{
            cout<<"Invalid Move! You Cannot Play that Card."<<endl;
            goto label;
        }
    } 
	else 
	{
        cout<<"Invalid Choice! Please try again.\n";
        goto label;
    }
}

bool isValidPlay(int playercard,int topcard) //the user is enter write choice or not
{
    int playercolor;
    int playervalue;
    int topcolor;
    int topvalue;
    bool result=false;
    
    playercolor=playercard/100;
    playervalue=playercard%100;
    topcolor=topcard/100;
    topvalue=topcard%100;
    if (playervalue==13||playervalue==14) 
	{
		result=true;
    }
    else if (playercolor==topcolor||playervalue==topvalue)
    {
		result=true;
	}
    return result;
}

void drawCard(int playerHand[], int &handSize, int &deckIndex) 
{
    if (deckIndex<totalcards)//one card draw 
	{
        playerHand[handSize]=deck[deckIndex/numtypes][deckIndex%numtypes];
        handSize++;
        deckIndex++;
    } 
	else 
	{
        cout<<"No more cards left to draw!"<<endl;//empty deck
    }
}

void handleSpecialCard(int specialCardType,bool &gameWon,int &index,int playerhand[],int &handsize) 
{
    switch (specialCardType) 
	{
        case 10: //skip card
        {
            skipTurn();
            break;
        }
        case 11: //reverse card
        {
            reverseTurn();
            break;
        }
        case 12: //draw 2 card
        {
        	skipTurn();
            drawTwoCards(playerhand,handsize,index);
            break;
        }
        case 13://wild card
		{
			selectColor();
			break;
		}
        case 14://wild draw 4 card
    	{
            applyWildCardEffect(specialCardType,gameWon,playerhand,handsize,index);
            break;
        }
    }
}

void skipTurn() //skipping the turn
{
    currentplayer=(currentplayer%2)+1;
}

void reverseTurn()//reverse the turn 
{
	currentplayer=(currentplayer%2)+1;
}
void drawTwoCards(int playerhand[],int &handsize,int &index) //draw two cards 
{
    if (currentplayer==1) //if the turn is of player 1
	{
        for (int i=0;i<2;i++) 
		{
            player1Hand[handSize1]=deck[index/numtypes][index%numtypes];//increase of 2 cards
            handSize1++;
            index++;
        }
    } 
	else 
	{
        for (int i=0;i<2;i++) //if the turn is of player 2
		{
            player2Hand[handSize2]=deck[index/numtypes][index%numtypes];//increase of 2 cards
            handSize2++;
            index++;
        }
    }
}

void applyWildCardEffect(int wildType, bool &gameWon, int playerhand[], int &handsize, int &index) //both wild and wild draw 4 card function
{
    int chosencolor;
    chosencolor=selectColor();
    discardPile[discardTop]=chosencolor*100+wildType;
	
    if (wildType==14) //wild draw 4 card functionality
	{ 
   		skipTurn();
   		drawTwoCards(playerhand,handsize,index);
   		drawTwoCards(playerhand,handsize,index);	
		}    
    }
    
int selectColor() 
{//color selection after playing wild card
    int colorchoice;
    cout<<"Choose a color: "<<endl;
    cout<<"1. Red"<<endl;
    cout<<"2. Blue"<<endl;
    cout<<"3. Green"<<endl;
    cout<<"4. Yellow"<<endl;
    cout<<"Enter your choice: ";
    cin>>colorchoice;
    return colorchoice-1;
}
bool unoCall(int currentplayer,int handSize1,int handSize2) 
{
    char unoChoice;
    if ((currentplayer==1&&handSize1==2)||(currentplayer==2&&handSize2==2)) 
	{
        cout<<"You Have 2 Cards Left! Do You Want to Call UNO? (Y/N): ";//uno call when a player has 2 cards left
        cin>>unoChoice;

        if (unoChoice=='Y'||unoChoice=='y') 
		{
            cout<<"UNO called successfully!"<<endl;
            return true; 
        } 
		else //not called uno
		{
            cout<<"You did not call UNO!"<<endl;
            return false; 
        }
    }
    
    return true;
}
void logGameResult(const string &result) //function used to save game history
{
    ofstream file("game_log.txt", ios::app);
    if (file.is_open()) 
	{
        file<<result<<endl; 
        file.close();
    } 
	else 
	{
        cout<<"Failed to Open Game log File!"<<endl;
    }

    cout<<"Press Any Key to Continue..."<<endl;
    getch(); 
}
void loadGameHistory() //function use to show the game history
{
    ifstream file("game_log.txt");
    if (file.is_open()) 
	{
        cout<<endl<<"============================== GAME HISTORY =============================="<<endl<<endl;
        string line;
        while (getline(file, line)) 
		{
            cout<<"                             "<<line<<endl; 
        }
        file.close();
    } 
	else 
	{
        cout<<"No Game History Found!!!!"<<endl;
    }

    cout<<endl<<"Press any key to go back to the main menu..."<<endl;
    getch();
    clearScreen();
}
