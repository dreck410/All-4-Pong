#include <QMessageBox>
#include <QDateTime>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QApplication>
#include <QWindow>
#include <QtWidgets>
#include <sstream>


#include <QTimer>
#include "ingame.h"
#include "World.h"
#include "ui_ingame.h"
#include "start.h"
#include "ui_start.h"


//-----------------------------------------------------
//Does the host have to run a client and connect to the host game also?- Daniel
//The host will play the game but will do it through the server instead of powering
//up another client. -Thomas
//-----------------------------------------------------

Start::Start(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Start),
    connectCount(0)
{
    ui->setupUi(this);
    sound = false;
    timer = new QTimer(this);
    timer->setInterval(100);
    connect(timer, &QTimer::timeout, this, &Start::timerHit);
    connectCount = 1;
    server = new QTcpServer(this);
    ok = true;
    connect(server, &QTcpServer::newConnection, this, &Start::clientConnected);
    if (!server->listen(QHostAddress::Any, 5000)) {
        QMessageBox::critical(this, "Uh oh", "Cannot start socket.");
        exit(1);

    }

}


void Start::on_btnLoad_clicked(){
    World::getInstance()->setFile(true);
    InGame* gameScreen = new InGame(this);

    for (QObject *obj : server->children()){
        QTcpSocket *anotherSock = dynamic_cast<QTcpSocket*>(obj);
        if (anotherSock != NULL){
            QString str = "start";
            anotherSock->write(str.toLocal8Bit()+"\n");
        }
    }

    gameScreen->show();
    timer->start();
    this->hide();

}

//this method detects when a new client is connected and increments the connected count
void Start::clientConnected()
{
    QTcpSocket *sock = server->nextPendingConnection();
    connect(sock, &QTcpSocket::disconnected, this, &Start::clientDisconnected);
    connect(sock, &QTcpSocket::readyRead, this, &Start::dataReceived);
    ++connectCount;
    QString str;
    enoughPlayers();
    str = "Users: " + QString::number(connectCount);
    ui->lblConnected->setText(str);
    stringstream ss;
    QString side;
    ss << "side/" << connectCount -1 << "/";
    side = QString::fromStdString(ss.str());
    sock->write(side.toLocal8Bit() + "\n");
}
//checks teh combo box to the number of people connected.
// only allows the game to play if the correct number of players are present.
// ?Question is do we want the host to have to connect? ?
//ifso we have to change this.!!
void Start::enoughPlayers(){
    if (connectCount == ui->players_comboBox->currentIndex()+1){
        ui->start_Btn->setEnabled(true);
    }else{
        ui->start_Btn->setEnabled(false);
    }
}

void Start::timerHit(){
    //example ball:   0/ball1/x/y/
    //example player:  1/0/username/x/y/points/health/
    QString netString = World::getInstance()->getNetwork();
    if (sound == true){
        netString += "sound ";
    }
    for (QObject *obj : server->children()){
        QTcpSocket *anotherSock = dynamic_cast<QTcpSocket*>(obj);
        if (anotherSock != NULL){
            anotherSock->write(netString.toLocal8Bit()+"\n");
        }
        if (ok){
            ++clock;
        }
    }
    //send out the same information to all of the clients at the same time (remember server also plays game).
    //to see the format of this information, look at the networking Wiki page.

}

void Start::clientStop(){
    for (QObject *obj : server->children()){
        QTcpSocket *anotherSock = dynamic_cast<QTcpSocket*>(obj);
        if (anotherSock != NULL){
            QString netString = "stop";
            anotherSock->write(netString.toLocal8Bit()+"\n");
        }
        if (ok){
            ++clock;
        }
    }
}

//THis is the mouse information coming in!! Put this in a vector of mice held in the World Class -daniel To himself.

//recieves x,y,and paddle id from user
void Start::dataReceived()
{
    QTcpSocket *sock = dynamic_cast<QTcpSocket*>(sender());
    while (sock->canReadLine()) {
        QString str = sock->readLine();
        //  qDebug() << str;
        //World::getInstance()->updateUser(str); akjdhfa
        //do something with the information that is coming in
        //   "3/Thomas/x/y/
        //      pos/username/x/y/
        vector<QString>* info = World::getInstance()->split(str,'/');
        int pos = info->at(0).toInt();
        QString userName = info->at(1);
        if (ok && clock < 50){
            Player *inPlayer = World::getInstance()->getGamePlayer(pos);
            inPlayer->setUsername(userName);
            gameScreen->setUsernames();
            if (clock > 50){
                ok = false;
            }
        }
        int x = info->at(2).toInt();
        int y = info->at(3).toInt();
        QPoint* mouseIn = new QPoint(x,y);
        World::getInstance()->addMouse(mouseIn,pos);
        World::getInstance()->setPlayerName(info->at(1), pos);
    }

    //}
    //**********This is Schaub code that we can use as an example****************
    /*QTcpSocket *sock = dynamic_cast<QTcpSocket*>(sender());

    addToLog("Received data from socket ");
    while (sock->canReadLine()) {
        QString str = sock->readLine();
        addToLog("-> " + str);

        // send data to all connected clients
        for (QObject *obj : server->children()) {
            QTcpSocket *anotherSock = dynamic_cast<QTcpSocket*>(obj);
            if (anotherSock != NULL)
                anotherSock->write(str.toLocal8Bit());
        }

    }*/
}


//detects when a user disconnects and decrements the connectCount
void Start::clientDisconnected()
{
    QTcpSocket *sock = dynamic_cast<QTcpSocket*>(sender());
    sock->deleteLater();
    --connectCount;
    //ui->lblConnected->setText(QString::number(connectCount));
    //addToLog("Client disconnected.");
}


//this method will launch the actual game. this button is only activated when there are the
//correct number of players connected. That number is determined by the players combobox.
void Start::on_start_Btn_clicked()
{//"/home/user/csunix/dreck410/team/build-all-4-pong-Desktop-Debug/all-4-pong"

    //Oh-No the user pressed the start button and there is no game yet!!! ahhhh fix it. Go.

    StartingMethod();
    // return a.exec();

}

void Start::StartingMethod(){

    World::getInstance()->setDifficulty(ui->dif_comboBox_->currentIndex()+1);

    World::getInstance()->setupPlayers(ui->players_comboBox->currentIndex()+1);

    World::getInstance()->setPlayerName(ui->username_le->text(), 0);
    World::getInstance()->setFile(false);

    // players = ui->players_comboBox->currentIndex()+1;

    // difficulty = ui->dif_comboBox_->currentIndex()+1;

    World::getInstance()->setPopUps(ui->power_checkBox->isChecked());

    gameScreen = new InGame(this);

    for (QObject *obj : server->children()){
        QTcpSocket *anotherSock = dynamic_cast<QTcpSocket*>(obj);
        if (anotherSock != NULL){
            QString str = "start";
            anotherSock->write(str.toLocal8Bit()+"\n");
        }
    }

    timer->start();
    gameScreen->show();
    this->hide();


}

void Start::clientReset(){
    for (QObject *obj : server->children()){
        QTcpSocket *anotherSock = dynamic_cast<QTcpSocket*>(obj);
        if (anotherSock != NULL){
            QString str = "reset";
            anotherSock->write(str.toLocal8Bit()+"\n");
        }
    }
}



//When the number of players box is changed checks to see how many
// people are actaully connected so it locks it up or unlocks it
// if it's changed back to the right number.
void Start::on_players_comboBox_activated(int index)
{
    index;
    enoughPlayers();
}
