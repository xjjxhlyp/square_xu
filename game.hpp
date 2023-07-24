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
#include <queue>
#include <condition_variable>
class Cell {
public:
    enum CellType{Unknown, LeftBoundary, RightBoundary, TopBoundary, BottomBoundary, Space, Square};
    Cell(CellType t): type(t) {}
private:
    CellType type;
public:
    bool canJoin(const Cell& cell) {return (this->type == Space || cell.type == Space);}
    friend std::ostream& operator<<(std::ostream& out, Cell& cell);
    bool operator==(const Cell& cell2) {return type == cell2.type;}
    bool operator!=(const Cell& cell2) {return type != cell2.type;}
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
    Point(int r, int c): row(r), col(c) {}
};

class Shape{
private:
    std::vector<std::vector<Cell>> cells;
    std::vector<std::vector<Cell>> lastCells;
protected://只有子类可见
    Shape(std::vector<std::vector<Cell>> squares){cells = squares;}
public:
    void rotate();
    void rollbackAfterRotate(){cells = lastCells;}
    std::vector<Point> points();
    int height(){return (int)cells.size();}
    int width(){
        if(cells.size() == 0) return 0;
        return (int)cells[0].size();
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

enum Command {
    Unknown,
    Down,
    Left,
    Right,
    Rotate,
    DownToBottom
};

class ActiveShape{
private:
    Point point;
    std::shared_ptr<Shape> shape;
    
    Point lastPoint;
    std::shared_ptr<Shape> lastShape;
public:
    ActiveShape(Point pt, const std::shared_ptr<Shape>& shapes): point(pt), shape(shapes),lastPoint(pt),lastShape(shapes){}
    void responseCommand(Command cmd);
    void rollback(Command cmd);
    std::vector<Point> activePoints() const;
    bool isInBoundaries(int top, int bottom, int left, int right) const;
};

class MainScreen {
private:
    const int CellNumberPerRow = 12;
    const int CellNumberPerCol = 22;
    
    const int initRow = 1;
    const int initCol = 4;
    
    const int nextBegin = 2;
    const int scoresBegin = 10;
    const int levelBegin = 15;

    int score = 0;
    int level = 1;
    
public:
    std::vector<std::vector<Cell>> cells;
    MainScreen();
    Point initShapePoint(){return Point(initRow, initCol);}
    int width() {return CellNumberPerRow;}
    int height() {return CellNumberPerCol;}
    bool canJoin(const ActiveShape& as);
    void joinSquare(const ActiveShape& as);
    void printScore(){
        std::cout << "  ";
        std::cout << "scores : " << score;
    }
    void printLevel(){
        std::cout << "  ";
        std::cout << "level : " << level;
    }
    void printScreen(const ActiveShape& as);
    
private:
    bool canJoinInner(const ActiveShape& as);
    bool canRemove(int row);
    void RemoveOneRow(int row);
};

std::shared_ptr<Shape> createShape(ShapeType shapeType);

class UserCommand{
private:
    std::mutex mtx;
    std::condition_variable cv;
    std::queue<Command> cmds;
    
    int micro_seconds;
    std::mutex mtx_sleep;
private:
    char getchar_no_output();
    void receiveCommand();
    Command transformInputToCommand(char ch);
    void downPeriodly();
public:
    UserCommand(int period):micro_seconds(period){};
    Command getCmd();
    void generateCmds();
    void changeDownPeriod(int downPeriod){
        std::unique_lock<std::mutex> ulPeriod(mtx_sleep);
        micro_seconds = downPeriod;
    }
    int getDownPeriod(){
        std::unique_lock<std::mutex> ulPeriod(mtx_sleep);
        return micro_seconds;
    }
};

class Game{
public:
    ShapeType randomShape();
public:
    bool response(MainScreen &ms,ActiveShape& as, Command cmd);
    void downToBottom(MainScreen &ms,ActiveShape& as);
    void run();
};
#endif /* game_hpp */
