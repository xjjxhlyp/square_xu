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
#include <cstdlib>
#include <ctime>
#include <queue>

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
    YiShape,
    TtShape,
    LLShape,
    RLShape,
    LZShape,
    RZShape,
    ShapeTotal
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
    }){
    }
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

class MainScene {
    const int CellNumberPerRow = 12;
    const int CellNumberPerCol = 22;
public:
    enum EvenType{
        UserInput,
        TimedFall
    };
    
public:
    std::vector<std::vector<Cell>> cells;
public:
    MainScene();
    bool canJoin(std::vector<std::vector<Cell>> squares, int x, int y);
    void joinSquare(std::vector<std::vector<Cell>> squares, int x, int y);
    void cleanSquare(std::vector<std::vector<Cell>> squares, int x, int y);
    void print();
    void printScreen(EvenType et);
private:
    bool canRemove(int row);
    void RemoveOneRow(int row);
};

class Move {
public:
    enum Direction {
        Down, Left, Right
    };
    bool move(MainScene& ms, const std::vector<std::vector<Cell>>& squares, int x, int y, Direction di);
};

std::shared_ptr<Shape> createShape();

enum UserCommandType{
    ToDown = 66,
    ToLeft = 68,
    ToRight = 67,
    ToRotate = 65,
    ToBottom = 32
};

class UserCommand{
private:
    std::mutex mtx;
    std::queue<int> userComannds;
private:
    char getchar_no_output();
    void receiveCmd();
public:
    UserCommand(){
        std::thread th(&UserCommand::receiveCmd, this);
        th.detach();
    }
    int getCmd();
    void reponseCmd(MainScene&ms, Move mo, std::shared_ptr<Shape> shapes, int &row, int &col);
};



class Game{
    ShapeType randomShape(){
        return static_cast<ShapeType>(time(0) % ShapeType::ShapeTotal);// 输出随机选择的枚举值
    }
public:
    void run();
};

#endif /* game_hpp */
