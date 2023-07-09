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
    std::shared_ptr<Shape> shapes = createShape(Tshape);
    std::vector<Point> points = shapes->points();
    for(int i = 0; i < points.size(); i++){
        std::cout << points[i].row << points[i].col << std::endl;
    }
    return 0;
}

