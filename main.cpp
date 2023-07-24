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
    /*MainScreen ms;
    UserCommand uc(800000);
    uc.generateCmds();
    Game game;
       while(true){
           ActiveShape as(ms.initShapePoint(), createShape(Tshape));
           ms.printScreen(as);
           bool stop = false;
           while(!stop){
               Command cmd = uc.getCmd();
               stop = game.response(ms, as, cmd);
               ms.printScreen(as);
           }
           ms.joinSquare(as);
       }*/
    std::shared_ptr<Shape> shape = createShape(Tshape);
    std::vector<Point> sp = shape->points();
    for(int i = 0; i < sp.size(); i++){
        std::cout << sp[i].row << ' ' << sp[i].col << std::endl;
    }
    
    std::cout << std::endl;
    
    MainScreen ms;
    ActiveShape as(ms.initShapePoint(), createShape(Tshape));
    std::vector<Point> ap = as.activePoints();
    for(int i = 0; i < ap.size(); i++){
        std::cout << ap[i].row << ' ' << ap[i].col << std::endl;
    }
    
    
    return 0;
}


