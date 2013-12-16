#include "Paddle.h"
#include "Player.h"
#include "World.h"
#include "ingame.h"
#include "Ball.h"
#include <iostream>

int Player::nextID = 0;


Player::Player(){

    ID = nextID;

    currentScore = new Score();

    Health = 7;
    alive = true;
    nextID++;
    hand = World::getInstance()->getMouse(ID);
}

QPoint* Player::getHand(){
    hand = World::getInstance()->getMouse(ID);
    return hand;
}


int Player::getSpeed(){
    return speed;
}
void Player::damage() {
    if(Health > 0){
        --Health;


        currentScore->decreaseScore();
        //cout << "New health of player " << ID << " is " << Health << endl;
        //cout << "New score of player " << ID << " is " << currentScore->getCurrentScore() << endl;
        if (Health == 0) {
            World::getInstance()->died();
            foreach (Object * pad, World::getInstance()->getObjects())
                if (pad->getType() == "paddle" && pad->getPlayerId() == ID) {
                    //  this->setDead();
                    //pad->eliminate();
                    this->setDead();
                    //cout << "Moved player " << ID << " off screen." << endl;
                }
        }
    }
}

void Player::point(){
    currentScore->increaseScore();
    //cout << "New score of player " << ID << " is " << currentScore->getCurrentScore() << endl;
}

void AI::reset() {
    hand->setX(World::getInstance()->getWorldSize()/2);
    hand->setY(World::getInstance()->getWorldSize()/2);
}

QPoint *AI::getHand(){

    command();

    return hand;
}

void AI::command(){
    switch(difficulty){
    case 1:
        //easy
        followRandom(12,2);
        break;
    case 2:
        //medium
        followRandom(20,3);
        break;
    case 3:
        //hard
        followRandom(25,5);
        break;
    }
}

//the AI brain
void AI::followRandom(int big, int min){
    Ball* ball = dynamic_cast<Ball*>(World::getInstance()->getObjects().at(4));
    if(ball != NULL){

        int x = ball->getX();
        int y = ball->getY();

        speed = rand() % big;
        if(speed < min )speed = min;

        if(x < hand->x()){
            hand->setX(hand->x() - speed);
        }
        if(x > hand->x()){
            hand->setX(hand->x() + speed);
        }
        if(y < hand->y()){
            hand->setY(hand->y() - speed);
        }
        if(y > hand->y()){
            hand->setY(hand->y() + speed);
        }
    }
}

void User::calculateSpeed(){
    switch(ID){
    case 0:
    case 2:
        if((oldX > 50 && oldX < 400) && (hand->x() > 50 && hand->x() < 400 )){
            speed = abs(oldX - hand->x()); }else { speed = 0;}
        break;
    case 1:
    case 3:
        if((oldY > 50 && oldY < 400) && (hand->y() > 50 && hand->y() < 400 )){
            speed = abs(oldY - hand->y()); }else { speed = 0;}
        break;
    }

}

int User::getSpeed(){
    return speed;
}

QPoint* User::getHand(){

    Player::getHand();
    calculateSpeed();
    //qDebug() << speed;
    oldX = hand->x();
    oldY = hand->y();

    return hand;
}
//leave this here!! or else it will HATE YOU!!!!
QString Player::getNet(){QString defaultOut = "Default";return defaultOut;}

void Player::setPoint(int x, int y){
    hand->setX(x);
    hand->setY(y);
}

