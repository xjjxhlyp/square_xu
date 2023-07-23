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
    bool operator==(const Cell& cell2) {return type == cell2.type;}
    bool operator!=(const Cell& cell2) {return type != cell2.type;}
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
    Point(int r, int c): row(r), col(c) {}
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
    std::vector<Point> rightBoundary(){
        std::vector<Point> res;
        for(int i = 0; i < height(); i++){
            int j = width() - 1;
            while(cells[i][j] != Cell{Cell::Square})j--;
            res.push_back(Point(i, j));
        }
        return res;
    }
    
    std::vector<Point> bottomBoundary(){
        std::vector<Point> res;
        for(int i = 0; i < width(); i++){
            int j = height() - 1;
            while(j >= 0 && cells[j][i] != Cell{Cell::Square}) {
                j--;
            }
            res.push_back(Point(j, i));
        }
        return res;
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
    Up
};

class ActiveShape{
private:
    Point point;
    std::shared_ptr<Shape> shape;
    Point lastPoint;
    std::shared_ptr<Shape> lastShape;
public:
    ActiveShape(Point pt, const std::shared_ptr<Shape>& shapes): point(pt), shape(shapes),lastPoint(pt){}
    void responseCommand(Command cmd);
    void rollback(){
        point = lastPoint;
        shape = lastShape;
    }
    std::vector<Point> activePoints() const;
    
    std::vector<Point> bottomBoundary(){
        return shape->bottomBoundary();
    }
    std::vector<Point> rightBoundary(){
        return shape->rightBoundary();
    }
    bool isInBoundaries(int top, int bottom, int left, int right) const;
    int height(){return shape->height();}
    int width() {return shape->width();}
    
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
        return Point(initRow, initCol);
    }
    int width() {return CellNumberPerRow;}
    int height() {return CellNumberPerCol;}
    bool canJoinInner(const ActiveShape& as);
    bool canJoin(const ActiveShape& as);
    void joinSquare(const ActiveShape& as);
    void cleanSquare(const ActiveShape& as);
    bool canJoinByBoundary(const std::vector<Point>& bottom){
        for (int i = 0; i < bottom.size(); i++) {
            if(cells[bottom[i].row][bottom[i].col] == Cell{Cell::Square}) return false;
        }
        return true;
    }
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
    void adjustAfterRotate(MainScreen &ms, ActiveShape& as){
        int cnt = abs(as.width() - as.height()); //
        int k = cnt;
        
        while(k > 0 && !ms.canJoinByBoundary(as.rightBoundary())){
            as.responseCommand(Left);
            k--;
        }
        
        k = cnt;
        while(k > 0 && !ms.canJoinByBoundary(as.bottomBoundary())){
            as.responseCommand(Up);
            k--;
        }
        
    }
    
    bool response(MainScreen &ms,ActiveShape& as, Command cmd);
    void run();
};
#endif /* game_hpp */
