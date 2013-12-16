#include "startup.h"
#include <QApplication>
#include <QTcpSocket>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Startup w;
    w.show();
    
    return a.exec();
}
