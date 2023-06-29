//
//  main.cpp
//  square_xu
//
//  Created by 许京京 on 2023/5/20.
//

#include "game.hpp"
#include <thread>
#include <unistd.h>//包含sleep()的头文件

void sum(int &row){
    row++;
    
}

using namespace std;
int main() {
    Game game;
    game.run();
    return 0;
}

