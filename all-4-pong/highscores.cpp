
#include "highscores.h"
#include <QString>
#include "World.h"


//initiates the HighScore singleton.
//Creates a new file on each computer
//that plays
HighScore* HighScore::instance=NULL;


void HighScore::addScore(QString username, int score) {
    if (score > firstHighestScore) {
        addScore(firstHSPlayerName,firstHighestScore);
        firstHighestScore = score;
        firstHSPlayerName = username;
    }
    else if (score > secondHighestScore) {
        addScore(secondHSPlayerName,secondHighestScore);
        secondHighestScore = score;
        secondHSPlayerName = username;
    }
    else if(score > thirdHighestScore) {
        thirdHighestScore = score;
        thirdHSPlayerName = username;
    }
    print = true;
}

void HighScore::importData() {
    string info = "";
    fstream* stream = new fstream;
    stream->open("high_score.txt");
    if (stream->is_open()) {
        getline(*stream, info);
    }
    if (info == "") {
        firstHighestScore = secondHighestScore = thirdHighestScore = 0;
        firstHSPlayerName = secondHSPlayerName = thirdHSPlayerName = "AI";
    }
    else {
        vector<string>* scores = World::getInstance()->splitString(info,'/');
        firstHSPlayerName = QString::fromStdString(scores->at(1));
        firstHighestScore = stoi(scores->at(2));
        secondHSPlayerName = QString::fromStdString(scores->at(3));
        secondHighestScore = stoi(scores->at(4));
        thirdHSPlayerName = QString::fromStdString(scores->at(5));
        thirdHighestScore = stoi(scores->at(6));
    }
    print = true;
}

string HighScore::printData() {
    string data = string("3/") + firstHSPlayerName.toUtf8().constData() + string("/") + to_string(firstHighestScore) + string("/") + secondHSPlayerName.toUtf8().constData() + string("/") + to_string(secondHighestScore) + string("/") + thirdHSPlayerName.toUtf8().constData() + string("/") + to_string(thirdHighestScore);
    print = false;
    return data;
}

void HighScore::exportData() {
    string info = printData();
    ofstream* stream = new ofstream();

    stream->open("high_score.txt");

    if (stream->is_open()){
        *stream << info;
    }

    stream->close();
}
