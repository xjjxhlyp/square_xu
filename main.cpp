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
    ActiveShape currAs(ms.initShapePoint(),createShape(game.randomShape()));
       while(true){
           ActiveShape nextAs(ms.initShapePoint(),createShape(game.randomShape()));
           ms.printScreen(currAs, nextAs);
           bool stop = false;
           while(!stop){
               Command cmd = uc.getCmd();
               stop = game.response(ms, currAs, cmd);
               ms.printScreen(currAs, nextAs);
           }
           ms.joinSquare(currAs);
           currAs = nextAs;
       }
    return 0;
}


