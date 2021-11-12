#include "Board.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
Board::Board(QWidget *parent) : QWidget(parent)
{
    for(int i=0;i<32;++i)
    {
        stone[i].init(i);
    }
    _selectid = -1;
    _bRedTurn = true;
}
void Board::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int d = 40;
    _r = d/2;
    //棋盘的横线10条
    for(int i=1;i <= 10;++i)
    {
        painter.drawLine(QPoint(d,i*d),QPoint(9*d,i*d));
    }
    //棋盘的竖线9条
    for(int i=1;i <=9;++i)
    {
        if(i == 1||i == 9)
        {
            painter.drawLine(QPoint(i*d,d),QPoint(i*d,10*d));
        }
        else
        {
            painter.drawLine(QPoint(i*d,d),QPoint(i*d,5*d));
            painter.drawLine(QPoint(i*d,6*d),QPoint(i*d,10*d));
        }

    }
    //士将框
    painter.drawLine(QPoint(4*d,d),QPoint(6*d,3*d));
    painter.drawLine(QPoint(6*d,d),QPoint(4*d,3*d));
    painter.drawLine(QPoint(4*d,10*d),QPoint(6*d,8*d));
    painter.drawLine(QPoint(6*d,10*d),QPoint(4*d,8*d));

    //绘制32颗棋子
    for(int i=0;i < 32;++i)
    {
        drawStone(painter,i);
    }

}
//判断点击的点在棋盘哪里
QPoint Board::center(int row, int col)
{
    QPoint ret;
    ret.rx() = (col+1)* _r*2;
    ret.ry() = (row+1)* _r*2;
    return ret;
}
QPoint Board::center(int id)
{
    return center(stone[id]._row,stone[id]._col);
}
void Board::drawStone(QPainter& painter,int id)
{
    if(stone[id]._dead)
        return;
    QPoint c = center(id);
    QRect rect = QRect(c.x() -_r,c.y()-_r,_r*2,_r*2);

    if(id == _selectid)
        painter.setBrush(QBrush(Qt::gray));
    else
        painter.setBrush(QBrush(Qt::yellow));
    painter.setPen(Qt::black);
    painter.drawEllipse(center(stone[id]._row,stone[id]._col),_r,_r);
    if(stone[id]._red)
        painter.setPen(Qt::red);
    painter.setFont(QFont("SimHei",_r,700));

    painter.drawText(rect,stone[id].getText(),QTextOption(Qt::AlignCenter));
}
//效率低
bool Board::getRowCol(QPoint pt, int &row, int &col)
{
    for(row = 0;row <= 9;row++)
    {
        for(col = 0;col <= 8;col++)
        {
            QPoint c = center(row,col);
            int dx = c.x() - pt.x();
            int dy = c.y() - pt.y();
            int dist = dx*dx + dy*dy;
            if(dist < _r*_r)
                return true;
        }
    }
    return false;
}

bool Board::isDead(int id)
{
    if(id == -1)    return true;
    return stone[id]._dead;
}

int Board::getStoneId(int row,int col)
{
    for(int i=0;i<32;++i)
    {
        if(stone[i]._row == row && stone[i]._col == col && !isDead(i))
            return i;
    }
    return -1;
}
//判断在同一直线上是否有其他棋子
int Board::getStoneCountAtLine(int row1,int col1,int row2,int col2)
{
    int ret = 0;
    if(row1 != row2 && col1 != col2)
        return -1;
    if(row1 == row2 && col1 == col2)
        return -1;
    if(row1 == row2)
    {
        int min = col1 < col2 ? col1 : col2;
        int max = col1 < col2 ? col2 : col1;
        for(int col = min + 1;col < max;++col)
        {
            if(getStoneId(row1,col) != -1)
            {
                ++ret;
            }
        }
    }
    else
    {
        int min = row1 < row2 ? row1 : row2;
        int max = row1 < row2 ? row2 : row1;
        for(int row = min + 1;row < max;++row)
        {
            if(getStoneId(row,col1) != -1)
            {
                ++ret;
            }
        }
    }
    return ret;
}
bool Board::canMove(int moveid, int row, int col, int killid)
{
    if(stone[moveid]._red == stone[killid]._red)
    {
        //颜色相同换选择的棋子
        _selectid = killid;
        update();
        return false;
    }
    switch (stone[moveid]._type)
    {
        case Stone::JIANG:
            return canMoveJiang(moveid,row,col,killid);
            break;
        case Stone::SHI:
            return canMoveShi(moveid,row,col,killid);
            break;
        case Stone::CHE:
            return canMoveChe(moveid,row,col,killid);
            break;
        case Stone::MA:
            return canMoveMa(moveid,row,col,killid);
            break;
        case Stone::XIANG:
            return canMoveXiang(moveid,row,col,killid);
            break;
        case Stone::PAO:
            return canMovePao(moveid,row,col,killid);
            break;
        case Stone::BING:
            return canMoveBing(moveid,row,col,killid);
            break;
    }
    return true;
}

/*int Board::relation(int row1, int col1, int row2, int col2)
{
    return qAbs(row1-row2)*10 + qAbs(col1-col2);
}*/
//将的移动逻辑
bool Board::canMoveJiang(int moveid,int row,int col,int killid)
{
    /*
        "将"的走棋规则如下：
        1.移动范围在九宫格内
        2.每次只能在棋盘上移动一格，可向任意方向(上下左右)移动
        3.双方的将在同一直线上且它们之间没有其他棋子阻隔，则可以直接飞跃吃掉对方的将
    */
    if(stone[moveid]._red)
    {
        if(row > 2)    return false;
    }
    else
    {
        if(row < 7)    return false;
    }
    if(col < 3)
        return false;
    if(col > 5)
        return false;
    int drow = stone[moveid]._row - row;
    int dcol = stone[moveid]._col - col;
    int d = abs(drow)*10 + abs(dcol);
    qDebug()<<d;
    if(d == 1 || d == 10)
        return true;
    return false;
}
//士的移动逻辑
bool Board::canMoveShi(int moveid,int row,int col,int killid)
{
    if(stone[moveid]._red)
    {
        if(row > 2)    return false;
    }
    else
    {
        if(row < 7)    return false;
    }
    if(col < 3)
        return false;
    if(col > 5)
        return false;
    int drow = stone[moveid]._row - row;
    int dcol = stone[moveid]._col - col;
    int d = abs(drow)*10 + abs(dcol);
    if(d == 11)
        return true;
    return false;
}
//象的移动逻辑
bool Board::canMoveXiang(int moveid,int row,int col,int killid)
{
    int drow = stone[moveid]._row - row;
    int dcol = stone[moveid]._col - col;
    int d = abs(drow)*10 + abs(dcol);
    if(d != 22)
        return false;
    int rEye = (stone[moveid]._row + row)/2;
    int cEye = (stone[moveid]._col + col)/2;
    if(getStoneId(rEye,cEye) != -1)
        return false;
    return true;
}
//炮的移动逻辑
bool Board::canMovePao(int moveid,int row,int col,int killid)
{
    int retchess = getStoneCountAtLine(stone[moveid]._row,stone[moveid]._col,row,col);
    if(retchess == 1||retchess == 0)
        return true;
    return false;
}
//马的移动逻辑
bool Board::canMoveMa(int moveid,int row,int col,int killid)
{
    int drow = stone[moveid]._row - row;
    int dcol = stone[moveid]._col - col;
    int d = abs(drow)*10 + abs(dcol);
    if(d != 21 && d != 12)
        return false;
    else if(d == 12)
    {
        int rma = (stone[moveid]._col + col)/2;
        if(getStoneId(stone[moveid]._row,rma) != -1)
        {
            return false;
        }

    }
    else
    {
        int cma = (stone[moveid]._row + row)/2;
        if(getStoneId(cma,stone[moveid]._col) != -1)
            return false;
    }
    return true;
}
//车的移动逻辑
bool Board::canMoveChe(int moveid,int row,int col,int killid)
{
    int retchess = getStoneCountAtLine(stone[moveid]._row,stone[moveid]._col,row,col);
    if(retchess == 0)
        return true;
    return false;
}
//兵的移动逻辑
bool Board::canMoveBing(int moveid,int row,int col,int killid)
{
    int drow = stone[moveid]._row - row;
    int dcol = stone[moveid]._col - col;
    int d = abs(drow)*10 + abs(dcol);
    if(d != 1 && d != 10)
        return false;
    if(stone[moveid]._red)
    {
        if(stone[moveid]._row > row)
            return false;
        if(stone[moveid]._row < 5)
        {
            if(stone[moveid]._col != col)
                return false;
        }
    }
    else
    {
        if(stone[moveid]._row < row )
            return false;
        if(stone[moveid]._row > 5 )
        {
            if(stone[moveid]._col != col)
                return false;
        }
            \
    }
}

void Board::mouseReleaseEvent(QMouseEvent* ev)
{
    QPoint pt = ev->pos();
    //将pt转化成棋盘的行列，然后判断其上有没有棋子，有就选中
    int row,col;
    bool bRet = getRowCol(pt,row,col);
    if(bRet == false)
        return;
    int i;
    int clickid = -1;
    for(i=0;i<32;++i)
    {
        if(stone[i]._row == row && stone[i]._col == col && stone[i]._dead == false)
        {
            break;
        }
    }
    if(i<32)
    {
        clickid = i;
    }
    if(_selectid == -1)
    {
        if(clickid != -1)
        {
            if(_bRedTurn == stone[clickid]._red)
            {
                _selectid = clickid;
                update();
            }
        }
    }
    else
    {
        if(canMove(_selectid,row,col,clickid))
        {
            //走棋
            stone[_selectid]._row = row;
            stone[_selectid]._col = col;
            if(clickid != -1)
            {
                stone[clickid]._dead = true;
            }
            _selectid = -1;
            _bRedTurn = !_bRedTurn;
            update();
        }

    }
}
