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

enum Direction {
    Down, Left, Right
};

class ActiveShape{
private:
    Point point;
    std::shared_ptr<Shape> shape;
public:
    ActiveShape(Point pt, const std::shared_ptr<Shape>& shapes): point(pt), shape(shapes){}
    std::vector<Point> activePoints() const;
    bool isInBoundaries(int top, int bottom, int left, int right) const;
};

class MainScene {
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


class Move {
    enum Direction {
        Down, Left, Right
    };
public:
    bool move(MainScene& ms, const std::vector<std::vector<Cell>>& squares, int x, int y, Direction di);
};

std::shared_ptr<Shape> createShape(ShapeType shapeType);

class UserCommand{
private:
    std::mutex mtx;
    int cmd;
private:
    char getchar_no_output();
    //在类内使用线程，要用static修饰改函数
    void receiveCommand();
public:
    UserCommand(){
        cmd = 0;
    }
    int getCmd();
    void beginReceiveCmd(){
        std::thread th(&UserCommand::receiveCommand, this);
        th.detach();
    }
};

class Game{
private:
    ShapeType randomShape();
public:
    void run();
};
#endif /* game_hpp */
