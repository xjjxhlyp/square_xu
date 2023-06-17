//
//  main.cpp
//  square_xu
//
//  Created by 许京京 on 2023/5/20.
//

#include "game.hpp"

//枚举是全局的，是和类平级的，不能放在函数里边
enum ShapeType{Square, Linshape, Tshape, LLshape, RLshape, LZshape, RZshape};

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

class UserCommand{
    std::mutex mtx;
public:
    int cmd;
    UserCommand(){
        cmd = 0;
    }
    
    void SetCmd(char ch){
        mtx.lock();
        cmd = ch;
        mtx.unlock();
    }
    
    int getCmd(){
        int res;
        mtx.lock();
        res = cmd;
        cmd = 0;
        mtx.unlock();
        return res;
    }
};

UserCommand us;

void reveiveInput(){
    system("stty -icanon");//直接接收一个字符，不用回车结束
    char ch;
    while (true) {
        ch = getchar();
        us.SetCmd(ch);
    }
}


int main() {
    int a = time(0) % 7;
    std::vector<std::vector<Cell>> squares = generateSquares(a);
    MainScene ms;
    
    ms.joinSquare(squares, 21, 1);
    ms.print();
    return 0;
}
