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
    MainScene ms;
    Game game;
       while(true){
           Point pt = ms.initShapePoint();
           std::shared_ptr<Shape> shapes = createShape(game.randomShape());
           ActiveShape as(pt, shapes);
           ms.joinSquare(as);
           ms.printScreen();
           Move mo;
           UserCommand uc;
           uc.beginReceiveCmd();
           bool stop = false;
           while(!stop ){
               Direction di = uc.getCmd();
               mo.move(ms, as, di);
               ms.printScreen();
           }
       }
    return 0;
}


