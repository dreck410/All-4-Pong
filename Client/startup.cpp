#include "startup.h"
#include "ui_startup.h"
#include "clientingame.h"



Startup::Startup(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Startup)
{
    ui->setupUi(this);
    clientgame = new clientingame();
    ok = false;
    clock = 0;
    timer = new QTimer(this);
    random = new QTimer(this);
    timer->setInterval(40);
    random->setInterval(40);
    connect(timer, &QTimer::timeout, this, &Startup::timerHit);
    connect (random,  &QTimer::timeout, this, &Startup::wait);

    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &Startup::dataReceived);
    connect(socket, &QTcpSocket::disconnected, this, &Startup::serverDisconnected);
}

Startup::~Startup()
{
    delete ui;
}

//connects to the host and sets a couple labels
void Startup::on_connect_Btn_clicked()
{
    QString hostname = ui->IP_line->text();
    if (hostname.size() == 0) {
        QMessageBox::critical(this, "Uh oh", "Please specify name of the host.");
        return;
    }
    socket->connectToHost(hostname, 5000);
    if (!socket->waitForConnected())  {
        QMessageBox::critical(this, "Uh oh", "Unable to connect to host.");
        return;
    }
    ui->connect_Btn->setText("Connected");
    ui->connect_Btn->setEnabled(false);
    ok = true;
    random->start();
    ui->wait_lbl->setText("Waiting for host to start game");
}



//my own personal split method... nuff said
vector<QString> *Startup::split(QString str, char delim){
    //vector<QString> Split(string str, char delim){
    vector<QString> *splitV = new vector<QString>();
    QString buf = "";
    int i = 0;
    while (i < str.length()){
        if (str[i] != delim){
            buf += str[i];
        } else if (buf.length() > 0) {
            splitV->push_back(buf);
            buf = "";
        }
        i++;
    }
    if (!buf.isEmpty())
        splitV->push_back(buf);
    return splitV;
}



//splits incoming string and calls method to move labels
void Startup::dataReceived() {

    while (socket->canReadLine()) {
        QString str = socket->readLine();
        //this will recieve a string of the a list of objects with their x and y coordinates
        //For example, ball,
        vector<QString> *spaceSplit = split(str, ' ');
        //vector<QString> *spaceSplit = str.split(' ');
        while(spaceSplit->size() > 0){
            vector<QString> *slashSplit = split(spaceSplit->at(0), '/');
            if(slashSplit->at(0) == "start\n"){
                clientgame->show();
                this->hide();
                random->stop();
                timer->start();
                ok = false;
                ui->wait_lbl->setText("");
                ui->connect_Btn->setEnabled(true);
                ui->connect_Btn->setText("Connect");
            }
            if(slashSplit->at(0) == "side"){
                side = slashSplit->at(1);
            }
            if(slashSplit->at(0) == "stop\n"){
                timer->stop();
            }
            if(slashSplit->at(0) == "reset\n"){
                clientgame->resethealth();
            }
            spaceSplit->erase(spaceSplit->begin()+0);
            clientgame->MoveLabels(slashSplit);
        }
    }
}

void Startup::serverDisconnected()
{
    ui->statusBar->showMessage("Disconnected.");
    //  ui->btnConnect->setEnabled(true);
    QMessageBox::critical(this, "Error","The host has disconnected." );
    clientgame->close();
    this->close();
}

//changes a label constantly while waiting for the host
void Startup::wait(){
    if (ok){
        ++clock;
    }
    if (clock == 10 && ok){
        ui->wait_lbl->setText("Waiting for host to start game.");
    }else if (clock == 20 && ok){
        ui->wait_lbl->setText("Waiting for host to start game..");
    }else if (clock == 30 && ok){
        ui->wait_lbl->setText("Waiting for host to start game...");
    }else if (clock == 40 && ok){
        ui->wait_lbl->setText("Waiting for host to start game");
        clock = 0;
    }
}

//this is called every clock tick and sends the paddle x, y, and Username
void Startup::timerHit()
{
    QString str =  side + "/" + ui->username_line->text() + "/" + QString::number(clientgame->getX()) + '/' + QString::number(clientgame->getY()) + '/'+ "\n";
    socket->write(str.toLocal8Bit());
}




