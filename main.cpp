#include "mainscene.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainScene w;
    w.show();
    //进入消息循环
    return a.exec();
}
