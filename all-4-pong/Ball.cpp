#include "Object.h"
#include "Ball.h"
#include "World.h"
#include "Paddle.h"
//#include "ui_game.h"

#include <QRect>
#include <QPoint>
#include <vector>
#include <cmath>
#include <QString>
#include <string>
///Constructors


Ball::Ball(int initSpeed):Object() {            //takes (speed)
    speed = initSpeed;
    x = World::getInstance()->getWorldSize()/2;
    y = World::getInstance()->getWorldSize()/2;
    radius = World::getInstance()->getWorldSize()/30;
    minSpeed = max(initSpeed/5,1);
    maxSpeed = initSpeed*10;
    playerId = -1; //need to determine a playerId to use for NULL
    speedX = (pow(-1,rand()%2)) * ((rand() % (speed-2))+1);
    setSpeedY(2);
    this->updateSpeedY();
    this->setPoint(); //point used to track the QLabel in the game
    World::getInstance()->addBall(this);
}

Ball::Ball(int initSpeed, int initX, int initY, int initPlayerId):Object() {
    speed = initSpeed;
    x = initX;
    y = initY;
    playerId = initPlayerId;
    radius = World::getInstance()->getWorldSize()/30;
    minSpeed = max(initSpeed/5,1);
    maxSpeed = initSpeed*10;
    speedX = (pow(-1,rand()%2)) * ((rand() % (speed-2))+1);;
    setSpeedY(1);
    this->updateSpeedY();
    this->setPoint();
    World::getInstance()->addBall(this);
}




//Methods


// reads the current ball state from offshore text file,
// returning a boolean value indicaing read success;
// if read succeeds, stores ball state in instance variables

void Ball::getInfo(vector<string>* strings){
    //parses info from a string
    x = stoi(strings->at(1));
    y = stoi(strings->at(2));
    speedX = stoi(strings->at(3));
    speedY = stoi(strings->at(4));
    playerId = stoi(strings->at(5));
}
// Ball/x/y/speedx/speedy/playerid/
void Ball::setInfo(ofstream * f){
    *f << "ball/" << x << "/"<< y << "/"<< speedX << "/"<< speedY<< "/"<< playerId<< "/"<< endl;
}


void Ball::updatePosition(){
    /*   if(this->getSpeedX() > 0 && this->getSpeedY() > 0){
    for(int i = 0; i <= this->getSpeedX() || i <= this->getSpeedY(); i++){

        if(i <= this->getSpeedX()){
            this->setX(this->getX() + i);
        }

        if(i <= this->getSpeedY()){
            this->setY(this->getY() + i);
        }

        if(this->collisionHandler())
        this->setPoint();
        break;
        }
    }else
        if(this->getSpeedX() < 0 && this->getSpeedY() < 0){
            for(int i = 0; i >= this->getSpeedX() || i >= this->getSpeedY(); i--){

                if(i >= this->getSpeedX()){
                    this->setX(this->getX() + i);
                }

                if(i >= this->getSpeedY()){
                    this->setY(this->getY() + i);
                }

                if(this->collisionHandler())break;
                this->setPoint();
                }
        }else
            if(this->getSpeedX() < 0 && this->getSpeedY() > 0){
                for(int i = 0; i >= this->getSpeedX() || abs(i) <= this->getSpeedY(); i--){

                    if(i >= this->getSpeedX()){
                        this->setX(this->getX() + i);
                    }

                    if(abs(i) <= this->getSpeedY()){
                        this->setY(this->getY() + abs(i));
                    }

                    if(this->collisionHandler()){
                        this->setPoint();break;}
                    this->setPoint();
                    }
            }else
                if(this->getSpeedX() > 0 && this->getSpeedY() < 0){
                    for(int i = 0; abs(i) <= this->getSpeedX() || i >= this->getSpeedY(); i--){

                        if(abs(i) <= this->getSpeedX()){
                            this->setX(this->getX() + abs(i));
                        }

                        if(i >= this->getSpeedY()){
                            this->setY(this->getY() + i);
                        }

                        if(this->collisionHandler())break;
                        this->setPoint();
                        }
                }

*/
    this->setX(this->getX() + this->getSpeedX());
    this->setY(this->getY() + this->getSpeedY());
    this->collisionHandler();
    /*End hard coding*/
    this->setPoint();

}



void Ball::setSpeedX(int newSpeedX) {
    if (abs(newSpeedX) >= minSpeed && abs(newSpeedX) <= maxSpeed) {
        speedX = newSpeedX;
        return;
    }
    if (newSpeedX < minSpeed) {
        speedX = abs(this->getSpeedX())/this->getSpeedX() * minSpeed;
        return;
    }
    speedX = abs(this->getSpeedX())/this->getSpeedX() * maxSpeed;
}

void Ball::setSpeedY(int newSpeedY) {
    if (abs(newSpeedY) >= minSpeed && abs(newSpeedY) <= maxSpeed) {
        speedY = newSpeedY;
        return;
    }
    if (abs(newSpeedY) < minSpeed) {
        speedY = abs(this->getSpeedY())/this->getSpeedY() * minSpeed;
        return;
    }
    speedX = abs(this->getSpeedY())/this->getSpeedY() * maxSpeed;
}

void Ball::setPoint() {point = QPoint(this->getX() - this->getRadius(),this->getY()-this->getRadius());}

void Ball::updateSpeedX() {
    this->setSpeedX(this->getSpeedX()/abs(this->getSpeedX())*(int)sqrt(pow(this->getSpeed(),2)-pow(this->getSpeedY(),2)));
}

void Ball::updateSpeedY() {
    this->setSpeedY(this->getSpeedY()/abs(this->getSpeedY())*(int)sqrt(pow(this->getSpeed(),2)-pow(this->getSpeedX(),2)));
}

void Ball::invertSpeedX() {
    this->setSpeedX(this->getSpeedX()*-1);
}

void Ball::invertSpeedY() {
    this->setSpeedY(this->getSpeedY()*-1);
}

bool Ball::collisionHandler() {
    //IF they're a wall
    if (World::getInstance()->getGamePlayer(0)->getHealth() <= 0 && this->getY() + this->getRadius() > World::getInstance()->getWorldSize()*14/15) {
        this->setY(World::getInstance()->getWorldSize()*14/15 - this->getRadius());
        this->invertSpeedY();
        return true;
    }
    if (World::getInstance()->getGamePlayer(1)->getHealth() <= 0 && this->getX() + this->getRadius() > World::getInstance()->getWorldSize()*14/15) {
        this->setX(World::getInstance()->getWorldSize()*14/15 - this->getRadius());
        this->invertSpeedX();
        return true;
    }
    if (World::getInstance()->getGamePlayer(2)->getHealth() <= 0 && this->getY() - this->getRadius() < World::getInstance()->getWorldSize()/15) {
        this->setY(World::getInstance()->getWorldSize()/15 + this->getRadius());
        this->invertSpeedY();
        return true;
    }
    if (World::getInstance()->getGamePlayer(3)->getHealth() <= 0 && this->getX() - this->getRadius() < World::getInstance()->getWorldSize()/15) {
        this->setX(World::getInstance()->getWorldSize()/15 + this->getRadius());
        this->invertSpeedX();
        return true;
    }
    //if they block it
    foreach(Object *o, World::getInstance()->getObjects()) {
        double distance = o->getDistancetoPaddle(QPoint(this->getX(),this->getY()));
        if (distance != -1 && distance <= this->getRadius()) {
            onCollision(o);
            return true;
        }
    }
    // if it scores
    if (this->getX() - this->getRadius() < World::getInstance()->getWorldSize()/15) {
        World::getInstance()->getGamePlayer(3)->damage();
        if (playerId != 3 && playerId != -1)
            World::getInstance()->getGamePlayer(playerId)->point();
        World::getInstance()->setRoundFinished(true);
        this->reset();
    }
    if (this->getX() + this->getRadius() > World::getInstance()->getWorldSize()*14/15) {
        World::getInstance()->getGamePlayer(1)->damage();
        if (playerId != 1 && playerId != -1)
            World::getInstance()->getGamePlayer(playerId)->point();
        World::getInstance()->setRoundFinished(true);
        this->reset();
    }
    if (this->getY() - this->getRadius() < World::getInstance()->getWorldSize()/15) {
        World::getInstance()->getGamePlayer(2)->damage();
        if (playerId != 2 && playerId != -1)
            World::getInstance()->getGamePlayer(playerId)->point();
        World::getInstance()->setRoundFinished(true);
        this->reset();
    }
    if (this->getY() + this->getRadius() > World::getInstance()->getWorldSize()*14/15) {
        World::getInstance()->getGamePlayer(0)->damage();
        if (playerId != 0 && playerId != -1)
            World::getInstance()->getGamePlayer(playerId)->point();
        World::getInstance()->setRoundFinished(true);
        this->reset();
    }
}

void Ball::onCollision(Object *obj) {
    if (obj->getType() == "paddle") {
        this->setPlayerId(obj->getPlayerId());
        ////cout << "Collision with player" << playerId << endl;
        switch (playerId % 2) {
        case 0:
            this->setY(obj->getLine().y1()-this->radius*abs(this->getSpeedY())/this->getSpeedY());
            this->incrementSpeedX(obj->getSpeed());
            this->invertSpeedY();
            this->incrementSpeedY(5 * abs(this->getSpeedY())/this->getSpeedY());
            break;
        case 1:
            this->setX(obj->getLine().x1()-this->radius*abs(this->getSpeedX())/this->getSpeedX());
            this->incrementSpeedY(obj->getSpeed());
            this->invertSpeedX();
            this->incrementSpeedX(5 * abs(this->getSpeedX())/this->getSpeedX());
            break;
        }
    }
    else if (obj->getType() == "shape") {
        Shapes* s = dynamic_cast<Shapes*>(obj);
        int side = s->hitShape(this);
        switch (side) {
        case 0:
            this->setY(s->getRect().bottom()+this->getRadius());
            this->invertSpeedY();
            this->incrementSpeedY(5);
            break;
        case 1:
            this->setX(s->getRect().right()+this->getRadius());
            this->invertSpeedX();
            this->incrementSpeedX(5);
            break;
        case 2:
            this->setY(s->getRect().top()-this->getRadius());
            this->invertSpeedY();
            this->incrementSpeedY(5);
            break;
        case 3:
            this->setX(s->getRect().left()-this->getRadius());
            this->invertSpeedX();
            this->incrementSpeedX(5);
            break;
        case 4:
            this->setX(s->getRect().right()+this->getRadius());
            this->setY(s->getRect().bottom()+this->getRadius());
            this->invertSpeedX();
            this->invertSpeedY();
            this->incrementSpeedX(5);
            this->incrementSpeedY(5);
            break;
        case 5:
            this->setX(s->getRect().right()+this->getRadius());
            this->setY(s->getRect().top()-this->getRadius());
            this->invertSpeedX();
            this->invertSpeedY();
            this->incrementSpeedX(5);
            this->incrementSpeedY(5);
            break;
        case 6:
            this->setX(s->getRect().left()-this->getRadius());
            this->setY(s->getRect().top()-this->getRadius());
            this->invertSpeedX();
            this->invertSpeedY();
            this->incrementSpeedX(5);
            this->incrementSpeedY(5);
            break;
        case 7:
            this->setX(s->getRect().left()-this->getRadius());
            this->setY(s->getRect().bottom()+this->getRadius());
            this->invertSpeedX();
            this->invertSpeedY();
            this->incrementSpeedX(5);
            this->incrementSpeedY(5);
            break;
        }
    }
}


void Ball::incrementSpeedX(int vector) {
    if (vector == 0)
        return;
    this->setSpeedX(this->getSpeedX() + (pow(vector,2)/vector + 5)/10);

}

void Ball::incrementSpeedY(int vector) {
    if (vector == 0)
        return;
    this->setSpeedY(this->getSpeedY() + (pow(vector,2)/vector + 5)/10);

}

void Ball::reset() {
    x = World::getInstance()->getWorldSize()/2;
    y = World::getInstance()->getWorldSize()/2;
    playerId = -1; //need to determine a playerId to use for NULL
    speedX = (pow(-1,rand()%2)) * ((rand() % (speed-2))+1);
    this->updateSpeedY();
    this->setPoint(); //point used to track the QLabel in the game
}

QString Ball::getNet(){

    QString str = "0/ball/"+ QString::number(this->getX()) +
            "/" + QString::number(this->getY()) + "/ ";
    return str;

}





