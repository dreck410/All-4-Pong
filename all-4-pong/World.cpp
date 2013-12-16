
#include <cassert>
#include "highscores.h"
#include "World.h"
#include "Object.h"
#include "Paddle.h"
#include "Shapes.h"
#include "Ball.h"
World* World::instance=NULL;

QString World::getBlock(){
    if(PopUps)
        counter++;
    if(counter == 500){
        counter = 0;
        // qDebug()<<"make a random object"<< endl;

        return "Shapes";
    }
    return NULL;
}


// update all elements in this game world
void World::UpdateWorld() {
    foreach (Object* obj, objects) {
        obj->updatePosition();
    }


}


// reset all elements in this game world
void World::ResetWorld() {
    for( int i = 0; i < GamePlayers.size();++i){
        delete GamePlayers.at(i);
    }

    for( int i = 0; i < objects.size();++i){
        delete objects.at(i);
    }
    GamePlayers.clear();
    objects.clear();
    balls.clear();
    gameIsOver = false;
    roundFinished = false;
    numberDead = 0;

}

// prints the current world state out to offshore text file,
// returning a boolean value indicating print success
void World::printWorldInfo() {


    ofstream* stream = new ofstream;

    stream->open("save_file.txt");

    if(stream->is_open()){

        *stream << "/" << PopUps << "/" << difficulty << "/" << endl;
        for(int i = 0; i < objects.size(); i++){
            objects.at(i)->setInfo(stream);
        }

        // establish connection with text file
        stream->close();

    }



}

vector<Shapes*> World::getBlocks(){
    vector<Shapes*> blocks;
    for(int i = 0; i < objects.size(); i++){
        Shapes* shape = dynamic_cast<Shapes*>(objects.at(i));
        if(shape != NULL){
            blocks.push_back(shape);
        }
    }
    return blocks;
}

// reads the current world state from offshore text file,
// returning a boolean value indicaing read success;
// if read succeeds, stores world state in instance variables
void World::readWorldInfo() {
    information.clear();


    ofstream* stream = new ofstream();
    for(int i = 0; i < objects.size();i++){
        objects.at(i)->getInfo(stream);


    }

    fstream* inFile = new fstream;
    inFile->open("save_file.txt");

    if(inFile->eofbit){
        information.push_back("");
        for(int j = 0; ;information.push_back(""),j++){

            getline(*inFile, information.at(j));
            if(information.at(j) == "")break;
        }
        inFile->close();
    }else{//failed
    }



    vector<string>* item;


    item = splitString(information.at(0),'/');
    setPopUps(stoi(item->at(0)));
    setDifficulty(stoi(item->at(1)));

    int numPlayers = 0;
    for(int i= 1; i < 5;i++){
        item = splitString(information.at(i), '/');
        if(item->at(6) != "AI")numPlayers++;
    }

    setupPlayers(numPlayers);

    //Ball/225/225/-3/9/-1/
    for(int i = 1; i < information.size() - 1;i++){
        item = splitString(information.at(i),'/');

        Object* obj;//
        string identifier = item->at(0);
        if(identifier == "shape"){

            obj = new Shapes(true);


        }
        if(objects.at(i-1)->getType() == QString::fromStdString(item->at(0))){
            objects.at(i - 1)->getInfo(item);
        }

    }

}




void World::setupPlayers(int num) {
    numberDead = 4;

    for (int i = 0; i < num && i<4; ++i){
        Player *thisUser = new User();
        GamePlayers.push_back(thisUser);
        thisUser->setUsername("User");
        numberDead--;

    }

    if (4 - num > 0){
        for (int i = 0; i< 4 - num; ++i) {
            Player *thisPlayer = new AI(difficulty);
            GamePlayers.push_back(thisPlayer);
            thisPlayer->setUsername("AI");
            numberDead--;
        }
    }
}


void World::pointScoredReset() {
    foreach (Player *play, GamePlayers) {
        play->reset();
    }
    if(numberDead < 3){
        roundEnd = "Round Over";
        Instructions = "Click to Continue";
    }
}

void World::gameOver(){
    //game over logic.
    //ends the game
    //saves high scores
    if(numberDead == 3){
        roundFinished = true;
        //displays gameover text.
        roundEnd = "Game Over";
        Instructions = "Click to go Home";
        gameIsOver = true;
        foreach(Player *play, GamePlayers) {
            HighScore::getInstance()->addScore(play->getUsername(),play->getCurrentScore()->getCurrentScore());
        }
        HighScore::getInstance()->exportData();
    }
}



QString World::getNetwork(){
    QString netString;
    for (int i = 0; i < objects.size(); ++i){
        Object* obj = objects.at(i);
        //testing
        obj->getType();

        netString += obj->getNet();

        //qDebug() << netString << "\n";
    }
    if (HighScore::getInstance()->getBool() == true){
        netString += QString::fromStdString(HighScore::getInstance()->printData());
    }
    return netString;
}

vector<string>* World::splitString(string str, char delim){
    vector<string> *splitV = new vector<string>();
    string buf = "";
    int i = 0;
    while (i < str.size()){
        if (str.at(i) != delim){
            buf += str.at(i);
        } else if (buf.length() > 0) {
            splitV->push_back(buf);
            buf = "";
        }
        i++;
    }
    if (buf != "")
        splitV->push_back(buf);
    return splitV;
}

vector<QString> *World::split(QString str, char delim){
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

void World::updateUser(QString str){
    vector<QString> *splitV = split(str, '/');
    Player *thisUser = getGamePlayer(splitV->at(0).toInt());

    thisUser->setUsername(splitV->at(1));
    thisUser->setPoint(splitV->at(2).toInt(), splitV->at(3).toInt());


}

void World::setPlayerName(QString name, int side){
    Player *inPlayer = getGamePlayer(side);
    if(name == "")name = "User";
    if(inPlayer != NULL)
        inPlayer->setUsername(name);
}


