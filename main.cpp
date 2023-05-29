//
//  main.cpp
//  square_xu
//
//  Created by 许京京 on 2023/5/20.
//

#include "game.hpp"


int main() {
    // insert code here...

    //printScene();
    //screen(0, 1);
    //show(3,4,0,1, "口");
    
    MainScene ms;
    testShape testshape;
    ms.joinSquare(testshape.cells, 21, 1);
    ms.print();
    ms.RemoveOneRow(21);
    
    
    ms.print();
    return 0;
}
