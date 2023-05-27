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
    int type;
public:
    Cell(int t): type(t) {}
    bool canJoin(const Cell& cell) {return (this->type == 5 || cell.type == 5);}
    friend std::ostream& operator<<(std::ostream& out, Cell& cell);
    friend bool operator==(const Cell& cell1, const Cell& cell2) {return cell1.type == cell2.type;}
};

class SquareShape {
public:
    std::vector<std::vector<Cell>> cells;
    SquareShape() {
        cells = {{Cell{6}, Cell{6}}, {Cell{6}, Cell{6}}};
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
};

class Move {
    
public:
    enum Direction {
        Down, Left, Right
    };
    void move(MainScene& ms, const std::vector<std::vector<Cell>>& squares, int x, int y, Direction di);
};

#endif /* game_hpp */
