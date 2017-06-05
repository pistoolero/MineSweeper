// MineSweeper.cpp: Okreœla punkt wejœcia dla aplikacji konsoli.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <memory>

const int sizex = 8, sizey = 8, minescount = 10;

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

class Field {
protected:
	char sign;
	bool isBomb;
	bool isCovered;
	bool isFlag;
	bool giveHealth;
	int posX;
	int posY;

public:
	Field() {
		this->sign = '.';
		this->isBomb = false;
		this->isCovered = true;
		this->giveHealth = false;
		this->isFlag = false;
	}

	void setCoverState(bool newState) {
		this->isCovered = newState;
	}

	char getSign() {
		return this->sign;
	}

};

class Mine : public Field {
public:
	Mine(int posX, int posY) {
		this->sign = 'M';
		this->isBomb = true;
		this->posX = posX;
		this->posY = posY;
	}


};
class AdditionalHealth : public Field {
public:
	AdditionalHealth(int posX, int posY) {
		this->sign = 'H';
		this->isBomb = false; // Nigdy nie mo¿e byæ bomb¹;
		this->posX = posX;
		this->posY = posY;
	}
};
class Flag : public Field {
public:
	Flag(int posX, int posY) {
		this->sign = 'F';
		this->posX = posX;
		this->posY = posY;
	}
};

class Board {
protected:
	int sizeX;
	int sizeY;
	int minesCount;


public:
	Board(int sizeX, int sizeY, int minesCount) {
		this->sizeX = sizeX;
		this->sizeY = sizeY;
		this->minesCount = minesCount;

	}


	void spacer(int spaces) {
		for (int i = 0; i < spaces; ++i) {
			std::cout << "\t";
		}
	}

};

void drawBoard(Field board[sizex][sizey]) {
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
			if (board[x][y].getSign() == 'F')
			{
				std::cout << "!|";
			}
			if (board[x][y].getSign() == 'M')
			{
				std::cout << "b|";
			}
			else
			{
				std::cout << "_|";
			}
		}
		std::cout << std::endl;
	}
}

int main()
{
	Board gameboard = Board(sizex, sizey, minescount);
	drawBoard(gameboard);
    return 0;
}

