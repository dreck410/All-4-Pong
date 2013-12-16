#include <vector>
#include <string>
#include <iostream>
#include <QApplication>
#include <cassert>
#include <iostream>
#include <fstream>

#include "score.h"
#include "start.h"


#include "World.h"

#include "ingame.h"


//Pardon our Apearance we are undergoing some changes to better suit our users!
void unitTest() {
    //cout << "World tests starting " << endl;
    //  World::getInstance()->setUp(1, 3, false);
    assert(World::getInstance()->getGamePlayers().size() == 4);
    assert(World::getInstance()->getWorldSize() == 450);
    assert(World::getInstance()->getDifficulty() == 3);


    //cout << "all world tests complete" << endl << "starting user tests" << endl;

    User* us = dynamic_cast<User*>(World::getInstance()->getGamePlayer(0));
    assert(us != NULL);

    //cout << "all unit tests for the user complete" << endl << "Starting unit tests for AI" << endl;

    AI* ai = dynamic_cast<AI*>(World::getInstance()->getGamePlayer(3));
    assert(ai != NULL);
    assert(ai->getDiff() == 3);

    Score* c = new Score();

    c->setCurrentScore(0);
    assert(ai->getCurrentScore()->getCurrentScore() == c->getCurrentScore());
    assert(ai->getHealth() == 7);


    QPoint* q = new QPoint();
    q->setX(205);
    q->setY(205);

    assert(ai->getPoint()->x() == q->x());
    //cout << "all unit tests for the AI complete" << endl;









    //cout << "All unit tests pass!" << endl;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    Start Pong;
    Pong.show();

    vector<string> args(argv, argv+argc); // A vector of command line arguments

    if (args.size() == 2) { // Checks it the user entered a second command line argument
        if (args.at(1) == "-test") { // Checks if the user entered the -test flag
            unitTest(); // Run unit tests
        }
    }


    return a.exec();
}
