//
//  game.cpp
//  square_xu
//
//  Created by 许京京 on 2023/5/27.
//

#include "game.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <stdio.h>
#include <unistd.h>//包含sleep()的头文件
#include <vector>

std::ostream& operator<<(std::ostream& out, Cell& cell) {
    if (cell.type == 1) {
        std::cout << " |";
    } else if (cell.type == 2) {
        std::cout << "| ";
    } else if (cell.type == 3) {
        std::cout << "__";
    } else if (cell.type == 4) {
        std::cout << "--";
    } else if (cell.type == 5) {
        std::cout << "  " ;
    } else if (cell.type == 6) {
        std::cout << "🟥";
    }
    return out;
}


MainScene::MainScene() {
    cells.resize(CellNumberPerCol);
        
    cells[0] = std::vector<Cell>(CellNumberPerRow, Cell{3});
    for (int i = 1; i < CellNumberPerCol - 1; i++) {
        cells[i].push_back(Cell{1});
        for (int j = 1; j < CellNumberPerRow - 1; j++) {
            cells[i].push_back(Cell{5});
        }
        cells[i].push_back(Cell{2});
    }
    cells.back() = std::vector<Cell>(CellNumberPerRow, Cell{4});
}
    
bool MainScene::canJoin(std::vector<std::vector<Cell>> squares, int x, int y){
    if (x <= 0 || x >= CellNumberPerCol - 1) {
        return false;
    }
    if (y <= 0 || y >= CellNumberPerRow - 1) {
        return false;
    }
    for(int i = 0; i < squares.size(); i++){
        for(int j = 0; j < squares[0].size(); j++){
            if (!squares[i][j].canJoin(cells[x + i][y + j])) {
                return false;
            }
        }
    }
    return true;
}
    
void MainScene::joinSquare(std::vector<std::vector<Cell>> squares, int x, int y) {
    if (x <= 0 || x >= CellNumberPerCol - 1) {
        return;
    }
    if (y <= 0 || y >= CellNumberPerRow - 1) {
        return;
    }
    for (int i = 0; i < squares.size(); i++) {
        for (int j = 0; j < squares[0].size(); j++) {
            cells[x + i][y + j] = squares[i][j];
        }
    }
}
void MainScene::cleanSquare(std::vector<std::vector<Cell>> squares, int x, int y){
    for (int i = 0; i < squares.size(); i++) {
        for (int j = 0; j < squares[0].size(); j++) {
            cells[x + i][y + j] = Cell{5};
        }
    }
}

void MainScene::print() {
    for (int i = 0; i < CellNumberPerCol; i++) {
        for (int j = 0; j < CellNumberPerRow; j++) {
            std::cout << cells[i][j];
        }
        std::cout << std::endl;
    }
}

void Move::move(MainScene& ms, const std::vector<std::vector<Cell>>& squares, int x, int y, Direction di) {
    int m = x, n = y;
    switch(di) {
    case Down:
        m = x + 1;
        break;
    case Left:
        n = y - 1;
        break;
    case Right:
        n = y + 1;
        break;
    }
        
        
    ms.cleanSquare(squares,x, y);
    if(ms.canJoin(squares, x+m, y+n)){
        ms.joinSquare(squares, x+m, y + n);
    }
    else{
        ms.joinSquare(squares, x, y);
    }
}
