// MineSweeper.cpp: Okreœla punkt wejœcia dla aplikacji konsoli.
//

#include "stdafx.h"


const int sizex = 8, sizey = 8, minescount = 10;
const char COVERED = 'X';
const char UNCOVERED = ' ';
const char FLAG = 'F';

class GameState {
	protected:
		int actualState; // 0 - Brak gry, 1 - W trakcie, 2 - Wygrana, 3 - GameOver.


	public:
		GameState() {
			this->actualState = 0;
		}

		void setGameState(int newState) {
			this->actualState = newState;
		}

		int getGameState() {
			return this->actualState;
		};
};

class Menu {

public:
	int GetInput()
	{
		int choice;
		std::cout << std::endl << "Make your choice: ";
		std::cin >> choice;
		return choice;
	}
	void displayMenu() {
		std::cout << "\n\n\n\t\t===========================================" << std::endl;
		std::cout << std::endl << "\t\t\t\t MINESWEEPER \t\t\t\t" << std::endl <<std::endl;
		std::cout << "\t\t\tCreated By Wiktor Morawski" << std::endl;
		std::cout << "\t\t\tIndex Number: 430692" << std::endl;
		std::cout << "\t\t===========================================" << std::endl << std::endl;

		std::cout << "\t\t1 - New Game" << std::endl;
		std::cout << "\t\t2 - Scores" << std::endl;
		std::cout << "\t\t3 - Exit Game" << std::endl;

	}
};
class Score {
private:
	std::ofstream out;
	std::ifstream scoreFile;
	int numberOfScores;
	int *scores;
	int playerScore;
public:
	 Score() {
		playerScore = 0;
		out.open("scores.txt", std::ios::app);
		out.close();
		numberOfScores = countScores();
		scores = new int[numberOfScores];
	}
	 ~Score() {
		 delete[] scores;
	}
	 bool isFileEmpty() {
		 bool result;

		 countScores() == 0 ? result = true : result = false;
		 return result;
	 }
	 int countScores() {
		 std::ifstream ifs("scores.txt");
		 int words = 0;
		 std::string word;	
		 while (ifs >> word) {
			 ++words;
		 }
		 return words;
	 }
	 int* getScores() {
		 std::ifstream ifs("scores.txt");
		 if (ifs.is_open())
		 {

			 for (int i = 0; i < countScores(); ++i)
			 {
				 ifs >> scores[i];
			 }
		 }
		 std::sort(scores, scores + countScores(), std::greater<int>());
		 return scores;
	 }
	 void increaseScore() {
		 playerScore += 1;
	 }
	 void saveScore() {
		 out.open("scores.txt", std::ios::app);
		 out << playerScore << std::endl;
		 out.close();
	 }

};

class Field {
private: 
	bool covered;
	char sign;
	int posX;
	int posY;

public:

	bool isCovered() { return covered; }
	char getSign() { return sign; }
};
class Safe : public Field {
private:
	char sign;

public:
	Safe() : sign(' ') {};
};
class Mine : public Field {
private:
	char sign;
public:
	Mine() : sign('B') {};
};
class Board {
protected:
	int sizeX;
	int sizeY;
	int minesCount;


public:
	Field* playingBoard = new Field[sizex];
	~Board() {
		delete[] playingBoard;
	}
	Board(int sizeX, int sizeY, int minesCount) : sizeX(sizeX), sizeY(sizeY), minesCount(minesCount) {

		for (int i = 0; i < sizex; ++i) {
			playingBoard[i] = *new Field[sizeY];
		}
	}
	void spacer(int spaces) {
		for (int i = 0; i < spaces; ++i) {
			std::cout << "\t";
		}
	}
	void drawBoard() {
		std::cout << " _";
		for (int i = 0; i < sizex; i++)
		{
			std::cout << i << "_";
		}
		std::cout << std::endl;
		for (int y = 0; y < sizey; y++)
		{
			std::cout << y << "|";
			for (int x = 0; x < sizex; x++)
			{
				std::cout << "_|";
				//if (board[x][y].getSign() == 'F')
				//{
				//	std::cout << "!|";
				//}
				//if (board[x][y].getSign() == 'M')
				//{
				//	std::cout << "b|";
				//}
				//else
				//{
				//	std::cout << "_|";
				//}
			}
			std::cout << std::endl;
		}
	}



};
int main()
{
	int choice = 0;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 10);
	Score scores;
	Menu mainMenu;
	GameState state;
	Board board = Board(sizex,sizey,minescount);

	do {

		mainMenu.displayMenu();
		choice = mainMenu.GetInput();
		switch (choice) {
		case 1:
			system("cls");

			state.setGameState(1);
			std::cout << "\t\t\t\tWelcome to The Game" << std::endl;

			for (int i = 0; i < sizex; ++i) {
				for (int j = 0; j < sizey; ++j) {
					board.playingBoard[i][j];
				}
			}
			//board.drawBoard();

				break;
		case 2:
			system("cls");
			if (scores.isFileEmpty()) {
				std::cout << "\n\n\n\t\t\t BRAK WYNIKOW \n\n\n" << std::endl;

			}
			else {
				scores.countScores();
				std::cout << "\n\t\t\t--- Najlepsze wyniki --- " << std::endl;
				for (int i = 0; i < scores.countScores(); ++i) {
					std::cout << "\t\t\t\t"<< i + 1<< ". " << scores.getScores()[i] << std::endl;
				}
			}
			break;
		case 3:
			system("cls");
			state.setGameState(0);
			std::cout << "\n\t\t\t\tGood Bye" << std::endl;

			break;


		default:
			break;
		}
	} while (choice != 3 && state.getGameState() == 0 );


	return EXIT_SUCCESS;
}

