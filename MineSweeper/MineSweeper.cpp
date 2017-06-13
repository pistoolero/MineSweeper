// MineSweeper.cpp: Określa punkt wejścia dla aplikacji konsoli.
//

#include "stdafx.h"


const int sizex = 8, sizey = 8, minescount = 10;
const double START_MULTIPLER = 1.8;
const double INC_MULTIPLER = 1.02;
int covered = sizex * sizey;
bool isOnMap(int row, int col)
{
	if ((row >= 0) && (row < sizey) &&
		(col >= 0) && (col < sizex)) {
		return true;
	}
	return false;
}
class GameState {
	protected:
		int actualState; // 0 - Brak gry, 1 - W trakcie, 2 - Wygrana, 3 - GameOver.


	public:
		GameState() {
			actualState = 0;
		}

		void setGameState(int newState) {
			actualState = newState;
		}

		int getGameState() {
			return actualState;
		};
		void blankLine() {
			std::cout << "\t\t\t|\t\t\t\t\t|" << std::endl;
		}
		void loseMessage(HANDLE &hconsole, int score) {
			SetConsoleTextAttribute(hconsole, 12);
			std::cout << std::endl;
			std::cout << "\t\t\t ---------------------------------------" << std::endl;
			blankLine();
			blankLine();
			std::cout << "\t\t\t|\t\t PRZEGRALES\t\t|" << std::endl;
			blankLine();
			std::cout << "\t\t\t|\t TWOJ WYNIK TO " << score << "\t\t|" << std::endl;
			std::cout << "\t\t\t ---------------------------------------" << std::endl;
			std::cout << std::endl << std::endl;

		}
		void winMessage(HANDLE &hconsole, int score) {
			SetConsoleTextAttribute(hconsole, 10);
			std::cout << std::endl;
			std::cout << "\t\t\t ---------------------------------------" << std::endl;
			blankLine();
			blankLine();
			std::cout << "\t\t\t|\t\t WYGRALES\t\t|" << std::endl;
			blankLine();
			std::cout << "\t\t\t|\t TWOJ WYNIK TO " << score << "\t\t|" << std::endl;
			std::cout << "\t\t\t ---------------------------------------" << std::endl;
			std::cout << std::endl << std::endl;
		}
		void restartGame() {
			system("cls");
			setGameState(0);
		}
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
	std::string GetShortDecision() {
		std::string choice;
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
	double playerScore;
	double multipler;

public:
	 Score() {
		multipler = START_MULTIPLER;
		playerScore = 0;
		out.open("scores.txt", std::ios::app);
		out.close();
		numberOfScores = countScores();

		scores = new int[numberOfScores + 1];
	}
	 ~Score() {
		 delete[] scores;
	}
	 bool isFileEmpty() {
		 bool result;
		 countScores() == 0 ? result = true : result = false;
		 return result;
	 }
	 void increaseMultipler() {
		 multipler *= INC_MULTIPLER;
	 }
	 int countScores() {
		 std::ifstream ifs("scores.txt");
		 int words = 0;
		 std::string word;	
		 while (ifs >> word) {
			 ++words;
		 }
		 ifs.close();

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
		 ifs.close();
		
		 return scores;
	 }
	 void increaseScore() {
		 --covered;
		 playerScore += 1 * multipler;
		 increaseMultipler();
	 }
	 void saveScore() {
		 out.open("scores.txt", std::ios::app);
		 out << (int)playerScore << std::endl;
		 out.close();
	 }
	 float getPlayerScore() { return playerScore; }
	 void setPlayerScore(float x) { playerScore = x; };
	 void setMultipler(float x) { multipler = x; }
};

class Field {
protected:
	std::string sign;
	bool isBomb;
	bool isCovered;
	bool isFlag;
	bool giveHealth;
	int posX;
	int posY;

public:
	Field() {

		sign = "[ ]";
		isBomb = false;
		isCovered = true;
		giveHealth = false;
		isFlag = false;

	}
	Field(int posX, int posY) : posX(posX), posY(posY) {};

	void setCoverState(bool newState) {
		
		isCovered = newState;
	}
	bool getCoverState() {return isCovered;}

	std::string getSign() {
		return sign;
	}
	void setSign(std::string _sign) {
		
		sign = _sign;
	}
	bool getBombState() {
		return isBomb;
	}
	bool givesHealth() {
		return giveHealth;
	}
	bool isFlagged() {
		return isFlag;
	}
	virtual ~Field() {

	}

};

class Mine : public Field {
public:
	Mine(int _posX, int _posY) {

		sign = "[M]";
		isBomb = true;
		posX = _posX;
		posY = _posY;
	}
	~Mine() {

	}

};
class AdditionalHealth : public Field {
public:
	AdditionalHealth(int _posX, int _posY) {
		sign = "[+]";
		isBomb = false; // Nigdy nie mo¿e byæ bomb¹;
		posX = _posX;
		posY = _posY;
		giveHealth = true;
	}
	~AdditionalHealth() {

	}

};
class Flag : public Field {
public:
	Flag(int _posX, int _posY) {
		sign = "[F]";
		posX = _posX;
		posY = _posY;
	}
};

class Board {
protected:
	int sizeX;
	int sizeY;
	int minesCount;


public:
	Board() {

	};
	Board(int _sizeX, int _sizeY, int _minesCount) {
		sizeX = _sizeX;
		sizeY = _sizeY;
		minesCount = _minesCount;

	}

	void drawBoard(Field *fields[][sizey]) {
		std::cout << "\t";
		for (int i = 0; i < sizex; ++i) {
			std::cout << " " << i << "\t";
		}
		std::cout << std::endl << std::endl << std::endl;
		for (int y = 0; y < sizey; y++) {
			std::cout << y << "\t";
			for (int x = 0; x < sizex; x++) {
				if (fields[x][y]->getCoverState()) {
					std::cout << "[ ]\t";
				}
				else {
					std::cout << fields[x][y]->getSign() << "\t";
				}

			}
			std::cout << std::endl << std::endl;
		}
	}
	void spacer(int spaces) {
		for (int i = 0; i < spaces; ++i) {
			std::cout << "\t";
		}
	}

	int countAdjacentMines(int row, int col, Field *fields[][sizey]) {

		/*
		Zlicza sąsiednie bomby

		N.W   N    N.E
		  \    |    /
		   \   |   /
		W----Cell----E
		   /   |  \
		  /    |   \
		S.W    S   S.E

		Cell-->Current Cell (row, col)
		N -->  North        (row-1, col)
		S -->  South        (row+1, col)
		E -->  East         (row, col+1)
		W -->  West            (row, col-1)
		N.E--> North-East   (row-1, col+1)
		N.W--> North-West   (row-1, col-1)
		S.E--> South-East   (row+1, col+1)
		S.W--> South-West   (row+1, col-1)
		*/

		int count = 0;
		if (isOnMap(row - 1, col)) {
			if (fields[row - 1][col]->getBombState()) {
				count++;
			}

		}
		if (isOnMap(row + 1, col)) {
			if (fields[row + 1][col]->getBombState()) {
				count++;
			}

		}
		if (isOnMap(row , col + 1)) {
			if (fields[row][col + 1]->getBombState()) {
				count++;
			}

		}
		if (isOnMap(row, col - 1)) {
			if (fields[row][col - 1]->getBombState()) {
				count++;
			}

		}
		if (isOnMap(row -1, col + 1)) {
			if (fields[row - 1][col + 1]->getBombState()) {
				count++;
			}

		}
		if (isOnMap(row - 1, col - 1)) {
			if (fields[row - 1][col - 1]->getBombState()) {
				count++;
			}

		}
		if (isOnMap(row + 1, col + 1)) {
			if (fields[row + 1][col + 1]->getBombState()) {
				count++;
			}

		}
		if (isOnMap(row + 1, col - 1)) {
			if (fields[row + 1][col - 1]->getBombState()) {
				count++;
			}

		}

		return count;
	}

};

class Player {
private:
	int livesCount;
public:
	Player() : livesCount(1) {};

	int getLives() { return livesCount; }
	void increaseLives() { livesCount += 1; }
	void decreaseLives() { livesCount -= 1; }
	int moveX() {
		int x;
		std::cin >> x;
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore();
			
		}
		return x;
	}
	int moveY() {
		int y;
		std::cin >> y;
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore();

		}
		return y;
	}
	bool isValidMove(int x, int y) {
		if (x < 0 || x > sizex - 1 || y < 0 || y > sizey - 1) {
			return false;
		}
		else {
			return true;
		}
	}
};



int main()
{
	srand(time(NULL));
	int choice = 0;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 11);
	Score scores;
	Menu mainMenu;
	GameState state;
	Field *fields[sizex][sizey];
	Player player;
	Board board;
	char flag;
	int x, y, ahxpos, ahypos;

	do {
		covered = sizex * sizey;
		mainMenu.displayMenu();
		choice = mainMenu.GetInput();
		switch (choice) {
		case 1:
			ahxpos = rand() % sizex;
			ahypos = rand() % sizey;
			system("cls");
			state.setGameState(1);
			for (int i = 0; i < sizex; ++i) {
				for (int j = 0; j < sizey; ++j) {
					fields[i][j] = new Field();
				}
			}
			for (int i = 0; i < minescount; i++)
			{

 				int xpos = rand() % sizex;
				int ypos = rand() % sizey;

				if (!fields[xpos][ypos]->getBombState()) {
					delete fields[xpos][ypos];
					fields[xpos][ypos] = new Mine(xpos, ypos);
				}
				else
				{
					i--;
				}
			}

			while (fields[ahxpos][ahypos]->getBombState()) {
				ahxpos = rand() % sizex;
				ahypos = rand() % sizey;
			}
			delete fields[ahxpos][ahypos];
			fields[ahxpos][ahypos] = new AdditionalHealth(ahxpos, ahypos);
			std::cout << std::endl;
			do {
				system("cls");
				std::cout << "\t\t\t\tWelcome to The Game" << std::endl;
				std::cout << std::endl;
				std::cout << ahxpos << ahypos << std::endl;
				board.spacer(3);
				SetConsoleTextAttribute(hConsole, 10);
				std::cout << "Ilosc zyc: " << player.getLives();
				board.spacer(2);
				SetConsoleTextAttribute(hConsole, 12);
				std::cout << "Wynik: " << (int)scores.getPlayerScore() << "\t covered(" << covered  << ") minescount(" << minescount << ")" <<std::endl << std::endl;
				SetConsoleTextAttribute(hConsole, 11);
				
				board.drawBoard(fields);
				
				std::cout << std::endl << "Podaj Koordynaty w formacie KOLUMNA RZAD: ";
				x = player.moveX();
				y = player.moveY();
				//flag = player.setFlag();

				if (player.isValidMove(x, y)) {

					if (fields[x][y]->getBombState()) {
						player.decreaseLives();
					}
					else {
							
							if (fields[x][y]->getCoverState() && fields[x][y]->givesHealth()) {
								player.increaseLives();

							}

						int count = board.countAdjacentMines(x, y, fields);
						std::string state;
						if (count > 0) {
							state = " " + std::to_string(count) + " ";
							if (fields[x][y]->getCoverState()) {
								scores.increaseScore();
								fields[x][y]->setCoverState(false);
							}

						}
						else {
							state = "   ";
							for (int i = x - 1; i < x + 2; ++i) {
								for (int j = y - 1; j < y + 2; ++j) {
									if (isOnMap(i, j)) {
										int c = board.countAdjacentMines(i, j, fields);
										if (c == 0) {									
											if (i != x && j != y && fields[i][j]->getCoverState()) {
												
												fields[i][j]->setSign("   ");
												fields[i][j]->setCoverState(false);
											}
											else if (fields[i][j]->getCoverState() == true && !fields[x][y]->givesHealth()) {
												fields[i][j]->setSign("   ");
												fields[i][j]->setCoverState(false);
												scores.increaseScore();
											}

										}
									}
								}
							}
							
						}
						if (!fields[x][y]->givesHealth()) {
							
							fields[x][y]->setSign(state);
						}

					}
					fields[x][y]->setCoverState(false);
				}
			} while (player.getLives() > 0 && covered > minescount);
			system("cls");
			for (int i = 0; i < sizex; i++) {
				for (int j = 0; j < sizey; j++) {
					if (!fields[i][j]->getBombState() && !fields[i][j]->givesHealth()) {
					int count = board.countAdjacentMines(i, j, fields);
					std::string state;
					if (count > 0) {
						state = " " + std::to_string(count) + " ";
					}
					else {
						state = "   ";
					}
					
						fields[i][j]->setSign(state);
					}
					fields[i][j]->setCoverState(false);
				}
			}
			std::cout << std::endl << std::endl;
			board.drawBoard(fields);
			for (int i = 0; i < sizex; i++) {
				for (int j = 0; j < sizey; j++) {
					delete fields[i][j];
				}
			}

			scores.saveScore();
			if (player.getLives() == 0) {
				
				state.loseMessage(hConsole, scores.getPlayerScore());

			}
			else if (covered <= minescount) {
				
				state.winMessage(hConsole, scores.getPlayerScore());
			}
			std::cout << "Chcesz wrocic do menu? t / n: ";
			if (mainMenu.GetShortDecision() == "t") {
				SetConsoleTextAttribute(hConsole, 11);
				player.increaseLives();
				scores.setPlayerScore(0);
				scores.setMultipler(START_MULTIPLER);
				state.restartGame();

			}
			else {
				continue;
			}
			break;
		case 2:
			system("cls");
			if (scores.isFileEmpty()) {
				std::cout << "\n\n\n\t\t\t BRAK WYNIKOW \n\n\n" << std::endl;

			}
			else {
				scores.countScores();
				std::cout << "\n\t\t\t--- Wszystkie wyniki --- " << std::endl;
				for (int i = 0; i < scores.countScores(); ++i) {
					if(scores.getScores()[i] > 0)
					std::cout << "\t\t\t\t"<< i + 1<< ". " << scores.getScores()[i] << std::endl;
				}
			}
			
			break;
		case 3:

			state.setGameState(0);
			std::cout << "\n\t\t\t\tGood Bye" << std::endl;
			break;


		default:
			break;
		}
	} while (choice != 3 && state.getGameState() == 0 );

	return EXIT_SUCCESS;
}

