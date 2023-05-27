//
//  square.cpp
//  square_xu
//
//  Created by 许京京 on 2023/5/25.
//

#include "game.hpp"

#include <gtest/gtest.h>

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
}
