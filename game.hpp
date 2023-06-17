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
enum ShapeType{Square, Linshape, Tshape, LLshape, RLshape, LZshape, RZshape};
class SquareShape {
public:
    std::vector<std::vector<Cell>> cells;
    SquareShape() {
        cells = {
            {Cell{Cell::Square}, Cell{Cell::Square}},
            {Cell{Cell::Square}, Cell{Cell::Square}}
        };
    }
    
};

class LineShape{
public:
    std::vector<std::vector<Cell>> cells;
    LineShape(){
        cells = {
            {Cell{Cell::Square},Cell{Cell::Square},Cell{Cell::Square},Cell{Cell::Square}}
        };
    }
};

class TShape{
public:
    std::vector<std::vector<Cell>> cells;
    TShape(){
        cells = {
            {Cell{Cell::Square}, Cell{Cell::Square},Cell{Cell::Square}},
            {Cell{Cell::Space}, Cell{Cell::Square},Cell{Cell::Space}}
        };
    }
};

class LeftLShape{
public:
    std::vector<std::vector<Cell>> cells;
    LeftLShape() {
        cells = {
            {Cell{Cell::Square},Cell{Cell::Space}},
            {Cell{Cell::Square},Cell{Cell::Space}},
            {Cell{Cell::Square},Cell{Cell::Square}}
        };
    }
};

class RightLShape{
public:
    std::vector<std::vector<Cell>> cells;
    RightLShape() {
        cells = {
            {Cell{Cell::Space},Cell{Cell::Square}},
            {Cell{Cell::Space},Cell{Cell::Square}},
            {Cell{Cell::Square},Cell{Cell::Square}}
        };
    }
};




class LeftZShape{
public:
    std::vector<std::vector<Cell>> cells;
    LeftZShape(){
        cells = {
            {Cell{Cell::Square}, Cell{Cell::Square},Cell{Cell::Space}},
            {Cell{Cell::Space}, Cell{Cell::Square},Cell{Cell::Square}}
        };
    }
};

class RightZShape{
public:
    std::vector<std::vector<Cell>> cells;
    RightZShape(){
        cells = {
            {Cell{Cell::Space}, Cell{Cell::Square},Cell{Cell::Square}},
            {Cell{Cell::Square}, Cell{Cell::Square},Cell{Cell::Space}}
        };
    }
};

class MainScene {
    const int CellNumberPerRow = 20;
    const int CellNumberPerCol = 30;
public:
    std::vector<std::vector<Cell>> cells;
    
    MainScene();
    bool canJoin(std::vector<std::vector<Cell>> squares, int x, int y);
    void joinSquare(std::vector<std::vector<Cell>> squares, int x, int y);
    void cleanSquare(std::vector<std::vector<Cell>> squares, int x, int y);
    void print();
private:
    bool canRemove(int row);
    void RemoveOneRow(int row);
};

class Move {
public:
    enum Direction {
        Down, Left, Right
    };
    void move(MainScene& ms, const std::vector<std::vector<Cell>>& squares, int x, int y, Direction di);
};


#endif /* game_hpp */
