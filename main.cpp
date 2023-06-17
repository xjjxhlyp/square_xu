//
//  main.cpp
//  square_xu
//
//  Created by 许京京 on 2023/5/20.
//

#include "game.hpp"
std::vector<std::vector<Cell>> generateSquares(int a){
    std::vector<std::vector<Cell>> squares;
    SquareShape sShape;
    LineShape lShape;
    TShape tShape;
    LeftLShape llShape;
    RightLShape rlShape;
    LeftZShape lzShape;
    RightZShape rzShape;
    switch(a){
        case 0:
            squares = sShape.cells;
            break;
        case 1:
            squares = lShape.cells;
            break;
        case 2:
            squares = tShape.cells;
            break;
        case 3:
            squares = llShape.cells;
            break;
        case 4:
            squares = rlShape.cells;
            break;
        case 5:
            squares = lzShape.cells;
            break;
        case 6:
            squares = rzShape.cells;
            break;
        default:
            std::cout << "unexpected shape" << a;
    }
    return squares;
}


int main() {
    int a = 4;
    std::vector<std::vector<Cell>> squares = generateSquares(a);
    MainScene ms;
    
    ms.joinSquare(squares, 21, 1);
    ms.print();
    return 0;
}
