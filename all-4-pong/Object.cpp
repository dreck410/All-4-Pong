

#include <vector>
#include <QPoint>
#include "World.h"

#include "Object.h"
using namespace std;
//class Ball;
//#include "Ball.h"

int Object::nextObjId = 0;

Object::Object(QPoint initPoint){
    point = initPoint;
    objId = ++nextObjId;
    World::getInstance()->add(this);
}
/*
Object::Object(string type){
    if (Type == "Paddle")
        obj = new Paddle();
    else if(Type =="Ball")
        obj = new Ball(10);
    else if(Type =="Shapes")
        obj = new Shapes();
}
*/
Object::Object(){
    objId = ++nextObjId;
    World::getInstance()->add(this);
}

//inline
Object::~Object() {

}

bool Object::getHit(){return false;}

void Object::updatePosition(){}
bool Object::getBound(){return false;}
QString Object::getNet(){QString Q; return Q;}
