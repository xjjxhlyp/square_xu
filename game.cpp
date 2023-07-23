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
#include <condition_variable>
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
        std::cout << "  ";
    } else if (cell.type == Cell::Square) {
        std::cout << "🟥";
    }
    return out;
}


MainScreen::MainScreen() {
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

bool MainScreen::canJoin(const ActiveShape& as){
    if (!as.isInBoundaries(0,CellNumberPerCol,0, CellNumberPerRow)) return false;
    std::vector<Point> vp = as.activePoints();
    for(int i = 0; i < vp.size(); i++){
        if(cells[vp[i].row][vp[i].col] == Cell{Cell::Square}) return false;
    }
    return true;
}

void MainScreen::joinSquare(const ActiveShape& as) {
    if(!canJoin(as)) return;
    std::vector<Point> vp = as.activePoints();
    for (int i = 0; i < vp.size(); i++) {
        cells[vp[i].row][vp[i].col] = Cell{Cell::Square};
    }
}

void MainScreen::cleanSquare(const ActiveShape& as){
    std::vector<Point> vp = as.activePoints();
    for (int i = 0; i < vp.size(); i++) {
        cells[vp[i].row][vp[i].col] = Cell{Cell::Space};
    }
}

void MainScreen::print() {
    for (int i = 0; i < CellNumberPerCol; i++) {
        for (int j = 0; j < CellNumberPerRow; j++) {
            std::cout << cells[i][j];
        }
        std::cout << std::endl;
    }
}
void MainScreen::printScreen(){
    print();
    std::cout << std::endl;
    printf("\033[23A");//\033表示光标向上移动；23表示上移23行
}

bool MainScreen::canRemove(int row){
    for(int i = 1; i < cells[i].size() - 1; i++){
        if(cells[row][i] == Cell{Cell::Space}){
            return false;
        }
    }
    return true;
}

void MainScreen::RemoveOneRow(int row){
    if(canRemove(row)){
        for(int i = 1; i < cells[i].size()-1; i++){
            cells[row][i] = Cell{Cell::Space};
        }
    }
}

std::shared_ptr<Shape> createShape(ShapeType shapeType){
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

std::vector<Point> Shape::points(){
    std::vector<Point> res;
    for(int i = 0; i < cells.size(); i++){
        for(int j = 0; j < cells[i].size(); j++){
            Point temp;
            temp.row = i;
            temp.col = j;
            if(cells[i][j] == Cell{Cell::Square}) res.push_back(temp);
        }
    }
    return res;
}

bool ActiveShape::isInBoundaries(int top, int bottom, int left, int right) const{
    if(point.row <= top || point.row >= bottom - shape->height()){
        return false;
    }
    if(point.col <= left || point.col >= right - shape->width()){
        return false;
    }
    return true;
}

std::vector<Point> ActiveShape::activePoints() const{
    std::vector<Point> res = shape->points();
    for(int i = 0; i < res.size(); i++){
        res[i].row += point.row;
        res[i].col += point.col;
    }
    return res;
}

void ActiveShape::responseCommand(Command cmd, int rightBoundary, int bottomBoundary){
    lastPoint = point;
    lastShape = shape;
    switch(cmd) {
        case Down:
            point.row++;
            break;
        case Left:
            point.col--;
            break;
        case Right:
            point.col++;
            break;
        case Rotate:
            rotate(rightBoundary, bottomBoundary);
            break;
        case DownToBottom:
            downToBottom(bottomBoundary);
            break;
    }
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

Command UserCommand::transformInputToCommand(char ch){
    const int down = 66;
    const int left = 68;
    const int right = 67;
    const int downToBottom = 32;
    const int rotate = 65;
    switch(ch){
        case down:
            return Down;
        case left:
            return Left;
        case right:
            return Right;
        case downToBottom:
            return DownToBottom;
        case rotate:
            return Rotate;
    }
    return Unknown;
}

void UserCommand::receiveCommand(){
    system("stty -icanon");//直接接收一个字符，不用回车结束
    while(true){
        Command cmd = transformInputToCommand(getchar_no_output());
        if(cmd == Unknown) continue;
        std::unique_lock<std::mutex> ul(mtx);
        cmds.push(cmd);
        cv.notify_one();
    }
}

void UserCommand::downPeriodly(){
    while(true){
        usleep(getDownPeriod());
        std::unique_lock<std::mutex> ul(mtx);
        cmds.push(Down);
        cv.notify_one();
    }
}

Command UserCommand::getCmd(){
    Command res = Unknown;
    std::unique_lock<std::mutex> ul(mtx);
    while(cmds.empty()){
        cv.wait(ul);
    }
    res = cmds.front();
    cmds.pop();
    return res;
}

void UserCommand::generateCmds(){
    std::thread th(&UserCommand::receiveCommand, this);
    th.detach();
    std::thread thPeriod(&UserCommand::downPeriodly, this);
    thPeriod.detach();
}

ShapeType Game::randomShape(){
    return static_cast<ShapeType> (rand() % ShapeTypeTotal);
}

void Game::response(MainScreen& ms, ActiveShape& as, Command cmd){
     ms.cleanSquare(as);
     as.responseCommand(cmd, ms.width(), ms.height());
     if (!ms.canJoin(as)) {
         as.rollback();
     }
     ms.joinSquare(as);
 }

void Game::run(){
    MainScreen ms;
    UserCommand uc(500000);
    uc.generateCmds();
    ms.printScreen();
}
