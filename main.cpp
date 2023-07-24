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
    MainScreen ms;
    UserCommand uc(800000);
    uc.generateCmds();
    Game game;
       while(true){
           ActiveShape as(ms.initShapePoint(),createShape(game.randomShape()));
           ms.printScreen(as);
           bool stop = false;
           while(!stop){
               Command cmd = uc.getCmd();
               stop = game.response(ms, as, cmd);
               ms.printScreen(as);
           }
           ms.joinSquare(as);
           
       }
    return 0;
}


