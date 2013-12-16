#include "ingame.h"
#include "ui_ingame.h"
#include "start.h"


#include <vector>
#include <QPalette>
#include <QBrush>
#include <QColor>

#include <QMouseEvent>
#include <QtWidgets>
#include <QFile>
#include <QDebug>
#include <sstream>


#include "World.h"
#include "Paddle.h"
#include "Object.h"
#include "GameLabel.h"
#include "Timer.h"
#include "start.h"


InGame::InGame(Start* window, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InGame),
    home(window)
{
    ui->setupUi(this);

    setMouseTracking(true);
    ui->gameCourt->setMouseTracking(true);
    counter = 0;




    //Player 0
    Health.push_back(ui->lblLife1PB);
    Health.push_back(ui->lblLife2PB);
    Health.push_back(ui->lblLife3PB);
    Health.push_back(ui->lblLife4PB);
    Health.push_back(ui->lblLife5PB);
    Health.push_back(ui->lblLife6PB);
    Health.push_back(ui->lblLife7PB);
    // player 1
    Health.push_back(ui->lblLife1PR);
    Health.push_back(ui->lblLife2PR);
    Health.push_back(ui->lblLife3PR);
    Health.push_back(ui->lblLife4PR);
    Health.push_back(ui->lblLife5PR);
    Health.push_back(ui->lblLife6PR);
    Health.push_back(ui->lblLife7PR);
    //player 2
    Health.push_back(ui->lblLife1PT);
    Health.push_back(ui->lblLife2PT);
    Health.push_back(ui->lblLife3PT);
    Health.push_back(ui->lblLife4PT);
    Health.push_back(ui->lblLife5PT);
    Health.push_back(ui->lblLife6PT);
    Health.push_back(ui->lblLife7PT);
    //player 3
    Health.push_back(ui->lblLife1PL);
    Health.push_back(ui->lblLife2PL);
    Health.push_back(ui->lblLife3PL);
    Health.push_back(ui->lblLife4PL);
    Health.push_back(ui->lblLife5PL);
    Health.push_back(ui->lblLife6PL);
    Health.push_back(ui->lblLife7PL);



    //Link Game Model to GUI

    ui->gameCourt->findChild<GameLabel*>("lblPaddleBottom")->initializeObj("Paddle");
    ui->gameCourt->findChild<GameLabel*>("lblPaddleRight")->initializeObj("Paddle");
    ui->gameCourt->findChild<GameLabel*>("lblPaddleTop")->initializeObj("Paddle");
    ui->gameCourt->findChild<GameLabel*>("lblPaddleLeft")->initializeObj("Paddle");
    ui->gameCourt->findChild<GameLabel*>("lblBall")->initializeObj("Ball");

    //Loads a file if the user clicked the load file button
    if(World::getInstance()->getFile()){
        //load file

        World::getInstance()->readWorldInfo();


        for(int i = 5; i < World::getInstance()->getObjects().size(); i++){

            GameLabel* block = new GameLabel(ui->gameCourt);
            // objects.at(i);
            block->setObj(World::getInstance()->getObjects().at(i));
            setUpBlocklbl(block);
        }
    }
//Sets the users scores to either 0 or the loaded file
    ui->lblScorePB->setText(QString::number(World::getInstance()->getGamePlayer(0)->getCurrentScore()->getCurrentScore()));
    ui->lblScorePR->setText(QString::number(World::getInstance()->getGamePlayer(1)->getCurrentScore()->getCurrentScore()));
    ui->lblScorePT->setText(QString::number(World::getInstance()->getGamePlayer(2)->getCurrentScore()->getCurrentScore()));
    ui->lblScorePL->setText(QString::number(World::getInstance()->getGamePlayer(3)->getCurrentScore()->getCurrentScore()));

    //Set the health bar according to the loaded file.
    for(int i = 0;i < 4;i++){
        for(int j = 7; j > World::getInstance()->getGamePlayer(i)->getHealth();){
            j--;
            HealthDamage(i,j);
        }
    }

//links the paddles to the players
    ui->gameCourt->findChild<GameLabel*>("lblPaddleBottom")->getObj()->setPlayerId(0);
    ui->gameCourt->findChild<GameLabel*>("lblPaddleRight")->getObj()->setPlayerId(1);
    ui->gameCourt->findChild<GameLabel*>("lblPaddleTop")->getObj()->setPlayerId(2);
    ui->gameCourt->findChild<GameLabel*>("lblPaddleLeft")->getObj()->setPlayerId(3);



    setUsernames();
    setHighScores();


    //Setup the Timer
    Timer::getInstance()->getTimer()->setInterval(20);//approx 60 fps
    connect(Timer::getInstance()->getTimer(), &QTimer::timeout,this,&InGame::timerHit);
    // connect(Timer::getInstance()->getTimer(), &QTimer::timeout,this,&InGame::Animate);


}



InGame::~InGame() {
    delete ui;
}

//this is called by the World class for when a player is damaged this is to change the
// graphic!
void InGame::HealthDamage(int index, int health){
    int spot = 7 * index;

    if(health > -1 && health < 7){
        QLabel* lbl = Health.at(spot + health);
        lbl->setStyleSheet("background-color: rgb(0, 0, 0); border-radius: 10px;");

    }
    else if (health < 0) {
        //less than zero don't do anything, risk dipping into the next players health
    }
}

void InGame::setUsernames(){
    vector<Player*> players = World::getInstance()->getGamePlayers();
    for(Player *inPlayer : players){
        int ID = inPlayer->getID();
        QString name = inPlayer->getUsername();
        if(ID == 0){
            ui->lblUsernamePB->setText(name);
        }else if(ID == 1){
            ui->lblUsernamePR->setText(name);
        }else if(ID == 2){
            ui->lblUsernamePT->setText(name);
        }else if(ID == 3){
            ui->lblUsernamePL->setText(name);
        }
    }
}

//Pauses the game but right now running health bar tests.
void InGame::on_btnPause_clicked() {
    Pause();
}



void InGame::on_btnCheat_clicked() {
    for(int i = 0; i < 10; i++){
        int num = rand() % 4;
        if(World::getInstance()->getGamePlayer(num)->getHealth() > 0){
            World::getInstance()->getGamePlayer(num)->damage();
            HealthDamage(num,World::getInstance()->getGamePlayer(num)->getHealth());
            break;
        }
    }
}

QPoint InGame::getGameCourt(QPoint in) {
    QPoint out;
    out = ui->gameCourt->mapFromParent(in);

    return out;
}

void InGame::mouseMoveEvent(QMouseEvent *ev) {

    QPoint l;
    l = this->getGameCourt(ev->pos());
    QPoint* m = new QPoint(l.x(), l.y());
    World::getInstance()->addMouse(m,0);

}

//for testing purposes
void InGame::mousePressEvent(QMouseEvent *ev) {
    //qDebug() << getGameCourt(ev->pos()).x() << ", "<< getGameCourt(ev->pos()).y() << "  ------------------------------";
   ev;
    Pause();

}

bool InGame::makeBlock(bool powerUps){
    if(powerUps)
        counter++;
    if(counter == 450){
        counter = 0;
        return true;
    }
    return false;

}

void InGame::setUpBlocklbl(GameLabel* block){
    block->setObjectName("lblPopUp");
    int r = rand() % 200;
    int g = rand() % 200;
    int b = rand() % 255;
    stringstream sstrm;
    sstrm << "background-color: rgb(" << r << "," << g << "," << b << "); border-radius:6px;";
    QString styleSheet;
   styleSheet = QString::fromStdString(sstrm.str());


    block->setStyleSheet(styleSheet);
    block->setGeometry(block->getObj()->getRect());

}

void InGame::timerHit() {

    if(makeBlock(World::getInstance()->getPopUps())){

        GameLabel* block = new GameLabel(ui->gameCourt, "Shapes");
        setUpBlocklbl(block);

    }

    World::getInstance()->UpdateWorld();
    // makeBlock(World::getInstance()->getPower());
    foreach (GameLabel *g, ui->gameCourt->findChildren<GameLabel*>()) {
        g->updatePosition();
    }

    World::getInstance()->gameOver();
    if (World::getInstance()->getRoundFinished() == true) {
        World::getInstance()->pointScoredReset();

        Timer::getInstance()->getTimer()->stop();
        ui->lblGameDeclaration->setText(World::getInstance()->getRound()); // display 'Round Over' declaration
        ui->lblPlayDeclaration->setText(World::getInstance()->getInstruct()); // display pressPlay instructions
        ui->btnPause->setText("Play");
        ui->lblScorePB->setText(QString::number(World::getInstance()->getGamePlayer(0)->getCurrentScore()->getCurrentScore()));
        ui->lblScorePR->setText(QString::number(World::getInstance()->getGamePlayer(1)->getCurrentScore()->getCurrentScore()));
        ui->lblScorePT->setText(QString::number(World::getInstance()->getGamePlayer(2)->getCurrentScore()->getCurrentScore()));
        ui->lblScorePL->setText(QString::number(World::getInstance()->getGamePlayer(3)->getCurrentScore()->getCurrentScore()));
        for (int i = 0; i < 4; ++i) {
            HealthDamage(i,World::getInstance()->getGamePlayer(i)->getHealth());
        }
    } else if (World::getInstance()->getRoundFinished() == false) {
        ui->lblGameDeclaration->setText(""); // do not display 'Round Over' declaration
        ui->lblPlayDeclaration->setText(""); // do not display pressPlay instructions
    }

}


//Resets all of the world items... and by that i mean, calls the gamemodel class to reset all of the world items! and redisplay the
// home screen.
void InGame::on_btnHome_clicked() {
    GoHome();
}

void InGame::GoHome(){
    this->deleteLater();
    Timer::getInstance()->getTimer()->stop();
    World::getInstance()->ResetWorld();

    ui->btnPause->setText("Play");

    home->clientStop();
    home->clientReset();
    home->show();


}

void InGame::Pause(){


    if(World::getInstance()->getEnd()){
        GoHome();
    }else{
        World::getInstance()->setRoundFinished(false);
        if (ui->btnPause->text() == "Pause") {
            Timer::getInstance()->getTimer()->stop();
            ui->btnPause->setText("Play");
        }

        else if (ui->btnPause->text() == "Play") {
            Timer::getInstance()->getTimer()->start();
            ui->btnPause->setText("Pause");
        }
        //purley testing this breaks object view blah blah blah
        World::getInstance()->printWorldInfo();
    }
}

void InGame::setHighScores() {
    HighScore::getInstance()->importData();
    ui->lblTop1Score->setText(QString::number(HighScore::getInstance()->getFirstHighestScore()));
    ui->lblTop1Usrnm->setText(HighScore::getInstance()->getFirstHSPlayerName());
    ui->lblTop2Score->setText(QString::number(HighScore::getInstance()->getSecondHighestScore()));
    ui->lblTop2Usrnm->setText(HighScore::getInstance()->getSecondHSPlayerName());
    ui->lblTop3Score->setText(QString::number(HighScore::getInstance()->getThirdHighestScore()));
    ui->lblTop3Usrnm->setText(HighScore::getInstance()->getThirdHSPlayerName());
}


