#pragma once
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include <fstream>
#include <string>
#include <vector>

class Tile {
public:

	//int numBombPoint;
	sf::Sprite HiddenTile;
	sf::Sprite Flag;
	sf::Sprite RevealedTile;
	sf::Sprite Mines;
	sf::Sprite NumTile;
	std::vector<Tile*> neighbors;

	bool isBomb;
	bool youLose;
	bool isRevealed;
	bool isFlagged;
	bool isDebug;
	bool isPaused;
	int numBombs;
	pair<int, int> coord;

	void bombStage();

	Tile(float xpos, float ypos);
	Tile(const Tile& other);
	const Tile& operator=(const Tile& other);
	void draw(sf::RenderWindow& window);
	bool toggleFlag();
	//void CreateBomb(bool bombState);
	void toggleDebug();
	bool reveal(int& nearBomb);

	void tileClicked(Tile tiles);

};

Tile::Tile(float xpos, float ypos) {
	coord = { xpos / 32, ypos / 32 };
	HiddenTile.setPosition(xpos, ypos);
	RevealedTile.setPosition(xpos, ypos);
	Flag.setPosition(xpos, ypos);
	NumTile.setPosition(xpos, ypos);
	Mines.setPosition(xpos, ypos);

	sf::Texture& tile_hiddenTexture = TextureManager::getTexture("tile_hidden");
	HiddenTile.setTexture(tile_hiddenTexture);

	sf::Texture& tile_revealedTexture = TextureManager::getTexture("tile_revealed");
	RevealedTile.setTexture(tile_revealedTexture);

	sf::Texture& flagTexture = TextureManager::getTexture("flag");
	Flag.setTexture(flagTexture);

	sf::Texture& mineTexture = TextureManager::getTexture("mine");
	Mines.setTexture(mineTexture);


	//bombNum = 0;
	isBomb = false;
	isRevealed = false;
	isFlagged = false;
	isDebug = false;
	youLose = false;
}

const Tile& Tile::operator=(const Tile& other) {
	coord = other.coord;
	HiddenTile.setPosition(other.HiddenTile.getPosition());
	RevealedTile.setPosition(other.HiddenTile.getPosition());
	Flag.setPosition(other.HiddenTile.getPosition());
	NumTile.setPosition(other.HiddenTile.getPosition());
	Mines.setPosition(other.HiddenTile.getPosition());

	sf::Texture& tile_hiddenTexture = TextureManager::getTexture("tile_hidden");
	HiddenTile.setTexture(tile_hiddenTexture);

	sf::Texture& tile_revealedTexture = TextureManager::getTexture("tile_revealed");
	RevealedTile.setTexture(tile_revealedTexture);

	sf::Texture& flagTexture = TextureManager::getTexture("flag");
	Flag.setTexture(flagTexture);

	sf::Texture& mineTexture = TextureManager::getTexture("mine");
	Mines.setTexture(mineTexture);


	//bombNum = 0;
	isBomb = false;
	isRevealed = false;
	isFlagged = false;
	isDebug = false;
	youLose = false;
	return *this;
}

Tile::Tile(const Tile& other) {
	*this = other;
}

// tile when clicked
void Tile::tileClicked(Tile tiles) {
	if (isFlagged) return;
	isRevealed = true;

	if (isBomb) {
		youLose = true;

	}

	else {
		int count = 0;
		for (int i = 0; i < neighbors.size(); i++) {
			if (neighbors[i] != nullptr && neighbors[i]->isBomb) {
				count++;
			}

		}

		numBombs = count;

		if (count == 0) {
			for (int i = 0; i < neighbors.size(); i++) {
				if (neighbors[i] != nullptr && neighbors[i]->isRevealed != true) {
					neighbors[i]->tileClicked(*neighbors[i]);


				}
			}
		}
	}
}

void Tile::bombStage() {
	int numBombs = 0;

	for (int i = 0; i < 8; i++) {

		if (neighbors[i] != nullptr && neighbors[i]->isBomb) {
			numBombs++;
		}
	}

	if (isBomb == true) {
		sf::Texture& mineTexture = TextureManager::getTexture("mine");
		NumTile.setTexture(mineTexture);
	}

	else {
		if (numBombs == 1) {
			sf::Texture& num1Texture = TextureManager::getTexture("number_1");
			NumTile.setTexture(num1Texture);
		}
		else if (numBombs == 2) {
			sf::Texture& num2Texture = TextureManager::getTexture("number_2");
			NumTile.setTexture(num2Texture);
		}
		else if (numBombs == 3) {
			sf::Texture& num3Texture = TextureManager::getTexture("number_3");
			NumTile.setTexture(num3Texture);
		}
		else if (numBombs == 4) {
			sf::Texture& num4Texture = TextureManager::getTexture("number_4");
			NumTile.setTexture(num4Texture);
		}
		else if (numBombs == 5) {
			sf::Texture& num5Texture = TextureManager::getTexture("number_5");
			NumTile.setTexture(num5Texture);
		}
		else if (numBombs == 6) {
			sf::Texture& num6Texture = TextureManager::getTexture("number_6");
			NumTile.setTexture(num6Texture);
		}
		else if (numBombs == 7) {
			sf::Texture& num7Texture = TextureManager::getTexture("number_7");
			NumTile.setTexture(num7Texture);
		}
		else if (numBombs == 8) {
			sf::Texture& num8Texture = TextureManager::getTexture("number_8");
			NumTile.setTexture(num8Texture);
		}
	}

	isFlagged = false;
	isDebug = false;
	isRevealed = false;

}

bool Tile::reveal(int& nearBomb) {

	if (!isFlagged && !isRevealed) {

		isRevealed = true;

		if (numBombs == 0 && isBomb == false) {

			for (int i = 0; i < 8; i++) {

				if (neighbors[i] != nullptr && !(neighbors[i]->isBomb)) {
					neighbors[i]->reveal(nearBomb);
				}
			}
		}

	}
	return isBomb;
}

bool Tile::toggleFlag() {

	if (!isRevealed) {
		if (isFlagged) {
			isFlagged = false;
		}
		else {
			isFlagged = true;
		}

	}
	return isFlagged;
}



void Tile::toggleDebug() {

	if (isDebug) {
		isDebug = false;

		// delete later
		isRevealed = false;
	}
	else {
		isDebug = true;
	}
}




void Tile::draw(sf::RenderWindow& window) {

	if (!isRevealed) {
		window.draw(HiddenTile);
	}
	else {
		window.draw(RevealedTile);
		if ((numBombs > 0) || isBomb)
			window.draw(NumTile);
	}


	if (isFlagged) {
		window.draw(Flag);

	}

	if (isDebug == true && !isFlagged) {

		if (isBomb == true) {

			window.draw(Mines);
		}
	}

	if (youLose == true) {
		window.draw(Mines);
	}

	if (isPaused == true) {
		window.draw(RevealedTile);
	}
}

