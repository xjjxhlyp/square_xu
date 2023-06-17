//
//  main.cpp
//  square_xu
//
//  Created by 许京京 on 2023/5/20.
//

#include "game.hpp"
std::vector<std::vector<Cell>> generateSquares(int a){
    enum ShapeType{Square, Linshape, Tshape, LLshape, RLshape, LZshape, RZshape};
    std::vector<std::vector<Cell>> squares;
    SquareShape sShape;
    LineShape lShape;
    TShape tShape;
    LeftLShape llShape;
    RightLShape rlShape;
    LeftZShape lzShape;
    RightZShape rzShape;
    switch(a){
        case Square:
            squares = sShape.cells;
            break;
        case Linshape:
            squares = lShape.cells;
            break;
        case Tshape:
            squares = tShape.cells;
            break;
        case LLshape:
            squares = llShape.cells;
            break;
        case RLshape:
            squares = rlShape.cells;
            break;
        case LZshape:
            squares = lzShape.cells;
            break;
        case RZshape:
            squares = rzShape.cells;
            break;
        default:
            squares = sShape.cells;//要有默认值来处理其他情况，但是不能被用户感知到
    }
    return squares;
}


int main() {
    int a = 7;
    std::vector<std::vector<Cell>> squares = generateSquares(a);
    MainScene ms;
    
    ms.joinSquare(squares, 21, 1);
    ms.print();
    return 0;
}
