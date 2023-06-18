//
//  main.cpp
//  square_xu
//
//  Created by 许京京 on 2023/5/20.
//

#include "game.hpp"

int main() {
    ShapeType shapeType = Square;
    Shape *shape = creatShape(shapeType);
    MainScene ms;
    
    ms.joinSquare(shape->Cells(), 21, 1);
    ms.print();
    return 0;
}
