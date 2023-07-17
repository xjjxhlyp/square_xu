//
//  game.hpp
//  square_xu
//
//  Created by 许京京 on 2023/5/27.
//

#ifndef game_hpp
#define game_hpp

#include <stdio.h>
#include <vector>
#include <iostream>
#include <termios.h>
#include <thread>
#include <unistd.h>
#include <string>
class Cell {
public:
    enum CellType{Unknown, LeftBoundary, RightBoundary, TopBoundary, BottomBoundary, Space, Square};
    Cell(CellType t): type(t) {}
    bool canJoin(const Cell& cell) {return (this->type == Space || cell.type == Space);}
    friend std::ostream& operator<<(std::ostream& out, Cell& cell);
    friend bool operator==(const Cell& cell1, const Cell& cell2) {return cell1.type == cell2.type;}
private:
    CellType type;
};

//枚举也是一个类，是全局的，不能放在函数里边
enum ShapeType{
    Square,
    Lineshape,
    Tshape,
    LLshape,
    RLshape,
    LZshape,
    RZshape,
    ShapeTypeTotal
};

struct Point{
    int row;
    int col;
};

class Shape{
    std::vector<std::vector<Cell>> cells;
protected://只有子类可见
    Shape(std::vector<std::vector<Cell>> squares){
        cells = squares;
    }
public:
    void rotate();
    std::vector<std::vector<Cell>> Cells(){
        return cells;
    }
    std::vector<Point> points();
    int width(){return cells.size();}
    int length(){
        if(cells.size() == 0) return 0;
        return cells[0].size();
    }
};

class SquareShape : public Shape{
public:
    SquareShape():Shape({
        {Cell{Cell::Square}, Cell{Cell::Square}},
        {Cell{Cell::Square}, Cell{Cell::Square}}
    }){}
};

class LineShape : public Shape{
public://子类的构造函数是public，外部才能创建对象
    LineShape(): Shape({
        {Cell{Cell::Square},Cell{Cell::Square},Cell{Cell::Square},Cell{Cell::Square}}
    }){}
};

class TShape : public Shape{
public:
    TShape() : Shape({
        {Cell{Cell::Square}, Cell{Cell::Square},Cell{Cell::Square}},
        {Cell{Cell::Space}, Cell{Cell::Square},Cell{Cell::Space}}
    }){}
};

class LeftLShape : public Shape{
public:
    LeftLShape() : Shape({
        {Cell{Cell::Square},Cell{Cell::Space}},
        {Cell{Cell::Square},Cell{Cell::Space}},
        {Cell{Cell::Square},Cell{Cell::Square}}
    }){}
};

class RightLShape : public Shape{
public:
    RightLShape():Shape({
        {Cell{Cell::Space},Cell{Cell::Square}},
        {Cell{Cell::Space},Cell{Cell::Square}},
        {Cell{Cell::Square},Cell{Cell::Square}}
    }) {}
};

class LeftZShape : public Shape{
public:
    LeftZShape() : Shape({
        {Cell{Cell::Square}, Cell{Cell::Square},Cell{Cell::Space}},
        {Cell{Cell::Space}, Cell{Cell::Square},Cell{Cell::Square}}
    }){}
};

class RightZShape : public Shape{
public:
    RightZShape() : Shape({
        {Cell{Cell::Space}, Cell{Cell::Square},Cell{Cell::Square}},
        {Cell{Cell::Square}, Cell{Cell::Square},Cell{Cell::Space}}
    }){}
};

const int down = 66;
const int left = 68;
const int right = 67;
const int downToBottom = 32;
const int roate = 65;
enum Command {
    Unknown,
    Down = down,
    Left = left,
    Right = right,
    Rotate = roate,
    DownToBottom = downToBottom,
};


class ActiveShape{
private:
    Point point;
    std::shared_ptr<Shape> shape;
    Point lastPoint;
public:
    ActiveShape(Point pt, const std::shared_ptr<Shape>& shapes): point(pt), shape(shapes),lastPoint(pt){}
    void move(Command cmd){
        lastPoint = point;
        switch(cmd) {
        case Down:
            lastPoint.row = point.row;
            point.row++;
            break;
        case Left:
            lastPoint.col = point.col;
            point.col--;
            break;
        case Right:
            lastPoint.col = point.col;
            point.col++;
            break;
        }
    }
    void rollback(){
        point = lastPoint;
    }
    std::vector<Point> activePoints() const;
    bool isInBoundaries(int top, int bottom, int left, int right) const;
};


class MainScene {
private:
    const int CellNumberPerRow = 12;
    const int CellNumberPerCol = 22;
    const int initRow = 1;
    const int initCol = 4;
public:
    std::vector<std::vector<Cell>> cells;
    MainScene();
    Point initShapePoint(){
        Point pt;
        pt.row = initRow;
        pt.col = initCol;
        return pt;
    }
    bool canJoin(const ActiveShape& as){
        if (!as.isInBoundaries(0,CellNumberPerCol,0, CellNumberPerRow)) return false;
        std::vector<Point> vp = as.activePoints();
        for(int i = 0; i < vp.size(); i++){
            if(cells[vp[i].row][vp[i].col] == Cell{Cell::Square}) return false;
        }
        return true;
    }
    void joinSquare(const ActiveShape& as);
    void cleanSquare(const ActiveShape& as);
    void printScreen();
private:
    void print();
    bool canRemove(int row);
    void RemoveOneRow(int row);
};

std::shared_ptr<Shape> createShape(ShapeType shapeType);

class UserCommand{
private:
    std::mutex mtx;
    Command cmd;
private:
    char getchar_no_output();
    //在类内使用线程，要用static修饰改函数
    void receiveCommand();
public:
    UserCommand(){
        cmd = Unknown;
    }
    Command getCmd();
    void beginReceiveCmd(){
        std::thread th(&UserCommand::receiveCommand, this);
        th.detach();
    }
};

class Game{
public:
    ShapeType randomShape();
public:
    void move(MainScene& ms, ActiveShape& as, Command cmd){
         ms.cleanSquare(as);
         as.move(cmd);
         if (!ms.canJoin(as)) {
             as.rollback();
         }
         ms.joinSquare(as);
     }
    void run();
};
#endif /* game_hpp */
