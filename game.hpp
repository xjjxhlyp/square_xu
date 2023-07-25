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
    enum CellType{Unknown, LeftBoundary, RightBoundary, TopBoundary, BottomBoundary, Space, Square,NormalString};
    Cell(CellType t): type(t) {}
    Cell(const std::string& s):str(s), type(NormalString){}
private:
    CellType type;
    std::string str;
public:
    bool canJoin(const Cell& cell) {return (this->type == Space || cell.type == Space);}
    bool isSquare() const {return type == Square;}
    bool isSpace() const {return type == Space;}
    void set(const CellType& t) {type = t;}
    friend std::ostream& operator<<(std::ostream& out, Cell& cell);
    bool operator==(const Cell& cell2) const {return type == cell2.type;}
    bool operator!=(const Cell& cell2) const {return type != cell2.type;}
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
    Shape(std::vector<std::vector<Cell>> squares){cells = squares;}
public:
    void rotate();
    std::vector<Point> points() const;
    int height() const {return (int)cells.size();}
    int width() const {
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
    Shape shape;
    
    Point lastPoint;
    Shape lastShape;
public:
    ActiveShape(Point pt, const Shape& shapes): point(pt), shape(shapes),lastPoint(pt),lastShape(shapes){}
    void responseCommand(Command cmd);
    void rollback();
    std::vector<Point> activePoints()const;
    bool isInBoundaries(int top, int bottom, int left, int right) const;
};

class MainScreen {
private:
    const int RowNumbers = 22;
    const int ColNumbers = 12;
    
    const int InitRow = 1;
    const int InitCol = 4;
    
    const int BackColNumbers = 6;
    const int NextBegin = 3;
    const int ScoreBegin = 10;
    const int LevelBegin = 15;
    
    int score = 0;
    int level = 1;
    
public:
    std::vector<std::vector<Cell>> cells;
    MainScreen();
    Point initShapePoint(){return Point(InitRow, InitCol);}
    int width() {return ColNumbers;}
    int height() {return RowNumbers;}
    bool canJoin(const ActiveShape& as);
    void joinSquare(const ActiveShape& as);
    void printScreen(const ActiveShape& as, const ActiveShape& nextAs);
    void remove();
private:
    void joinLevel(std::vector<std::vector<Cell>>& currCells, const Point pt, const Cell& cell);
    void joinActiveShape(std::vector<std::vector<Cell>>& cells, const std::vector<Point>& p, Point point);
    bool canJoinInner(const ActiveShape& as);
    bool rowCanRemove(int row);
    void aboveCellsFall(int row);
    
};

Shape createShape(ShapeType shapeType);

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
    bool response(MainScreen &ms,ActiveShape& as, Command cmd);
    void downToBottom(MainScreen &ms,ActiveShape& as);
    
public:
    void run(){
        MainScreen ms;
        UserCommand uc(300000);
        uc.generateCmds();
        ActiveShape currAs(ms.initShapePoint(),createShape(randomShape()));
        while(true){
            ActiveShape nextAs(ms.initShapePoint(),createShape(randomShape()));
            if(!ms.canJoin(currAs)) {
                printf("\033[01;34m Game Over!");
                break;
            }
            ms.printScreen(currAs, nextAs);
            bool stop = false;
            while(!stop){
                Command cmd = uc.getCmd();
                stop = response(ms, currAs, cmd);
                ms.printScreen(currAs, nextAs);
            }
            ms.joinSquare(currAs);
            ms.remove();
            ms.printScreen(currAs, nextAs);
            currAs = nextAs;
        }
    };
};
#endif /* game_hpp */
