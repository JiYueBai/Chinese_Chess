#include <QApplication>
#include "Singlegame.h"

int main(int argc,char* argv[])
{
    QApplication app(argc,argv);
    SingleGame board;
    board.show();
    return app.exec();
}
