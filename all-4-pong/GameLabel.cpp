#include "GameLabel.h"
#include "Object.h"
#include "Paddle.h"
#include "Shapes.h"
#include "Ball.h"

void GameLabel::initializeObj(QString objType) {

    if (objType == "Paddle")
        obj = new Paddle();
    else if(objType =="Ball")
        obj = new Ball(10);
    else if(objType =="Shapes")
        obj = new Shapes(true);
    /*  else
        obj = new Object();

*/}

void GameLabel::updatePosition() {
    this->move(obj->getQPoint().x(),obj->getQPoint().y());
    this->show();
}
