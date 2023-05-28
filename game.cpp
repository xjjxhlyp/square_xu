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
#include <algorithm>

std::ostream& operator<<(std::ostream& out, Cell& cell) {
    if (cell.type == Cell::LeftBoundary) {
        std::cout << " |";
    } else if (cell.type == Cell::RightBoundary) {
        std::cout << "| ";
    } else if (cell.type == Cell::TopBoundary) {
        std::cout << "__";
    } else if (cell.type == Cell::BottomBoundary) {
        std::cout << "--";
    } else if (cell.type == Cell::Space) {
        std::cout << "  " ;
    } else if (cell.type == Cell::Square) {
        std::cout << "🟥";
    }
    return out;
}


MainScene::MainScene() {
    cells.resize(CellNumberPerCol);
        
    cells[0] = std::vector<Cell>(CellNumberPerRow, Cell{Cell::TopBoundary});
    for (int i = 1; i < CellNumberPerCol - 1; i++) {
        cells[i].push_back(Cell{Cell::LeftBoundary});
        for (int j = 1; j < CellNumberPerRow - 1; j++) {
            cells[i].push_back(Cell{Cell::Space});
        }
        cells[i].push_back(Cell{Cell::RightBoundary});
    }
    cells.back() = std::vector<Cell>(CellNumberPerRow, Cell{Cell::BottomBoundary});
}
    
bool MainScene::canJoin(std::vector<std::vector<Cell>> squares, int x, int y){
    if (x <= 0 || x  >= CellNumberPerCol - squares.size()) {
        return false;
    }
    //要保证整个图形不会超出边界
    int cols = squares[0].size();
    for(int i = 1; i < squares.size(); i++){
        int col =squares[i].size();
        cols = std::max(cols, col);
    }
    
    if (y <= 0 || y >= CellNumberPerRow - cols) {
        return false;
    }
    for(int i = 0; i < squares.size(); i++){
        for(int j = 0; j < squares[i].size(); j++){ //squares每行元素个数不一定一样
            if (!squares[i][j].canJoin(cells[x + i][y + j])) {
                return false;
            }
        }
    }
    return true;
}
    
void MainScene::joinSquare(std::vector<std::vector<Cell>> squares, int x, int y) {
    if (x <= 0 || x >= CellNumberPerCol - squares.size()) {
        return;
    }
    if (y <= 0 || y >= CellNumberPerRow - 1) {
        return;
    }
    for (int i = 0; i < squares.size(); i++) {
        for (int j = 0; j < squares[i].size(); j++) {//squares每行元素个数不一定一样
            cells[x + i][y + j] = squares[i][j];
        }
    }
}
void MainScene::cleanSquare(std::vector<std::vector<Cell>> squares, int x, int y){
    for (int i = 0; i < squares.size(); i++) {
        for (int j = 0; j < squares[i].size(); j++) {//squares每行元素个数不一定一样
            cells[x + i][y + j] = Cell{Cell::Space};
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
    if(ms.canJoin(squares, m, n)){
        ms.joinSquare(squares, m, n);
    }
    else{
        ms.joinSquare(squares, x, y);
    }
}
