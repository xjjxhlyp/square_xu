//
//  square.cpp
//  square_xu
//
//  Created by 许京京 on 2023/5/25.
//

#include "game.hpp"

#include <gtest/gtest.h>
class testShape{
    std::vector<Cell> cell3 {18, Cell{Cell::Square}};
    std::vector<Cell> cell4 {16, Cell{Cell::Square}};
public:
    std::vector<std::vector<Cell>> cells;
    testShape() {
        cells.push_back(cell3);
        cells.push_back(cell4);
    }
};


TEST(canJoin, CanJoin) {
    Cell cell1(1);
    Cell cell2(2);
    EXPECT_FALSE(cell1.canJoin(cell2));
}

TEST(move_move, down_succ) {
    MainScene ms;
    SquareShape ss;
    ms.joinSquare(ss.cells, 3, 2);
    
    Move move;
    move.move(ms, ss.cells, 3, 2, Move::Down);
    
    EXPECT_EQ(ms.cells[3][2], Cell{1});
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    /*UserCommand uc;
    while(true){
        int cmd =  uc.getCmd();
        if(cmd != 0){
            std::cout << cmd << std::endl;
            sleep(1);
        }
    }*/
    
    /*ShapeType shapeType = Square;
    std::shared_ptr<Shape> shapes = creatShape(shapeType);
    MainScene ms;
    int row = 1, col = 4;
    ms.joinSquare(shapes->Cells(), row, col);
    ms.print();
    Move mo;
    //std::printf("\033[23A");*/
}
