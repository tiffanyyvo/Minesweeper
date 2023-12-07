#pragma once
#include "Tile.h"
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include <map>
#include <fstream>
#include <iostream>
#include <random>
#include <cstdlib>
#include <chrono>



// Tip: width of board:800
// heigth of board:612
class Board {
	sf::Sprite debugSprite;
	sf::Sprite happyFaceSprite;
	sf::Sprite loseSprite;
	sf::Sprite winSprite;
	int tilesRevealed;
	int numBombs;
	vector<bool> tileIcon;


public:
	int colCount;
	int rowCount;
	int mineCount;
	int flags;
	vector<Tile> tiles;
	void CreateTiles();
	void displayTiles(sf::RenderWindow& window);
	Board();
	void ChangeTiles(sf::Event& event);
	void randomizeTile();
	void Debug();
	void findNeighbors();
	void revealMinesLost();
	bool isOver;
	bool isLost;
	bool leadOpen = false;
	bool reset = false;
	bool isPaused = false;
};


Board::Board() {
	CreateTiles();

	std::ifstream inputFile("board_config.cfg");

	if (inputFile.is_open()) {
		inputFile >> colCount >> rowCount >> mineCount;
		inputFile.close();

	}

	int height = rowCount * 32;
	int width = (colCount * 32) + 100;

	sf::Texture& debugTexture = TextureManager::getTexture("debug");
	debugSprite.setTexture(debugTexture);
	debugSprite.setPosition((colCount * 32) - 304, 32 * (rowCount + 0.5));

	sf::Texture& happyFaceButtonTexture = TextureManager::getTexture("face_happy");
	happyFaceSprite.setTexture(happyFaceButtonTexture);
	happyFaceSprite.setPosition(((colCount / 2.0f) * 32) - 32, 32 * (rowCount + 0.5));

	sf::Texture& winButtonTexture = TextureManager::getTexture("face_win");
	winSprite.setTexture(winButtonTexture);
	winSprite.setPosition(((colCount / 2.0f) * 32) - 32, 32 * (rowCount + 0.5));

	sf::Texture& loseButtonTexture = TextureManager::getTexture("face_lose");
	loseSprite.setTexture(loseButtonTexture);
	loseSprite.setPosition(((colCount / 2.0f) * 32) - 32, 32 * (rowCount + 0.5));
}

void Board::CreateTiles() {
	float row = 0.0;
	float col = 0.0;

	std::ifstream inputFile("board_config.cfg");

	if (inputFile.is_open()) {
		inputFile >> colCount >> rowCount >> mineCount;
		inputFile.close();
	}

	else {
		cout << "Couldnt open file" << endl;
	}

	for (int i = 0; i < rowCount; i++) {
		for (int k = 0; k < colCount; k++) {

			Tile temp = Tile(col, row);
			tiles.push_back(temp);
			col += 32.0;
		}
		col = 0.0;
		row += 32.0;
	}
	randomizeTile();

}

//show tiles
void Board::displayTiles(sf::RenderWindow& window) {

	for (int i = 0; i < tiles.size(); i++) {
		tiles[i].draw(window);
	}
	window.draw(debugSprite);
}

// Finding the neighbors
void Board::findNeighbors() {

	std::ifstream inputFile("board_config.cfg");

	if (inputFile.is_open()) {
		inputFile >> colCount >> rowCount >> mineCount;
		inputFile.close();
	}

	for (int i = 0; i < tiles.size(); i++) {
		tiles[i].neighbors.clear();

		for (int j = 0; j < 8; j++) {
			tiles[i].neighbors.push_back(nullptr);
		}

		if (i == 0) //tile is top left corner
		{
			tiles[i].neighbors[4] = &(tiles[i + 1]);
			tiles[i].neighbors[6] = &(tiles[i + colCount]);
			tiles[i].neighbors[7] = &(tiles[i + colCount + 1]);
		}

		else if (i == colCount - 1) // tile is top right corner
		{
			tiles[i].neighbors[3] = &(tiles[i - 1]);
			tiles[i].neighbors[5] = &(tiles[i + colCount - 1]);
			tiles[i].neighbors[6] = &(tiles[i + colCount]);
		}

		else if (i == (colCount * rowCount) - colCount) // tile is bottom left corner
		{
			tiles[i].neighbors[4] = &(tiles[i + 1]);
			tiles[i].neighbors[1] = &(tiles[i - colCount]);
			tiles[i].neighbors[2] = &(tiles[i - colCount + 1]);
		}

		else if (i == colCount * rowCount - 1) // tile is bottom right corner
		{
			tiles[i].neighbors[3] = &(tiles[i - 1]);
			tiles[i].neighbors[1] = &(tiles[i - colCount]);
			tiles[i].neighbors[0] = &(tiles[i - colCount - 1]);
		}

		else if (i < colCount) // tile is on the top
		{
			tiles[i].neighbors[3] = &(tiles[i - 1]);
			tiles[i].neighbors[4] = &(tiles[i + 1]);
			tiles[i].neighbors[5] = &(tiles[i + colCount - 1]);
			tiles[i].neighbors[6] = &(tiles[i + colCount]);
			tiles[i].neighbors[7] = &(tiles[i + colCount + 1]);
		}

		else if (i > (colCount * rowCount) - colCount) // tile is on the bottom
		{
			tiles[i].neighbors[1] = &(tiles[i - colCount]);
			tiles[i].neighbors[4] = &(tiles[i + 1]);
			tiles[i].neighbors[2] = &(tiles[i - colCount + 1]);
			tiles[i].neighbors[3] = &(tiles[i - 1]);
			tiles[i].neighbors[0] = &(tiles[i - colCount - 1]);
		}

		else if (((i + 1) % colCount) == 0) // tile is on the right edge
		{
			tiles[i].neighbors[0] = &(tiles[(i - colCount) - 1]);
			tiles[i].neighbors[1] = &(tiles[i - colCount]);
			tiles[i].neighbors[3] = &(tiles[i - 1]);
			tiles[i].neighbors[5] = &(tiles[i + (colCount - 1)]);
			tiles[i].neighbors[6] = &(tiles[i + colCount]);
		}


		else if ((i % colCount) == 0) // tile is on the left edge
		{
			tiles[i].neighbors[1] = &(tiles[i - colCount]);
			tiles[i].neighbors[2] = &(tiles[i - colCount + 1]);
			tiles[i].neighbors[4] = &(tiles[i + 1]);
			tiles[i].neighbors[6] = &(tiles[i + colCount]);
			tiles[i].neighbors[7] = &(tiles[i + colCount + 1]);
		}

		else // tile is in the middle
		{
			tiles[i].neighbors[0] = &(tiles[i - colCount - 1]);
			tiles[i].neighbors[1] = &(tiles[i - colCount]);
			tiles[i].neighbors[2] = &(tiles[i - colCount + 1]);
			tiles[i].neighbors[3] = &(tiles[i - 1]);
			tiles[i].neighbors[4] = &(tiles[i + 1]);
			tiles[i].neighbors[5] = &(tiles[i + colCount - 1]);
			tiles[i].neighbors[6] = &(tiles[i + colCount]);
			tiles[i].neighbors[7] = &(tiles[i + colCount + 1]);
		}

		tiles[i].bombStage();
	}
}

//Debug
void Board::Debug() {
	for (int i = 0; i < tiles.size(); i++) {
		tiles[i].toggleDebug();
	}
}

// Showing mines after lose
void Board::revealMinesLost() {
	for (int i = 0; i < tiles.size(); i++) {
		if (tiles[i].isBomb) {
			tiles[i].youLose = true;
		}
	}
}

// sets random tils to bombs
void Board::randomizeTile() {

	tileIcon.clear();
	std::ifstream inputFile("board_config.cfg");

	if (inputFile.is_open()) {
		inputFile >> colCount >> rowCount >> mineCount;
		inputFile.close();

	}
	int numRandom;
	std::srand(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));

	for (int i = 0; i < mineCount; i++) {

		numRandom = rand() % tiles.size();

		if (tiles[numRandom].isBomb == true) {
			i--;

		}
		else if (tiles[numRandom].isBomb == false) {
			tiles[numRandom].isBomb = true;
		}
	}
	findNeighbors();
	tilesRevealed = 0;


}


// when clicked
void Board::ChangeTiles(sf::Event& event) {

	std::ifstream inputFile("board_config.cfg");
	inputFile >> colCount >> rowCount >> mineCount;

	inputFile.close();

	// Toggle Flag
	if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right && !tiles[0].isPaused)
	{
		sf::Vector2i position = { event.mouseButton.x, event.mouseButton.y };

		if (isLost == false && isOver == false) {
			if (position.x >= 0 && position.x <= colCount * 32)
			{
				if (position.y >= 0 && position.y <= rowCount * 32)
				{
					int x = (position.x / 32);
					int y = (position.y / 32);

					int pos = (y * 25) + x;
					if (!tiles[pos].isFlagged && !tiles[pos].isRevealed)
					{
						tiles[pos].toggleFlag();
						flags++;
					}
					else if (!tiles[pos].isRevealed && tiles[pos].isFlagged)
					{
						tiles[pos].toggleFlag();
						flags--;
					}
				}
			}
		}
	}


	if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2i position = { event.mouseButton.x, event.mouseButton.y };

		// Toggle Debug
		if (isLost == false && isOver == false) {
			if (debugSprite.getGlobalBounds().contains(position.x, position.y) && !tiles[0].isPaused) {
				int count = 0;

				int x = (position.x / 32);
				int y = (position.y / 32);

				int pos = (y * 25) + x;

				int tempCount = 0;

				for (int i = 0; i < tiles.size(); i++) {

					if (tiles[i].isBomb) {
						tiles[i].reveal(tempCount);
						tiles[i].toggleDebug();
					}
				}
			}

			// Clicking tile

			if (position.y >= 0 && position.y < rowCount * 32 && !tiles[0].isPaused)
			{
				if (position.x >= 0 && position.x <= colCount * 32)
				{
					int x = (position.x / 32);
					int y = (position.y / 32);
					int count = 0;
					int countBomb = 0;

					int pos = (y * colCount) + x;

					tiles[pos].tileClicked(tiles[pos]);

					// lose
					if (tiles[pos].youLose == true) {
						isLost = true;
						revealMinesLost();

					}

					//win
					for (int i = 0; i < tiles.size(); i++) {
						if (tiles[i].isRevealed == true) {
							count++;
						}
					}

					for (int j = 0; j < tiles.size(); j++) {
						if (tiles[j].isBomb == true) {
							countBomb++;
						}
					}

					if (tiles.size() == count) {
						isOver = true;

						for (int i = 0; i < tiles.size(); i++) {
							if (tiles[i].isBomb == true) {
								tiles[i].isFlagged = true;
							}
							if (tiles[i].isDebug == true) {
								tiles[i].toggleDebug();
							}
						}
						cout << "win" << endl;
					}

					else if (tiles.size() == (count + countBomb)) {
						isOver = true;

						for (int i = 0; i < tiles.size(); i++) {
							if (tiles[i].isBomb == true) {
								tiles[i].isFlagged = true;
							}
							if (tiles[i].isDebug == true) {
								tiles[i].toggleDebug();
							}
						}
						cout << "win" << endl;
					}


				}
			}


			//smile button
			else if (happyFaceSprite.getGlobalBounds().contains(position.x, position.y)) {

				for (int i = 0; i < tiles.size(); i++) {
					tiles[i].isBomb = false;
					tiles[i].isRevealed = false;
					tiles[i].isFlagged = true;
					tiles[i].isDebug = false;
					tiles[i].toggleFlag();
					tiles[i].youLose = false;

				}

				isLost = false;
				isOver = false;
				reset = true;
				flags = 0;
				randomizeTile();

			}
		}

		// when happy face is clicked
		if (happyFaceSprite.getGlobalBounds().contains(position.x, position.y)) {

			for (int i = 0; i < tiles.size(); i++) {
				tiles[i].isBomb = false;
				tiles[i].isRevealed = false;
				tiles[i].isFlagged = true;
				tiles[i].isDebug = false;
				tiles[i].toggleFlag();
				tiles[i].youLose = false;
			}

			isLost = false;
			isOver = false;
			reset = true;
			flags = 0;
			randomizeTile();

		}

	}
}