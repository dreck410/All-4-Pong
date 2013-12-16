#include "clientingame.h"
#include "ui_clientingame.h"


clientingame::clientingame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::clientingame)
{
    ui->setupUi(this);
    setMouseTracking(true);
    x = 255;
    y = 255;

    //adds all of the health labels to a vector
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

}

//client game destructor
clientingame::~clientingame()
{
    delete ui;
}
//captures mouse position and maps it to the gameCourt widget
void clientingame::mouseMoveEvent(QMouseEvent *ev) {

    x = ui->gameCourt->mapFromParent(ev->pos()).x();

    y = ui->gameCourt->mapFromParent(ev->pos()).y();
    //timer->start();
}

//recieves a vector of label commands and moves the proper label
void clientingame::MoveLabels(std::vector<QString> *v){
    if (v->at(0) == "0"){
        // 0/ball1/x/y/

        QPoint point;
        point.setX(v->at(2).toInt());
        point.setY(v->at(3).toInt());
        //int ballX = ui->gameCourt->mapFromParent(point).x();
        //int ballY = ui->gameCourt->mapFromParent(point).y();
        //ui->lblBall_2->move(ballX, ballY);
        ui->lblBall_2->move(point.x() - 12, point.y() - 12);
    }else if(v->at(0) == "1"){
        //1/0/username/x/y/points/health/
        int xx;
        int yy;
        int health;
        if (v->at(1) == "0"){ //bottom player
            xx = v->at(3).toInt();
            yy = v->at(4).toInt();
            health = v->at(6).toInt();
            ui->paddle_0->move(xx,yy);
            ui->lblUsernamePB->setText(v->at(2));
            ui->lblScorePB->setText(v->at(5));
            HealthDamage(0,health);

        }else if(v->at(1) == "1"){ // right players
            xx = v->at(3).toInt();
            yy = v->at(4).toInt();
            health = v->at(6).toInt();
            ui->paddle_1->move(xx,yy);
            ui->lblUsernamePR->setText(v->at(2));
            ui->lblScorePR->setText(v->at(5));
            HealthDamage(1,health);

        }else if(v->at(1) == "2"){ // top player
            xx = v->at(3).toInt();
            yy = v->at(4).toInt();
            health = v->at(6).toInt();
            ui->paddle_2->move(xx,yy);
            ui->lblUsernamePT->setText(v->at(2));
            ui->lblScorePT->setText(v->at(5));
            HealthDamage(2,health);

        }else if(v->at(1) == "3"){ // left player
            xx = v->at(3).toInt();
            yy = v->at(4).toInt();
            health = v->at(6).toInt();
            ui->paddle_3->move(xx,yy);
            ui->lblUsernamePL->setText(v->at(2));
            ui->lblScorePL->setText(v->at(5));
            HealthDamage(3,health);
        }

    }else if(v->at(0) == "2"){ //creates one of the popping up objects
        //2/x/y/width/height/
        //update the object
        QLabel *lbl = new QLabel(ui->gameCourt);

        lbl->setGeometry(v->at(1).toInt(), v->at(2).toInt(), v->at(3).toInt(), v->at(4).toInt());
        lbl->setStyleSheet("background-color: rgb(255,255,255); border-radius: 6px;");
        lbl->show();
        objs.push_back(lbl);
    }else if(v->at(0) == "3"){//sets the high scores banner
        // 3/username/score/username/score/username/score/username/score/
        ui->lblTop1Usrnm->setText(v->at(1));
        ui->lblTop1Score ->setText(v->at(2));
        ui->lblTop2Usrnm->setText(v->at(3));
        ui->lblTop2Score ->setText(v->at(4));
        ui->lblTop3Usrnm->setText(v->at(5));
        ui->lblTop3Score ->setText(v->at(6));
    }

}

//sets the health labels to reflect the host info
void clientingame::HealthDamage(int index, int health){
    int spot = 7 * index;
    //--health;
    if(health > -1 && health < 7){
        QLabel* lbl = Health.at(spot + health);
        lbl->setStyleSheet("background-color: rgb(0, 0, 0); border-radius: 10px;");
    }
    else if (health < 0) {
        //qDebug() << "No more death possible";
    }
}

//resets all health back to full and deletes objects
void clientingame::resethealth(){
    for(QLabel *lbl: Health){
        lbl->setStyleSheet("background-color: rgb(0, 200, 0);\nborder-radius: 10px;");

    }
    for(QLabel *obj: objs){
        obj->deleteLater();
    }
}
