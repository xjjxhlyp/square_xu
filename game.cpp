//
//  game.cpp
//  square_xu
//
//  Created by 许京京 on 2023/5/27.
//

#include "game.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <stdio.h>
#include <unistd.h>//包含sleep()的头文件
#include <vector>


std::ostream& operator<<(std::ostream& out, Cell& cell) {
    if (cell.type == Cell::LeftBoundary) {
        std::cout << " |";
    } else if (cell.type == Cell::RightBoundary) {
        std::cout << "| ";
    } else if (cell.type == Cell::TopBoundary) {
        std::cout << "__";
    } else if (cell.type == Cell::BottomBoundary) {
        std::cout << "--";
    } else if (cell.type == Cell::Space) {
        std::cout << "  " ;
    } else if (cell.type == Cell::Square) {
        std::cout << "🟥";
    }
    return out;
}


MainScene::MainScene() {
    cells.resize(CellNumberPerCol);
        
    cells[0] = std::vector<Cell>(CellNumberPerRow, Cell{Cell::TopBoundary});
    for (int i = 1; i < CellNumberPerCol - 1; i++) {
        cells[i].push_back(Cell{Cell::LeftBoundary});
        for (int j = 1; j < CellNumberPerRow - 1; j++) {
            cells[i].push_back(Cell{Cell::Space});
        }
        cells[i].push_back(Cell{Cell::RightBoundary});
    }
    cells.back() = std::vector<Cell>(CellNumberPerRow, Cell{Cell::BottomBoundary});
}

int shapeWidth(std::vector<std::vector<Cell>> squares){
    int width = squares[0].size();
    for(int i = 1; i < squares.size(); i++){
        int rowWidth =squares[i].size();
         width= std::max(width, rowWidth);
    }
    return width;
}


bool MainScene::canJoin(std::vector<std::vector<Cell>> squares, int x, int y){
    if (x <= 0 || x  >= CellNumberPerCol - squares.size()) {
        return false;
    }
    //要保证整个图形不会超出边界
    int width = shapeWidth(squares);
    if (y <= 0 || y >= CellNumberPerRow - width) {
        return false;
    }
    for(int i = 0; i < squares.size(); i++){
        for(int j = 0; j < squares[i].size(); j++){ //squares每行元素个数不一定一样
            if (!squares[i][j].canJoin(cells[x + i][y + j])) {
                return false;
            }
        }
    }
    return true;
}
    
void MainScene::joinSquare(std::vector<std::vector<Cell>> squares, int x, int y) {
    if(!canJoin((squares), x, y)) return;
    for (int i = 0; i < squares.size(); i++) {
        for (int j = 0; j < squares[i].size(); j++) {//squares每行元素个数不一定一样
            if(squares[i][j] == Cell{Cell::Space}) continue;
            cells[x + i][y + j] = squares[i][j];
        }
    }
}
void MainScene::cleanSquare(std::vector<std::vector<Cell>> squares, int x, int y){
    for (int i = 0; i < squares.size(); i++) {
        for (int j = 0; j < squares[i].size(); j++) {//squares每行元素个数不一定一样
            cells[x + i][y + j] = Cell{Cell::Space};
        }
    }
}

void MainScene::print() {
    for (int i = 0; i < CellNumberPerCol; i++) {
        for (int j = 0; j < CellNumberPerRow; j++) {
            std::cout << cells[i][j];
        }
        std::cout << std::endl;
    }
}
void MainScene::printScreen(){
    print();
    std::cout << std::endl;
    usleep(500000);
    std::printf("\033[23A");//\033表示光标向上移动；23表示上移23行
}
bool Move::move(MainScene& ms, const std::vector<std::vector<Cell>>& squares, int x, int y, Direction di) {
    int m = x, n = y;
    switch(di) {
    case Down:
        m = x + 1;
        break;
    case Left:
        n = y - 1;
        break;
    case Right:
        n = y + 1;
        break;
    }
    ms.cleanSquare(squares,x, y);
    if(ms.canJoin(squares, m, n)){
        ms.joinSquare(squares, m, n);
        return true;
    }
    else{
        ms.joinSquare(squares, x, y);
        return false;
    }
}

bool MainScene::canRemove(int row){
    for(int i = 1; i < cells[i].size() - 1; i++){
        if(cells[row][i] == Cell{Cell::Space}){
            return false;
        }
    }
    return true;
}

void MainScene::RemoveOneRow(int row){
    if(canRemove(row)){
        for(int i = 1; i < cells[i].size()-1; i++){
            cells[row][i] = Cell{Cell::Space};
        }
    }
}

std::shared_ptr<Shape> creatShape(ShapeType shapeType){
    switch(shapeType){
        case Square:
            return std::shared_ptr<SquareShape> (new SquareShape());
        case Lineshape:
            return std::shared_ptr<LineShape> (new LineShape());
        case Tshape:
            return std::shared_ptr<TShape> (new TShape());
        case LLshape:
            return std::shared_ptr<LeftLShape> (new LeftLShape());
        case RLshape:
            return std::shared_ptr<RightLShape> (new RightLShape());
        case LZshape:
            return std::shared_ptr<LeftZShape> (new LeftZShape());
        case RZshape:
            return std::shared_ptr<RightZShape> (new RightZShape());
        default:
            //要有默认值来处理其他情况，但是不能被用户感知到
            return std::shared_ptr<SquareShape> (new SquareShape());
    }
}

void Shape::rotate(){
    std::vector<std::vector<Cell>> res;
    for(int j = 0; j < cells[0].size(); j++){
        std::vector<Cell> temp;
        for(int i = 0; i < cells.size(); i++){
            temp.push_back(cells[i][j]);
        }
        res.push_back(temp);
    }
    int i = 0, j = res.size()-1;
    while(i < j){
        swap(res[i++],res[j--]);
    }
    cells = res;
}

char UserCommand::getchar_no_output(){
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

void UserCommand::receiveCommand(){
     system("stty -icanon");//直接接收一个字符，不用回车结束
     char ch;
     while(true){
        ch = getchar_no_output();
        mtx.lock();
        cmd = ch;
        mtx.unlock();
    }
 }
int UserCommand::getCmd(){
    int res;
    mtx.lock();
    res = cmd;
    cmd = 0;
    mtx.unlock();
    return res;
}

ShapeType Game::randomShape(){
    int r = 0;
    for(int i = 0; i < ShapeTypeTotal;i++ ){
       r = rand();
    }
    return static_cast<ShapeType> (r % ShapeTypeTotal);
}

void Game::run(){
    std::shared_ptr<Shape> shapes = creatShape(randomShape());
    MainScene ms;
    ms.printScreen();
}
