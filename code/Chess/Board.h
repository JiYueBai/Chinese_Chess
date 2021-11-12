#ifndef BOARD_H
#define BOARD_H


#include <QWidget>
#include "Stone.h"
#include "Step.h"
#include <QVector>
class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = nullptr);
    //game datas
    Stone stone[32];
    int _selectid;
    int _r;//棋子半径
    bool _bRedTurn;

    QVector<Step*> _steps;

    void paintEvent(QPaintEvent *);
    void drawStone(QPainter &painter,int id);
    void mouseReleaseEvent(QMouseEvent*);
    
    //返回棋盘每个点对应的像素坐标
    QPoint center(int row,int col);
    QPoint center(int id);
    bool getRowCol(QPoint pt,int& row,int& col);

    bool canMove(int moveid,int row,int col,int killid);
    bool canMoveJiang(int moveid,int row,int col,int killid);
    bool canMoveShi(int moveid,int row,int col,int killid);
    bool canMoveXiang(int moveid,int row,int col,int killid);
    bool canMovePao(int moveid,int row,int col,int killid);
    bool canMoveMa(int moveid,int row,int col,int killid);
    bool canMoveChe(int moveid,int row,int col,int killid);
    bool canMoveBing(int moveid,int row,int col,int killid);

    //int relation(int row1,int col1,int row2,int col2);
    int getStoneId(int row,int col);
    int getStoneCountAtLine(int row1,int col1,int row2,int col2);
    bool isDead(int id);


signals:

};

#endif // BOARD_H
