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
void MainScene::printScreen(EvenType et){
    print();
    std::cout << std::endl;
    switch(et){
        case UserInput:
            break;
        case TimedFall:
            usleep(500000);
            break;
    }
    std::printf("\033[23A");//光标向上移动23行，移动到屏幕开头
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

std::shared_ptr<Shape> createShape(ShapeType randomShape){
    switch(randomShape){
        case Square:
            return std::shared_ptr<SquareShape> (new SquareShape());
        case YiShape:
            return std::shared_ptr<LineShape> (new LineShape());
        case TtShape:
            return std::shared_ptr<TShape> (new TShape());
        case LLShape:
            return std::shared_ptr<LeftLShape> (new LeftLShape());
        case RLShape:
            return std::shared_ptr<RightLShape> (new RightLShape());
        case LZShape:
            return std::shared_ptr<LeftZShape> (new LeftZShape());
        case RZShape:
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

void UserCommand::receiveCmd(){
     system("stty -icanon");
     char ch;
     while(true){
        ch = getchar_no_output();
        mtx.lock();
        int cmd = ch;
        userComannds.push(cmd);
        mtx.unlock();
    }
 }

int UserCommand::getCmd(){
    if(userComannds.empty()) return 0;
    int res;
    mtx.lock();
    res = userComannds.front();
    userComannds.pop();
    mtx.unlock();
    return res;
}

void UserCommand::reponseCmd(MainScene&ms, Move mo, std::shared_ptr<Shape> shapes, int &row, int &col){
    while(!userComannds.empty()){
        int cmd = getCmd();
        UserCommandType ucType = static_cast<UserCommandType>(cmd);
        switch(ucType){
            case ToDown:
                if(mo.move(ms, shapes->Cells(), row, col,Move::Down))row++;
                ms.printScreen(MainScene::UserInput);
                break;
            case ToLeft:
                if(mo.move(ms, shapes->Cells(), row, col,Move::Left))col--;
                ms.printScreen(MainScene::UserInput);
                break;
            case ToRight:
                if(mo.move(ms, shapes->Cells(), row, col,Move::Right))col++;
                ms.printScreen(MainScene::UserInput);
                break;
        }
    }
}
void Game::run(){
    ShapeType rand = randomShape();
    std::shared_ptr<Shape> shapes = createShape(rand);
    MainScene ms;
    int row = 1, col = 4;
    ms.joinSquare(shapes->Cells(), row, col);
    ms.printScreen(MainScene::TimedFall);
    Move mo;
    UserCommand uc;
    bool stop = false;
    while(!stop){
        uc.reponseCmd(ms, mo, shapes, row, col);
        if(mo.move(ms, shapes->Cells(), row, col,Move::Down)){
            row++;
            ms.printScreen(MainScene::TimedFall);
        }
        else{
            stop = true;
            ms.print();
        }
       
    }
}
