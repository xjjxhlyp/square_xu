//
//  main.cpp
//  square_xu
//
//  Created by 许京京 on 2023/5/20.
//

#include "game.hpp"

int main() {
    ShapeType shapeType = Square;
    std::shared_ptr<Shape> shapes = creatShape(shapeType);
    MainScene ms;
    
    ms.joinSquare(shapes->Cells(), 21, 1);
    ms.print();
    return 0;
}
