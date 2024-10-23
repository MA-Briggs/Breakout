#include "GameManager.h"
#include "Ball.h"
#include "PowerupManager.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

GameManager::GameManager(sf::RenderWindow* window)
    : _window(window), _paddle(nullptr), _ball(nullptr), _brickManager(nullptr), _powerupManager(nullptr),
    _messagingSystem(nullptr), _ui(nullptr), _pause(false), _time(0.f), _lives(3), _pauseHold(0.f), _levelComplete(false),
    _powerupInEffect({ none,0.f }), _timeLastPowerupSpawned(0.f)
{
    _font.loadFromFile("font/montS.ttf");
    _masterText.setFont(_font);
    _masterText.setPosition(50, 400);
    _masterText.setCharacterSize(48);
    _masterText.setFillColor(sf::Color::Yellow);

    _miniText.setFont(_font);
    _miniText.setPosition(50, 500);
    _miniText.setCharacterSize(24);
    _miniText.setFillColor(sf::Color::Yellow);

    for (int i = 0; i < 10; i++) {
        sf::Text temp;
        temp.setFont(_font);
        temp.setPosition(450, (150 + (50*i)));
        temp.setCharacterSize(24);
        temp.setFillColor(sf::Color::Yellow);
        _leaderboardText.push_back(temp);
    }
}

bool GameManager::sortcol(const std::vector<int>& v1, const std::vector<int>& v2)
{
    return v1[1] < v2[1];
}


void GameManager::initialize()
{
    _paddle = new Paddle(_window);
    _brickManager = new BrickManager(_window, this);
    _messagingSystem = new MessagingSystem(_window);
    _ball = new Ball(_window, 400.0f, this); 
    _powerupManager = new PowerupManager(_window, _paddle, _ball);
    _ui = new UI(_window, _lives, this);

    // Create bricks
    _brickManager->createBricks(5, 10, 80.0f, 30.0f, 5.0f);
    
}

void GameManager::update(float dt, std::stringstream* ss)
{
    if (_powerupManager) { 
        _powerupInEffect = _powerupManager->getPowerupInEffect();
        _ui->updatePowerupText(_powerupInEffect);
        _powerupInEffect.second -= dt;

        if (_time > _timeLastPowerupSpawned + POWERUP_FREQUENCY && rand() % 700 == 0)      // TODO parameterise
        {
            _powerupManager->spawnPowerup();
            _timeLastPowerupSpawned = _time;
        }
    }
   
    

    if (_lives <= 0)
    {
        _masterText.setString("Game over.");
        return;
    }
    if (_levelComplete)
    {
        delete _ball;
        _ball = nullptr;

        delete _paddle;
        _paddle = NULL;

        delete _powerupManager;
        _powerupManager = NULL;

        _masterText.setString("Level completed. ");
        _miniText.setString("Please Enter Your Name [ENTER TO SUBMIT]:  " + ss->str());
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && _showLeaderBoard == false)
        {
            _showLeaderBoard = true;
            std::string input = ss->str();
            input.erase(input.length() - 1, 1);
            std::ofstream leaderboard;
            leaderboard.open("LeaderBoard.txt", std::ios_base::app);

            int seconds = _timeCompleted;
            int minutes = seconds / 60;
            seconds %= 60;
            minutes %= 60;

            leaderboard << input << " " << minutes << ":" << seconds << ";" << _timeCompleted << ",";

            leaderboard.close();
        }
        else if (_showLeaderBoard) {
            std::ifstream leaderboard;
            leaderboard.open("LeaderBoard.txt");
            std::string line;
            std::string section;
            std::vector<std::string> split_sections;
            std::vector<std::vector<int>> times;
            char delim = ',';
            char delim2 = ';';
            while (getline(leaderboard, line, delim)) {

                std::stringstream ss2(line);
                while (getline(ss2, section, delim2)) {
                    split_sections.push_back(section);
                }

            }

            for (int i = 1; i <= split_sections.size(); i += 2) {
                times.push_back({ i-1 , stoi(split_sections.at(i)) });
            }

            sort(times.begin(), times.end(), [](const std::vector<int>& v1, const std::vector<int>& v2){ return v1[1] < v2[1];});

            times.resize(10);
            for (int i = 0; i < times.size(); i++) {

                std::stringstream ss2;
                ss2 << split_sections.at(times[i][0]);

                _leaderboardText.at(i).setString(ss2.str());
            }


        }
       

       
        //return;
    }
    // pause and pause handling
    if (_pauseHold > 0.f) _pauseHold -= dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
    {
        if (!_pause && _pauseHold <= 0.f)
        {
            _pause = true;
            _masterText.setString("paused.");
            _pauseHold = PAUSE_TIME_BUFFER;
        }
        if (_pause && _pauseHold <= 0.f)
        {
            _pause = false;
            _masterText.setString("");
            _pauseHold = PAUSE_TIME_BUFFER;
        }
    }
    if (_pause)
    {
        return;
    }

    // timer.
    _time += dt;


 

    // move paddle
    if (_paddle) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) _paddle->moveRight(dt);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) _paddle->moveLeft(dt);
    }
 

    // update everything 
    if (_paddle) { _paddle->update(dt); }
    if(_ball){ _ball->update(dt); }
   
    if (_powerupManager) { _powerupManager->update(dt); }
}

void GameManager::loseLife()
{
    _lives--;
    _ui->lifeLost(_lives);

    // TODO screen shake.
}

void GameManager::render()
{
    if (_paddle) { _paddle->render(); }
    if (_ball) { _ball->render(); }
    _brickManager->render();
    if (_powerupManager) { _powerupManager->render(); }
    
    if (_showLeaderBoard) {
        for (int i = 0; i < 10; i++) {
            _window->draw(_leaderboardText[i]);
        }
    }
    else {
        _window->draw(_masterText);
        _window->draw(_miniText);
    }
    _ui->render();
}

void GameManager::levelComplete()
{
    _timeCompleted = _time;
    _levelComplete = true;

}

bool GameManager::getLevelComplete()
{
    return _levelComplete;
}


sf::RenderWindow* GameManager::getWindow() const { return _window; }
UI* GameManager::getUI() const { return _ui; }
Paddle* GameManager::getPaddle() const { return _paddle; }
BrickManager* GameManager::getBrickManager() const { return _brickManager; }
PowerupManager* GameManager::getPowerupManager() const { return _powerupManager; }
