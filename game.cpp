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
    }else if(cell.type == Cell::NormalString){
        std::cout << cell.str;
    }
    return out;
}

MainScreen::MainScreen() {
    cells.resize(RowNumbers);
    
    cells[0] = std::vector<Cell>(ColNumbers, Cell{Cell::TopBoundary});
    for (int i = 1; i < RowNumbers - 1; i++) {
        cells[i].push_back(Cell{Cell::LeftBoundary});
        for (int j = 1; j < ColNumbers - 1; j++) {
            cells[i].push_back(Cell{Cell::Space});
        }
        cells[i].push_back(Cell{Cell::RightBoundary});
        cells[i].insert(cells[i].end(), BackColNumbers, Cell{Cell::Space});
    }
    cells.back() = std::vector<Cell>(ColNumbers, Cell{Cell::BottomBoundary});
}
bool MainScreen::canJoinInner(const ActiveShape& as){
    std::vector<Point> vp = as.activePoints();
    for(int i = 0; i < vp.size(); i++){
        if(cells[vp[i].row][vp[i].col].isSquare()) return false;
    }
    return true;
}

bool MainScreen::canJoin(const ActiveShape& as){
    return (as.isInBoundaries(0,RowNumbers-1,0, ColNumbers-1) && canJoinInner(as));
}

void MainScreen::joinSquare(const ActiveShape& as) {
    if(!canJoin(as)) return;
    std::vector<Point> vp = as.activePoints();
    for (int i = 0; i < vp.size(); i++) {
        cells[vp[i].row][vp[i].col] = Cell{Cell::Square};
    }
}

void joinCell(std::vector<std::vector<Cell>>& currCells, const Point pt, const Cell& cell){
    currCells[pt.row][pt.col] = cell;
}

void MainScreen::joinActiveShape(std::vector<std::vector<Cell>>& cells, const std::vector<Point>& p, Point point){
    for (int i = 0; i < p.size(); i++) {
        cells[p[i].row + point.row][p[i].col + point.col] = Cell{Cell::Square};
    }
}

void MainScreen::printScreen(const ActiveShape& as, const ActiveShape& nextAs){
    auto currCells = cells;
    
    // join currAs
    std::vector<Point> vp = as.activePoints();
    joinActiveShape(currCells, vp, Point(0, 0));
    
    //join nextAs
    joinCell(currCells, Point(NextBegin, ColNumbers + 1), Cell("next"));
    std::vector<Point> nps = nextAs.activePoints();
    Point np = Point(NextBegin + 1, ColNumbers + 1);
    joinActiveShape(currCells, nps, Point(np.row - InitRow, np.col - InitCol));
    
    //join score
    joinCell(currCells, Point(ScoreBegin, ColNumbers + 1), Cell("score"));
    joinCell(currCells, Point(ScoreBegin + 1, ColNumbers + 1), Cell("  " + std::to_string(score)));
    
    //join level
    joinCell(currCells, Point(LevelBegin, ColNumbers + 1), Cell("level"));
    joinCell(currCells, Point(LevelBegin + 1, ColNumbers + 1), Cell("  "+std::to_string(level)));

    // print currCelles
    for (int i = 0; i < RowNumbers; i++) {
        for (int j = 0; j < cells[i].size(); j++) {
            std::cout << currCells[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    printf("\033[23A");//\033表示光标向上移动；23表示上移23行
}

bool MainScreen::rowCanRemove(int row){
    for(int i = 1; i < ColNumbers - 1; i++){
        if(cells[row][i].isSpace()){
            return false;
        }
    }
    return true;
}



void MainScreen::aboveCellsFall(int row){
    for(int i = row; i > 1; i--){
        for(int j = 1; j < ColNumbers - 1; j++){
            cells[i][j] = cells[i - 1][j];
        }
    }
}

void MainScreen::remove(){
    int row = RowNumbers - 2;// 从最后一行方块开始消除
    while(row > 1){
        if(!rowCanRemove(row)) {
            row--;
            continue;
        }
        aboveCellsFall(row);
        usleep(200000);
        score++;
    }
}

Shape createShape(ShapeType shapeType){
    switch(shapeType){
        case Square:
            return SquareShape();
        case Lineshape:
            return LineShape();
        case Tshape:
            return TShape();
        case LLshape:
            return LeftLShape();
        case RLshape:
            return RightLShape();
        case LZshape:
            return LeftZShape();
        case RZshape:
            return RightZShape();
        default:
            //要有默认值来处理其他情况，但是不能被用户感知到
            return  SquareShape();
    }
}

void Shape::rotate(){
    std::vector<std::vector<Cell>> res;
    //把列转化为行
    for(int j = 0; j < cells[0].size(); j++){
        std::vector<Cell> temp;
        for(int i = 0; i < cells.size(); i++){
            temp.push_back(cells[i][j]);
        }
        res.push_back(temp);
    }
    
    for(int i = 0; i < res.size(); i++){
        reverse(res[i].begin(), res[i].end());
    }
    cells = res;
}

std::vector<Point> Shape::points() const{
    std::vector<Point> res;
    for(int i = 0; i < cells.size(); i++){
        for(int j = 0; j < cells[i].size(); j++){
            if(cells[i][j].isSquare()) {
                res.push_back(Point(i, j));
            }
        }
    }
    return res;
}

bool ActiveShape::isInBoundaries(int topIdx, int bottomIdx, int leftIdx, int rightIdx) const{
    if(point.row <= topIdx || point.row + shape.height() > bottomIdx){ // idx + size是迭代器中end的概念，end 是无效的，可以等于bottom。
        return false;
    }
    if(point.col <= leftIdx || point.col + shape.width() > rightIdx){
        return false;
    }
    return true;
}

std::vector<Point> ActiveShape::activePoints() const{
    std::vector<Point> res = shape.points();
    for(int i = 0; i < res.size(); i++){
        res[i].row += point.row;
        res[i].col += point.col;
    }
    return res;
}

void ActiveShape::rollback(){
    point = lastPoint;
    shape = lastShape;
}
void ActiveShape::responseCommand(Command cmd){
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
            shape.rotate();
            break;
        default:
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

void Game::downToBottom(MainScreen &ms,ActiveShape& as){
    while(ms.canJoin(as)){
        as.responseCommand(Down);
    }
    as.rollback();
}

bool Game::response(MainScreen &ms,ActiveShape& as, Command cmd){
    as.responseCommand(cmd);
    //先处理悬空下落命令
    if(cmd == DownToBottom){
        downToBottom(ms, as);
        return true;
    }
    //处理其他命令
     bool res = false;
     if(!ms.canJoin(as)){
         as.rollback();
         if(cmd == Down) {
             res = true;
         }
     }
    return res;
 }

void Game::run(){
    MainScreen ms;
    UserCommand uc(800000);
    uc.generateCmds();
}
