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
    */
    UserCommand uc;
    //  传入顺序：线程函数，实例化类指针，函数参数
    std::thread th(&UserCommand::receiveCommand, &uc);
    th.join();
    return 0;
}
