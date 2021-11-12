#ifndef STONE_H
#define STONE_H
#include <QString>
class Stone
{
public:
    Stone();
    ~Stone();

    enum TYPE{JIANG,MA,CHE,XIANG,SHI,PAO,BING};
    void init(int id);

    int _row;
    int _col;
    TYPE _type;

    int _id;
    bool _dead;
    bool _red;

    QString name();

    void rotate();

};

#endif // STONE_H
