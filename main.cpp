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
    //为什么这里要输出一个换行？假如没有换行，光标会停在最后一行的结尾处，在上移后也会从第一行的结尾处开始打印，增加一个换行保证光标会上移到第一行的开始
    cout << endl;
    
    // flush表示立即打印到终端；
    //为什么需要立即打印到终端？cout的作用只是把数据放到打印缓冲区中，并不会立即打印到输出界面上；当调用sleep()后，进程会休眠，导致部分数据无法在输出界面显示
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
