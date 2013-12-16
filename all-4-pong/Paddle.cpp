
#include "Paddle.h"
#include "Ball.h"
#include "Object.h"
//#include "World.h"

#include <QRect>
#include <vector>
#include <QDebug>
#include <QString>
#include <cmath>
#include <iostream>
//#include <strstream>
#include <sstream>

#include "Paddle.h"
using namespace std;
//sets up the paddle depending on what location it is to be in.

void Paddle::setUp() {
    int worldSize = World::getInstance()->getWorldSize();
    switch(playerId){
    case 0:
        point.setX((worldSize - length)/2);
        point.setY(worldSize - (width * 2));
        break;
    case 1:
        point.setX(worldSize - (width * 2));
        point.setY((worldSize - length)/2);
        break;
    case 2:
        point.setX((worldSize - length)/2);
        point.setY(width);
        break;
    case 3:
        point.setX(width);
        point.setY((worldSize - length)/2);
        break;
    }
    this->setUpLine();
} //"Does this work??" + QString("kjdhfsk");

//
//+ Hand->getCurrentScore()->getCurrentScore()+ Hand->getHealth() + "/ ";
QString Paddle::getNet(){
    QString out;
    int xx = point.x();
    int yy = point.y();
    int score = Hand->getCurrentScore()->getCurrentScore();
    int health = Hand->getHealth();
    int thisId = playerId;
    QString UserName = Hand->getUsername();
    stringstream ss;
    ss << "1/" << thisId << "/" << UserName.toStdString() << "/" << xx <<"/" << yy <<"/" <<
          score <<"/" << health << "/ ";
    out = QString::fromStdString(ss.str());
    return out;

}


QString Paddle::getUserName(){
    return Hand->getUsername();
}

void Paddle::setUpLine() {
    switch(playerId){
    case 0:
        line.setLine(point.x(),point.y(),point.x()+length,point.y());
        break;
    case 1:
        line.setLine(point.x(),point.y(),point.x(),point.y()+length);
        break;
    case 2:
        line.setLine(point.x(),point.y()+width,point.x()+length,point.y()+width);
        break;
    case 3:
        line.setLine(point.x()+width,point.y(),point.x()+width,point.y()+length);
        break;
    }
}

void Paddle::moveLine(int distance) {
    switch (playerId % 2) {
    case 0:
        line.translate(distance,0);
        break;
    case 1:
        line.translate(0,distance);
        break;
    }
}

//Planning on getting rid of this unless you need it for the AI.
//If you do, give me a method to call in the balls logic.

//I don't think i need it.
/*     bool Paddle::getHit() {
            return false;
        }
*/
// prints the current paddle state out to offshore text file,
// returning a boolean value indicating print success
bool Paddle::printPaddleInfo() {
    bool didPrint = false;

    // establish connection with text file

    if (/*connection succeeds*/true) {
        // gather object state and concatenate into string
        // print string of state to text file
        didPrint = true;
    }

    return didPrint;
}

// reads the current paddle state from offshore text file,
// returning a boolean value indicaing read success;
// if read succeeds, stores paddle state in instance variables
bool Paddle::readPaddleInfo() {
    bool didRead = false;

    // establish connection with text file
    //Joseph you're a boss!!!
    if (/*connection succeeds*/true) {
        // read string of state from text file
        // parse string and store object state in instance variables
        didRead = true;
    }

    return didRead;
}


// /paddle/id/x/y/health/score/username



//runs the  update position code overridden from Object.

// then moves them accordingly.
void Paddle::updatePosition() {

    if(Hand->getAlive()){
        int orig;
        switch (playerId % 2) {
        case 0:
            orig = this->point.x();
            this->setX(Hand->getHand()->x()-(length/2));
            speed = this->point.x()-orig;
            break;
        case 1:
            orig = this->point.y();
            this->setY(Hand->getHand()->y()-(length/2));
            speed = this->point.y()-orig;
            break;
        }
        moveLine(speed);
    }else{
        eliminate();
    }

}

void Paddle::setY(int newY) {
    if (playerId % 2 == 0)
        return;
    point.setY(newY);
    if(point.y() < 0)
        point.setY(0);
    if(point.y() > (World::getInstance()->getWorldSize() - (length)))
        point.setY(World::getInstance()->getWorldSize() - (length));
}



void Paddle::setX(int newX) {
    if (playerId % 2 == 1)
        return;
    point.setX(newX);
    if(point.x() < 0)
        point.setX(0);
    if(point.x() > (World::getInstance()->getWorldSize() - (length)))
        point.setX(World::getInstance()->getWorldSize() - (length));
}

double Paddle::getDistancetoPaddle(QPoint pointIn) {
    switch(playerId % 2) {
    case 0:
        if (pointIn.x() >= line.x1() && pointIn.x() <= line.x2()) {
            return (double) abs(pointIn.y()-line.y1());
        }
        break;
    case 1:
        if (pointIn.y() >= line.y1() && pointIn.y() <= line.y2()) {
            return (double) abs(pointIn.x()-line.x1());
        }
        break;
    }
    return min(sqrt(pow(pointIn.x()-line.x1(),2)+pow(pointIn.y()-line.y1(),2)),sqrt(pow(pointIn.x()-line.x2(),2)+pow(pointIn.y()-line.y2(),2)));
}

void Paddle::eliminate() {
    //qDebug() << "Moving player " << playerId << " off screen" << endl;
    point.setX(-World::getInstance()->getWorldSize());
    point.setY(-World::getInstance()->getWorldSize());


    this->setUpLine();
}



void Paddle::getInfo(vector<string>* strings){
    this->setPlayerId(stoi(strings->at(1)));
    this->setX(stoi(strings->at(2)));
    this->setY(stoi(strings->at(3)));
    int health = stoi(strings->at(4));
    for(int i = 7; i > health; i--){
        Hand->damage();
        if(Hand->getHealth() <= 0){

        }
    }
    Hand->getCurrentScore()->setCurrentScore(stoi(strings->at(5)));
    Hand->setUsername(QString::fromStdString(strings->at(6)));
}

// Paddle/playerid/x/y/health/score/username/
void Paddle::setInfo(ofstream *f){
    if(Hand->getUsername().toStdString() == "")Hand->setUsername("User");
    *f  << "paddle/" << playerId<< "/" << point.x() << "/"
        << point.y() << "/" << Hand->getHealth() << "/"
        << Hand->getCurrentScore()->getCurrentScore() << "/"
        << Hand->getUsername().toStdString()<< "/" << endl;

}


