//
//  main.cpp
//  square_xu
//
//  Created by 许京京 on 2023/5/20.
//

#include "game.hpp"
#include <gtest/gtest.h>


int main() {
    // insert code here...

    //printScene();
    //screen(0, 1);
    //show(3,4,0,1, "口");
    
    MainScene ms;
    ms.print();
    
    LShape lshape;
    ms.joinSquare(lshape.cells, 2, 1);
    ms.print();
    Move mo;
    
    mo.move(ms, lshape.cells, 2, 1, Move::Down);
    //mo.move(ms, {{Cell{6}, Cell{6}}, {Cell{6}, Cell{6}}}, 3, 5, 1, 0);
    ms.print();
    return 0;
}
