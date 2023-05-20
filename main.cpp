//
//  main.cpp
//  square_xu
//
//  Created by 许京京 on 2023/5/20.
//

#include <iostream>
#include <string>
#include <stdio.h>
#include <unistd.h>//包含sleep()的头文件

using namespace std;

//显示页面
void screen(int score, int level){
    for(int i = 0; i < 22; i++){
        cout << "_";
    }
    cout << endl;
    int k = 0;
    while(k < 15){
        cout << "|";
        for(int i = 0; i < 20; i++){
            cout << " ";
        }
        cout << "|";
        if(k == 5){
            cout << " " << "scores:" << score;
        }
        if(k == 9){
            cout << " " << "level:" << level;
        }
        cout << endl;
        k++;
    }
    for(int i = 0; i < 21; i++){
        cout << "-";
    }
    // 为啥：for test
    cout << endl;
    
    // 为啥：立即打印到终端；sleep会使进程休眠
    cout << flush;
}

void shape(int shape){
    
}

void printScene() {
    //保证每次屏幕显示在同一位置
    int i = 0, j = 1;
    while(true){
        screen(i++, j++);
        sleep(1);
        printf("\033[17A");
    }
}


int main() {
    // insert code here...

    printScene();
    //screen(0, 1);
    return 0;
}
