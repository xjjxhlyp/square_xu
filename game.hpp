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
private:
    std::vector<std::vector<Cell>> cells;
protected://只有子类可见
    Shape(std::vector<std::vector<Cell>> squares){
        cells = squares;
    }
public:
    void rotate();
    std::vector<Point> points();
    int height(){return cells.size();}
    int width(){
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

enum Command {
    Unknown,
    Down,
    Left,
    Right,
    Rotate,
    DownToBottom,
};

class ActiveShape{
private:
    Point point;
    std::shared_ptr<Shape> shape;
    Point lastPoint;
    std::shared_ptr<Shape> lastShape;
public:
    ActiveShape(Point pt, const std::shared_ptr<Shape>& shapes): point(pt), shape(shapes),lastPoint(pt){}
    
    void rotate(int rightBoundary, int bottomBoundary){
        shape->rotate();
        while(point.col + shape->width()>= rightBoundary) point.col--;
        while(point.row + shape->height()>= bottomBoundary) point.row--;
    }
    
    void downToBottom(int bottomBoundary){
        while(point.row + shape->height()< bottomBoundary  - 1) point.row++;
    }
    void responseCommand(Command cmd, int rightBoundary, int bottomBoundary){
        lastPoint = point;
        lastShape = shape;
        switch(cmd) {
            case Down:
                point.row++;
                break;
            case Left:
                point.col--;
                break;
            case Right:
                point.col++;
                break;
            case Rotate:
                rotate(rightBoundary, bottomBoundary);
                break;
            case DownToBottom:
                downToBottom(bottomBoundary);
                break;
        }
    }
    
    void rollback(){
        point = lastPoint;
        shape = lastShape;
    }
    
    std::vector<Point> activePoints() const;
    bool isInBoundaries(int top, int bottom, int left, int right) const;
};

class MainScreen {
private:
    const int CellNumberPerRow = 12;
    const int CellNumberPerCol = 22;
    const int initRow = 1;
    const int initCol = 4;
public:
    std::vector<std::vector<Cell>> cells;
    MainScreen();
    Point initShapePoint(){
        Point pt;
        pt.row = initRow;
        pt.col = initCol;
        return pt;
    }
    int width() {return CellNumberPerRow;}
    int height() {return CellNumberPerCol;}
    bool canJoin(const ActiveShape& as);
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
    std::condition_variable cv;
    std::queue<Command> cmds;
private:
    char getchar_no_output();
    void receiveCommand();
public:
    Command getCmd();
    void beginReceiveCmd(){
        std::thread th(&UserCommand::receiveCommand, this);
        th.detach();
    }
    Command transformInputToCommand(char ch);
};

class Game{

public:
    ShapeType randomShape();
public:
    void response(MainScreen& ms, ActiveShape& as, Command cmd);
    
    
    void run();
};
#endif /* game_hpp */
