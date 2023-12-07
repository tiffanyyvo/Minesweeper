#include <iostream>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include "Tile.h"
#include "Board.h"
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

map<int, sf::Sprite> parseDigits(sf::Sprite digits) {
    map<int, sf::Sprite> digitsMap;

    for (int i = 0; i < 10; i++) {
        sf::IntRect rect(i * 21, 0, 21, 32);
        digits.setTextureRect(rect);
        sf::Sprite sprite = digits;
        digitsMap.emplace(i, sprite);
    }
    sf::IntRect rect(210, 0, 21, 32);
    digits.setTextureRect(rect);
    sf::Sprite sprite = digits;
    digitsMap.emplace(10, sprite);

    return digitsMap;
}


int main()
{
    // Reading in board config file
    std::ifstream inputFile("board_config.cfg");
    int colCount, rowCount, mineCount;
    inputFile >> colCount >> rowCount >> mineCount;

    inputFile.close();

    int height = rowCount * 32;
    int width = (colCount * 32) + 100;

    string name;


    // Creating Welcome Window
    sf::RenderWindow window(sf::VideoMode((colCount * 32), (rowCount * 32) + 100), "Welcome!!");

    sf::Font font;
    font.loadFromFile("font.ttf");
    if (!font.loadFromFile("font.ttf")) {
        std::cout << "cant load :(" << std::endl;
        return 0;
    }


    // Welcome Text
    sf::Text welcomeText;
    welcomeText.setString("WELCOME TO MINESWEEPER!");

    welcomeText.setFont(font);
    welcomeText.setCharacterSize(24);
    welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    welcomeText.setFillColor(sf::Color::White);

    sf::FloatRect welcomeTextRect = welcomeText.getLocalBounds();
    welcomeText.setOrigin(welcomeTextRect.left + welcomeTextRect.width / 2.0f, welcomeTextRect.top + welcomeTextRect.height / 2.0f);
    welcomeText.setPosition(sf::Vector2f((colCount * 32) / 2.0f, (((rowCount * 32) + 100) / 2.0f) - 150));



    // Enter Name Text
    sf::Text nameText;
    nameText.setString("Enter your name: ");

    nameText.setFont(font);
    nameText.setCharacterSize(20);
    nameText.setFillColor(sf::Color::White);

    sf::FloatRect nameTextRect = nameText.getLocalBounds();
    nameText.setOrigin(welcomeTextRect.left + nameTextRect.width / 2.0f, nameTextRect.top + nameTextRect.height / 2.0f);
    nameText.setPosition(sf::Vector2f((colCount * 32) / 2.0f, (((rowCount * 32) + 100) / 2.0f) - 75));



    // Input Text
    sf::Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(18);
    inputText.setFillColor(sf::Color::Yellow);

    sf::FloatRect inputTextRect = inputText.getLocalBounds();
    inputText.setOrigin(inputTextRect.left + inputTextRect.width / 2.0f, inputTextRect.top + inputTextRect.height / 2.0f);
    inputText.setPosition(sf::Vector2f((colCount * 32) / 2.0f, (((rowCount * 32) + 100) / 2.0f) - 45));



    // Cursor Text
    sf::Text cursorText;
    cursorText.setFont(font);
    cursorText.setCharacterSize(18);
    cursorText.setFillColor(sf::Color::Yellow);
    cursorText.setString("|");
    bool cursorVisable = true;



    // Play Button
    sf::Texture& playButtonTexture = TextureManager::getTexture("play");
    sf::Sprite playButtonSprite;
    playButtonSprite.setTexture(playButtonTexture);
    playButtonSprite.setPosition((colCount * 32) - 240, 32 * (rowCount + 0.5));

    bool playIsVisable = false;



    // Pause Button
    sf::Texture& pauseButtonTexture = TextureManager::getTexture("pause");
    sf::Sprite pauseButtonSprite;
    pauseButtonSprite.setTexture(pauseButtonTexture);
    pauseButtonSprite.setPosition((colCount * 32) - 240, 32 * (rowCount + 0.5));


    // Debug Button
    sf::Texture& debugButtonTexture = TextureManager::getTexture("debug");
    sf::Sprite debugButtonSprite;
    debugButtonSprite.setTexture(debugButtonTexture);
    debugButtonSprite.setPosition((colCount * 32) - 304, 32 * (rowCount + 0.5));


    // Leaderboard Button
    sf::Texture& leaderboardButtonTexture = TextureManager::getTexture("leaderboard");
    sf::Sprite leaderboardButtonSprite;
    leaderboardButtonSprite.setTexture(leaderboardButtonTexture);
    leaderboardButtonSprite.setPosition((colCount * 32) - 176, 32 * (rowCount + 0.5));

    // Face_happy button
    sf::Texture& happyFaceButtonTexture = TextureManager::getTexture("face_happy");
    sf::Sprite happyFaceButtonSprite;
    happyFaceButtonSprite.setTexture(happyFaceButtonTexture);
    happyFaceButtonSprite.setPosition(((colCount / 2.0f) * 32) - 32, 32 * (rowCount + 0.5));

    // Win happy face
    sf::Texture& winFaceButtonTexture = TextureManager::getTexture("face_win");
    sf::Sprite winFaceButtonSprite;
    winFaceButtonSprite.setTexture(winFaceButtonTexture);
    winFaceButtonSprite.setPosition(((colCount / 2.0f) * 32) - 32, 32 * (rowCount + 0.5));


    // Lose face
    sf::Texture& loseFaceButtonTexture = TextureManager::getTexture("face_lose");
    sf::Sprite loseFaceButtonSprite;
    loseFaceButtonSprite.setTexture(loseFaceButtonTexture);
    loseFaceButtonSprite.setPosition(((colCount / 2.0f) * 32) - 32, 32 * (rowCount + 0.5));


    // Welcome Window
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            }

            if (event.type == sf::Event::KeyPressed && !name.empty()) {
                if (event.key.code == sf::Keyboard::Enter) {
                    window.close();
                }
            }

            if (event.type == sf::Event::TextEntered) { // Getting user keyboard input 

                if (event.text.unicode < 128) {
                    if (event.text.unicode == 8 && !name.empty()) {
                        name.pop_back();
                    }

                    else if (std::isalpha(event.text.unicode) && name.length() < 10) {
                        if (name.empty()) {
                            name += std::toupper(static_cast<char>(event.text.unicode));
                        }
                        else if (event.text.unicode != 8) {
                            name += std::tolower(static_cast<char>(event.text.unicode));
                        }
                    }
                    inputText.setString(name);
                    cursorVisable = true;
                }
            }
        }

        inputText.setOrigin(inputTextRect.left + inputTextRect.width / 2.0f, inputTextRect.top + inputTextRect.height / 2.0f);
        inputText.setPosition((window.getSize().x - inputText.getGlobalBounds().width) / 2, (((rowCount * 32) + 100) / 2.0f) - 45);

        window.clear(sf::Color::Blue);
        window.draw(welcomeText);
        window.draw(nameText);
        window.draw(inputText);

        if (cursorVisable) {
            cursorText.setPosition(inputText.getPosition().x + inputText.getGlobalBounds().width, inputText.getPosition().y);
            window.draw(cursorText);
        }
        window.display();

    }

    vector<vector<int>> vect;
    //    Board* b = new Board;
    //    Board& board = *b;
    Board board;



    // Game Window
    sf::RenderWindow mainWindow(sf::VideoMode(sf::VideoMode((colCount * 32), (rowCount * 32) + 100)), "Main Window");

    //initialize timer
    auto start_time = std::chrono::high_resolution_clock::now();//starting the timer when the program begins to run!
    auto pauseTime = std::chrono::high_resolution_clock::now();
    auto elapsed_paused_time = std::chrono::duration_cast<chrono::seconds>(std::chrono::high_resolution_clock::now() - pauseTime).count();

    bool paused = false; //false when game in not paused, true when the game is paused
    bool leaderPause = false;

    sf::Texture& digitsText = TextureManager::getTexture("digits");
    sf::Sprite digits;
    digits.setTexture(digitsText);

    map<int, sf::Sprite> digitsMap = parseDigits(digits); //map for my digits

    int minutes;
    int seconds;

    //Debug
    bool debug = false;

    for (int i = 0; i < board.tiles.size(); i++) {
        board.tiles[i].isPaused = false;
    }

    while (mainWindow.isOpen())
    {
        sf::Event event;
        while (mainWindow.pollEvent(event) && board.leadOpen == false)
        {
            board.ChangeTiles(event);
            if (event.type == sf::Event::Closed) {
                mainWindow.close();

            }


            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mouse;
                sf::Vector2i vec = sf::Mouse::getPosition(mainWindow);
                mouse = sf::Mouse::getPosition(mainWindow);

                //std::cout << " Mouse button pressed at X: " << mouse.x;
                //std::cout << " Mouse button pressed at Y: " << mouse.y << std::endl;

                if (board.isLost == false && board.isOver == false) {


                    if (pauseButtonSprite.getGlobalBounds().contains(mainWindow.mapPixelToCoords(mouse))) {

                        paused = !paused; //boolean changed everytime pause button is clicked

                        if (paused) {
                            cout << "paused" << endl;
                            pauseTime = chrono::high_resolution_clock::now();

                            for (int i = 0; i < board.tiles.size(); i++) {
                                board.tiles[i].isPaused = true;
                            }

                        }
                        else if (!paused) {
                            //unpaused
                            auto unPausedTime = chrono::high_resolution_clock::now();
                            elapsed_paused_time += chrono::duration_cast<chrono::seconds>(unPausedTime - pauseTime).count(); //Addition is necessary for when hitting the pause button more than once
                            //cout << elapsed_paused_time % 60 << " " << endl;
                            for (int i = 0; i < board.tiles.size(); i++) {
                                board.tiles[i].isPaused = false;
                            }
                        }
                    }
                }

                // If leaderboard is clicked
                if (leaderboardButtonSprite.getGlobalBounds().contains(mainWindow.mapPixelToCoords(mouse))) {
                    bool wasPaused = board.tiles[0].isPaused;
                    for (int i = 0; i < board.tiles.size(); i++) {
                        board.tiles[i].isPaused = true;
                    }


                    if (true) {
                        //"separating" the integers. So.... 68 -> seconds0 = 6 and seconds1 = 8
                        int minutes0 = minutes / 10 % 10; //minutes index 0
                        int minutes1 = minutes % 10; // minutes index 1
                        int seconds0 = seconds / 10 % 10; // seconds index 0
                        int seconds1 = seconds % 10; // seconds index 1


                        digitsMap[minutes0].setPosition((colCount * 32) - 97, 32 * (rowCount + 0.5) + 16);
                        mainWindow.draw(digitsMap[minutes0]);

                        digitsMap[minutes1].setPosition((colCount * 32) - 78, 32 * (rowCount + 0.5) + 16);
                        mainWindow.draw(digitsMap[minutes1]);

                        digitsMap[seconds0].setPosition((colCount * 32) - 54, 32 * (rowCount + 0.5) + 16);
                        mainWindow.draw(digitsMap[seconds0]);

                        digitsMap[seconds1].setPosition((colCount * 32) - 33, 32 * (rowCount + 0.5) + 16);
                        mainWindow.draw(digitsMap[seconds1]);

                        int numFlags = board.mineCount - board.flags;
                        if (numFlags < 0) {
                            numFlags *= -1;
                            digitsMap[10].setPosition(12, 32 * (rowCount + 0.5) + 16);
                            mainWindow.draw(digitsMap[10]);
                        }
                        if (board.isOver) {
                            numFlags = 0;
                        }
                        digitsMap[numFlags / 100].setPosition(33, 32 * (rowCount + 0.5) + 16);
                        mainWindow.draw(digitsMap[numFlags / 100]);

                        digitsMap[numFlags / 10 % 10].setPosition(54, 32 * (rowCount + 0.5) + 16);
                        mainWindow.draw(digitsMap[numFlags / 10 % 10]);

                        digitsMap[numFlags % 10].setPosition(75, 32 * (rowCount + 0.5) + 16);
                        mainWindow.draw(digitsMap[numFlags % 10]);


                        if (paused) {
                            mainWindow.draw(playButtonSprite);
                        }
                        else if (!paused) {
                            mainWindow.draw(pauseButtonSprite);
                        }


                        if (board.isLost == true) {
                            mainWindow.draw(loseFaceButtonSprite);


                        }
                        else if (board.isOver == true) {
                            mainWindow.draw(winFaceButtonSprite);

                        }

                        else {
                            // Happy Face
                            mainWindow.draw(happyFaceButtonSprite);
                        }

                        //print board
                        board.displayTiles(mainWindow);

                        // LeaderBoard
                        mainWindow.draw(leaderboardButtonSprite);

                        mainWindow.display();
                    }

                    board.leadOpen = true;
                    leaderPause = true; //boolean changed everytime pause button is clicked
                    //Leaderboard Window
                    sf::RenderWindow leaderboardWindow(sf::VideoMode(sf::VideoMode((colCount * 16), (rowCount * 16) + 50)), "Leaderboard");

                    pauseTime = chrono::high_resolution_clock::now();

                    while (leaderboardWindow.isOpen()) {

                        sf::Event event2;
                        while (leaderboardWindow.pollEvent(event2))
                        {

                            if (leaderPause) {
                                //cout << "paused leaderboard" << endl;

                            }
                            if (leaderPause == false) {
                                cout << "unpaused" << endl;

                            }

                            if (event2.type == sf::Event::Closed) {

                                auto unPausedTime = chrono::high_resolution_clock::now();
                                elapsed_paused_time += chrono::duration_cast<chrono::seconds>(unPausedTime - pauseTime).count(); //Addition is necessary for when hitting the pause button more than once

                                leaderPause = false;
                                leaderboardWindow.close();
                            }
                        }
                        leaderboardWindow.clear(sf::Color::Blue);

                        leaderboardWindow.display();
                    }


                    board.leadOpen = false;
                    for (int i = 0; i < board.tiles.size(); i++) {
                        board.tiles[i].isPaused = wasPaused;
                    }
                }
            }
        }

        mainWindow.clear(sf::Color::White);

        //this finds the time elapsed, so the current time - the time the window opened.
        auto game_duration = std::chrono::duration_cast<std::chrono::seconds>(chrono::high_resolution_clock::now() - start_time);
        int total_time = game_duration.count(); // necessary to subtract elapsed time later because "game_duration.count()" is const


        if (!paused && board.isLost == false && board.isOver == false) {
            //enters if the game is NOT paused. This is the condition that keeps the timer from incrementing when paused.
            //std::cout << "not paused\n";

            total_time = total_time - elapsed_paused_time; //
            minutes = total_time / 60;
            seconds = total_time % 60;
        }
        else if (leaderPause) {
            total_time = total_time - elapsed_paused_time; //
            minutes = total_time / 60;
            seconds = total_time % 60;
            leaderPause = false;
        }

        /// CHANGE!!!!
        if (board.reset == true) {


            start_time = std::chrono::high_resolution_clock::now();//starting the timer when the program begins to run!
            pauseTime = std::chrono::high_resolution_clock::now();
            elapsed_paused_time = std::chrono::duration_cast<chrono::seconds>(std::chrono::high_resolution_clock::now() - pauseTime).count();
            game_duration = std::chrono::duration_cast<std::chrono::seconds>(chrono::high_resolution_clock::now() - start_time);
            minutes = total_time / 60;
            seconds = total_time % 60;

            board.reset = false;
        }


        //"separating" the integers. So.... 68 -> seconds0 = 6 and seconds1 = 8
        int minutes0 = minutes / 10 % 10; //minutes index 0
        int minutes1 = minutes % 10; // minutes index 1
        int seconds0 = seconds / 10 % 10; // seconds index 0
        int seconds1 = seconds % 10; // seconds index 1


        digitsMap[minutes0].setPosition((colCount * 32) - 97, 32 * (rowCount + 0.5) + 16);
        mainWindow.draw(digitsMap[minutes0]);

        digitsMap[minutes1].setPosition((colCount * 32) - 78, 32 * (rowCount + 0.5) + 16);
        mainWindow.draw(digitsMap[minutes1]);

        digitsMap[seconds0].setPosition((colCount * 32) - 54, 32 * (rowCount + 0.5) + 16);
        mainWindow.draw(digitsMap[seconds0]);

        digitsMap[seconds1].setPosition((colCount * 32) - 33, 32 * (rowCount + 0.5) + 16);
        mainWindow.draw(digitsMap[seconds1]);

        int numFlags = board.mineCount - board.flags;
        if (numFlags < 0) {
            numFlags *= -1;
            digitsMap[10].setPosition(12, 32 * (rowCount + 0.5) + 16);
            mainWindow.draw(digitsMap[10]);
        }
        if (board.isOver) {
            numFlags = 0;
        }
        digitsMap[numFlags / 100].setPosition(33, 32 * (rowCount + 0.5) + 16);
        mainWindow.draw(digitsMap[numFlags / 100]);

        digitsMap[numFlags / 10 % 10].setPosition(54, 32 * (rowCount + 0.5) + 16);
        mainWindow.draw(digitsMap[numFlags / 10 % 10]);

        digitsMap[numFlags % 10].setPosition(75, 32 * (rowCount + 0.5) + 16);
        mainWindow.draw(digitsMap[numFlags % 10]);


        if (paused) {
            mainWindow.draw(playButtonSprite);
        }
        else if (!paused) {
            mainWindow.draw(pauseButtonSprite);
        }


        if (board.isLost == true) {
            mainWindow.draw(loseFaceButtonSprite);


        }
        else if (board.isOver == true) {
            mainWindow.draw(winFaceButtonSprite);

        }

        else {
            // Happy Face
            mainWindow.draw(happyFaceButtonSprite);
        }

        //print board
        board.displayTiles(mainWindow);

        // LeaderBoard
        mainWindow.draw(leaderboardButtonSprite);

        mainWindow.display();

    }
    return 0;
}