#include "GameManager.h"
#include "Ball.h"
#include "PowerupManager.h"
#include <iostream>
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
    levelComplete();
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
        _miniText.setString("Please Enter 3 Initials [ENTER TO SUBMIT]:  " + ss->str());
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

            leaderboard << input << ";" << minutes << ":" << seconds << ",\n";

            leaderboard.close();
        }
        else if (_showLeaderBoard) {

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
    _window->draw(_masterText);
    _window->draw(_miniText);
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
