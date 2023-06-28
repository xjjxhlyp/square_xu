//
//  main.cpp
//  square_xu
//
//  Created by 许京京 on 2023/5/20.
//

#include "game.hpp"
#include <thread>
#include <unistd.h>//包含sleep()的头文件

using namespace std;
int main() {
    
    ShapeType shapeType = LLshape;
    std::shared_ptr<Shape> shapes = creatShape(shapeType);
    MainScene ms;
    int row = 1, col = 4;
    ms.joinSquare(shapes->Cells(), row, col);
    ms.printScreen();
    Move mo;
    UserCommand uc;
    bool stop = false;
    while(!stop){
        int cmd =  uc.getCmd();
        if(cmd == 66){
            mo.move(ms, shapes->Cells(), row, col,Move::Down);
            if(mo.isMove()){
                row++;
            }
        }
        else if(cmd == 68){
            mo.move(ms, shapes->Cells(), row, col,Move::Left);
            if(mo.isMove()){
                col--;
            }
        }
        else if(cmd == 67){
            mo.move(ms, shapes->Cells(), row, col,Move::Right);
            if(mo.isMove()){
                col++;
            }
        }
        ms.printScreen();
        mo.move(ms, shapes->Cells(), row, col,Move::Down);
        if(mo.isMove()){
            row++;
            ms.printScreen();
        }
        else{
            stop = true;
            ms.print();
        }
    }
    
    return 0;
}

