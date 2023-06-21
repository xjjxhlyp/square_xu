//
//  main.cpp
//  square_xu
//
//  Created by 许京京 on 2023/5/20.
//

#include "game.hpp"
#include <thread>


using namespace std;
int main() {
    /*ShapeType shapeType = Square;
    std::shared_ptr<Shape> shapes = creatShape(shapeType);
    MainScene ms;
    int row = 1, col = 4;
    ms.joinSquare(shapes->Cells(), row, col);
    ms.print();
    Move mo;
    //std::printf("\033[23A");*/
    
    UserCommand uc;
    while(true){
        int cmd =  uc.getCmd();
        std::cout << cmd << std::endl;
    }
    
    
    
    
    
    
    return 0;
}

