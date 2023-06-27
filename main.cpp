//
//  main.cpp
//  square_xu
//
//  Created by 许京京 on 2023/5/20.
//

#include "game.hpp"
#include <thread>
#include <unistd.h>//包含sleep()的头文件
void print(std::vector<std::vector<Cell>> res){
    for(int i = 0; i < res.size(); i++){
        for(int j = 0; j < res[0].size(); j++){
            std::cout << res[i][j] ;
        }
        std::cout << std::endl;
    }
}
using namespace std;
int main() {
    ShapeType shapeType = LZshape;
    std::shared_ptr<Shape> shapes = creatShape(shapeType);
    print(shapes->Cells());
    std::vector<std::vector<Cell>> res = shapes->rotate(shapes->Cells());
    print(res);
    return 0;
}

