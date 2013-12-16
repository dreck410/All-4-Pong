//Shapes.cpp
#include "Shapes.h"
#include "Object.h"
#include <sstream>


// prints the current shape state out to offshore text file,
// returning a boolean value indicating print success
void Shapes::getInfo(vector<string> *strings){

    point.setX(stoi(strings->at(1)));
    point.setY(stoi(strings->at(2)));
    rect.setX(stoi(strings->at(1)));
    rect.setY(stoi(strings->at(2)));
    rect.setWidth(stoi(strings->at(3)));
    rect.setHeight(stoi(strings->at(4)));

}
//Shapes/x/y/width/height/
void Shapes::setInfo(ofstream *f){
    *f <<   "shape/" << point.x() << "/" << point.y() << "/" << rect.width() << "/" << rect.height() << "/" << endl;
}

// reads the current shape state from offshore text file,
// returning a boolean value indicaing read success;
// if read succeeds, stores shape state in instance variables


QString Shapes::getNet(){
    QString out;
    if(net == true){
        stringstream ss;
        ss << "2/" << this->getRect().x() << "/" << this->getRect().y() << "/" << this->getRect().width() << "/" << this->getRect().height() << "/";
        out = QString::fromStdString(ss.str());
        net = false;
    }else{ out = "";}
    return out;
}
// 2/x/y/width/height/

void Shapes::updatePosition(){


}

Shapes::Shapes(bool flip):Object() {
    net = flip;
    //PlayerId = -1;
    int initx = rand() % World::getInstance()->getWorldSize();
    int inity = rand() % World::getInstance()->getWorldSize();
    rect.setX(initx);
    rect.setY(inity);
    int h = rand() % 30 + 25;
    int w = rand() % 30 + 25;
    rect.setHeight(h);
    rect.setWidth(w);
    //bounds checking
    //off the edges
    if(initx < 100)
        initx += 200;
    if(inity < 100)
        inity  += 200;
    if(initx > World::getInstance()->getWorldSize() - (rect.width() + 75))
        initx  -= rect.width()  + 150;
    if(inity > World::getInstance()->getWorldSize() - (rect.height() + 75))
        inity  -= rect.height() + 150;
    //keeps it out of the middle
    while(abs(initx - 205) < 100 && abs(inity - 205) < 100){
        int num = rand() % 200 - 100;

        initx += num; inity += num;
    }
    rect.setX(initx);
    rect.setY(inity);
    rect.setHeight(h);
    rect.setWidth(w);
    point.setX(initx);
    point.setY(inity);
    //   World::getInstance()->add(this);

}

Shapes::~Shapes(){
    //~Object();
}

double Shapes::getDistancetoPaddle(QPoint pointIn){
    if (pointIn.x() >= rect.left() && pointIn.x() <= rect.right() && pointIn.y() >= rect.top() && pointIn.y() <= rect.bottom()) {
        return 0;
    }
    else if (pointIn.x() >= rect.left() && pointIn.x() <= rect.right()) {
        return (double) min(abs(pointIn.y()-rect.top()),abs(pointIn.y()-rect.bottom()));
    }
    else if (pointIn.y() >= rect.top() && pointIn.y() <= rect.bottom()) {
        return (double) min(abs(pointIn.x()-rect.left()),abs(pointIn.x()-rect.right()));
    }
    else {
        return min(min(sqrt(pow(pointIn.x()-rect.topLeft().x(),2)+pow(pointIn.y()-rect.topLeft().y(),2)),sqrt(pow(pointIn.x()-rect.topRight().x(),2)+pow(pointIn.y()-rect.topRight().y(),2))),min(sqrt(pow(pointIn.x()-rect.bottomLeft().x(),2)+pow(pointIn.y()-rect.bottomLeft().y(),2)),sqrt(pow(pointIn.x()-rect.bottomRight().x(),2)+pow(pointIn.y()-rect.bottomRight().y(),2))));
    }
}

int Shapes::hitShape(Ball* b){
    //hit from top left
    if (b->getSpeedX() >= 0 && b->getSpeedY() >=0) {
        if (this->hitLeftSide(b)) {
            return 3;
        }
        else if (this->hitTopSide(b)) {
            return 2;
        }
        else if (b->getX() >= rect.left()) {
            return 2;
        }
        else if (b->getY() >= rect.top()) {
            return 3;
        }
        else {
            return 6;
        }
    }
    //hit from top right
    else if (b->getSpeedX() <= 0 && b->getSpeedY() >=0) {
        if (this->hitTopSide(b)) {
            return 2;
        }
        else if (this->hitRightSide(b)) {
            return 1;
        }
        else if (b->getX() <= rect.right()) {
            return 2;
        }
        else if (b->getY() >= rect.top()) {
            return 1;
        }
        else {
            return 5;
        }
    }
    //hit from bottom right
    else if (b->getSpeedX() <= 0 && b->getSpeedY() <=0) {
        if (this->hitRightSide(b)) {
            return 1;
        }
        else if (this->hitBottomSide(b)) {
            return 0;
        }
        else if (b->getX() <= rect.right()) {
            return 0;
        }
        else if (b->getY() <= rect.bottom()) {
            return 1;
        }
        else {
            return 4;
        }
    }
    //hit from bottom left
    else if (b->getSpeedX() >= 0 && b->getSpeedY() <=0) {
        if (this->hitBottomSide(b)) {
            return 0;
        }
        else if (this->hitLeftSide(b)) {
            return 3;
        }
        else if (b->getX() >= rect.left()) {
            return 0;
        }
        else if (b->getY() <= rect.bottom()) {
            return 3;
        }
        else {
            return 7;
        }
    }
    else {
        return -1;
    }
}

bool Shapes::hitBottomSide(Ball* b) {
    double pcntDistY = abs(((double)b->getY() - b->getRadius() - (double)rect.bottom())/(double)b->getSpeedY());
    double intersectX = (double)b->getX() - b->getRadius() - pcntDistY * (double)b->getSpeedX();
    if (intersectX <= (double)rect.right() && intersectX >= (double)rect.left()) {
        return true;
    }
    return false;
}

bool Shapes::hitLeftSide(Ball* b) {
    double pcntDistX = abs(((double)b->getX() + b->getRadius() - (double)rect.left())/(double)b->getSpeedX());
    double intersectY = (double)b->getY() + b->getRadius() - pcntDistX * (double)b->getSpeedY();
    if (intersectY <= (double)rect.bottom() && intersectY >= (double)rect.top()) {
        return true;
    }
    return false;
}

bool Shapes::hitRightSide(Ball* b) {
    double pcntDistX = abs(((double)b->getX() - b->getRadius() - (double)rect.right())/(double)b->getSpeedX());
    double intersectY = (double)b->getY() - b->getRadius() - pcntDistX * (double)b->getSpeedY();
    if (intersectY <= (double)rect.bottom() && intersectY >= (double)rect.top()) {
        return true;
    }
    return false;
}

bool Shapes::hitTopSide(Ball* b) {
    double pcntDistY = abs(((double)b->getY() + b->getRadius() - (double)rect.bottom())/(double)b->getSpeedY());
    double intersectX = (double)b->getX() + b->getRadius() - pcntDistY * (double)b->getSpeedX();
    if (intersectX <= (double)rect.right() && intersectX >= (double)rect.left()) {
        return true;
    }
    return false;
}
