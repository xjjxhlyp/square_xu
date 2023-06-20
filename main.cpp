//
//  main.cpp
//  square_xu
//
//  Created by 许京京 on 2023/5/20.
//

#include "game.hpp"
#include <termios.h>
#include <thread>

using namespace std;
class UserCommand{
    std::mutex mtx;
    int cmd;
    
public:
    UserCommand(){
        cmd = 0;
    }
    
    void SetCmd(char ch){
        mtx.lock();
        cmd = ch;
        mtx.unlock();
    }
    
    int getCmd(){
        int res;
        mtx.lock();
        res = cmd;
        cmd = 0;
        mtx.unlock();
        return res;
    }
};

UserCommand us;

char getchar_no_output(){
    struct termios org_opts{};
    struct termios new_opts{};
    tcgetattr(1,&org_opts);
    memcpy(&new_opts, &org_opts, sizeof(org_opts));
    new_opts.c_lflag &=~(ECHO | ECHOE );
    tcsetattr(1, TCSANOW, &new_opts);
    char ch = getchar();
    tcsetattr(1,TCSANOW, &org_opts);
    return ch;
}

void reveiveInput(){
    system("stty -icanon");//直接接收一个字符，不用回车结束
    char ch;
    while (true) {
        ch = getchar_no_output();
        us.SetCmd(ch);
        
    }
}

int main() {
    ShapeType shapeType = Square;
    std::shared_ptr<Shape> shapes = creatShape(shapeType);
    MainScene ms;
    int row = 1, col = 4;
    ms.joinSquare(shapes->Cells(), row, col);
    ms.print();
    Move mo;
    std::thread th(reveiveInput);
    
    th.join();
    return 0;
}
