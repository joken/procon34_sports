#ifndef AGENT_HPP
#define AGENT_HPP
#include <vector>
#include <deque>
#include <iostream>
#include "Field.hpp"

class Agent{
    private:
        int id;  // agentの番号(Unique)
        std::pair<int, int> position;
        std::pair<int, int> target;
        int LastDir; //前回に動いた方向[0,7](基本的に方向を180度回転させて使う)

        std::pair<int, int> nearWalls(Field *map, wallplan *plan);
        std::pair<int, int> nearEnemy(Field *map);
        int targetDistance(std::pair<int, int> tage);

        std::vector<int> builderAction(Field *map, wallplan *plan);
        std::vector<int> rndAction(Field *map, wallplan *plan);
        std::vector<int> lineAction(Field *map, wallplan *plan);
        std::vector<int> testerAction(Field *map, wallplan *plan);

        std::vector<int> breakerAction(Field *map);
    public:
        Agent(int id, std::pair<int, int> position){
            this->id = id;
            this->position = position;
            this->target = std::make_pair(0,0);
        }
        std::vector<int> action(Field *map, wallplan *plan){
            std::pair<int, int> tageF = nearWalls(map, plan);
            std::pair<int, int> tageE = nearEnemy(map);
            if(targetDistance(tageE) <= targetDistance(tageF) ){
                this->target = tageE;
                return breakerAction(map);
            }else{
                this->target = tageF;

                // return testerAction(map, plan);
                // return lineAction(map, plan);
                return rndAction(map, plan);
                // return builderAction(map, plan);
            }


        }
        void updatePositon(std::pair<int, int> pos){
            int dx = pos.first - this->position.first;
            int dy = pos.second - this->position.second;
            std::cerr << dx << " , " << dy << std::endl;

            if(dx == 0 && dy == 0){
                //動いてない
                // LastDir = 0;
                return;
            }else if(abs(dx) <= 1 && abs(dy) <= 1){
                // showPosition();
                // this->position.first = pos.first;
                // this->position.second = pos.second;
                this->position = pos;
                
                int dir = ((int)(atan2(dx, dy)*4/M_PI/*[0,2π)から[0, 8)へ*/ + 8/*負の角度を排除*/ + 3.5) % 8) + 1 /*0:停滞の分ずらす*/;
                this->LastDir = dir;
                return;
            }else{
                // showPosition();
                // this->position.first = pos.first;
                // this->position.second = pos.second;
                this->position = pos;
                return;
            }
        }
        void erase(std::vector<int> &v, int key);
        void showTarget(){
            std::cout << id << " target: " << this->target.first << " , " << this->target.second << std::endl;
        }
        void showPosition(){
            std::cout << id << " position: " << this->position.first << " , " << this->position.second << std::endl;

        }
};

std::pair<int, int> Agent::nearWalls(Field *map, wallplan *plan){
    int width = map->getWidth();
    int height = map->getHeight();
    std::vector<std::vector<int> > area = map->getArea();

    std::vector<std::vector<bool> > searched = std::vector<std::vector<bool> >(width, std::vector<bool>(height, false));

    std::deque<std::pair<int, int> > head;
    head.push_back(this->position);
    if(plan->walls[this->position.first][this->position.second] == 2){
        // this->target = std::make_pair(this->position.first, this->position.second);
        return std::make_pair(this->position.first, this->position.second);
    }
    
    // 幅優先探索
    while(!head.empty()){
        std::pair<int, int> here = head.front();
        int x = here.first;
        int y = here.second;
        // alart(x, y);
        head.pop_front();
        if(searched[x][y]){
            continue;
        }
        if(area[x][y] != 1 ){
            searched[x][y] = true; 
            //4近傍で壁があるかをチェック(field外は除外)
            for(int k = 0; k < 4; k++){
                int nx = x + dx4[k];
                int ny = y + dy4[k];
                if(nx < 0 || nx >= width || ny < 0 || ny >= height){
                    continue;
                }
                if(plan->walls[nx][ny] == 2){
                    // this->target = std::make_pair(x, y);
                    return std::make_pair(nx, ny);
                }
            }
            //8近傍で探索継続
            for(int k = 0; k < 8; k++){
                int nx = x + dx8[k];
                int ny = y + dy8[k];
                if(nx < 0 || nx >= width || ny < 0 || ny >= height){
                    continue;
                }
                head.push_back(std::make_pair(nx, ny));
            }
        }
    }
    
    // this->target = std::make_pair(random(0, width-1), random(0, height-1));
    return std::make_pair(random(0, width-1), random(0, height-1));
}

std::pair<int, int> Agent::nearEnemy(Field *map){
    int width = map->getWidth();
    int height = map->getHeight();
    std::vector<std::vector<int> > area = map->getArea();
    std::vector<std::vector<int> > territory = map->getTerritory();

    std::vector<std::vector<bool> > searched = std::vector<std::vector<bool> >(width, std::vector<bool>(height, false));

    std::deque<std::pair<int, int> > head;
    head.push_back(this->position);
    if(territory[this->position.first][this->position.second] == 2 && area[this->position.first][this->position.second] == 2){
        // this->target = std::make_pair(this->position.first, this->position.second);
        return std::make_pair(this->position.first, this->position.second);
    }
    
    // 幅優先探索
    while(!head.empty()){
        std::pair<int, int> here = head.front();
        int x = here.first;
        int y = here.second;
        // alart(x, y);
        head.pop_front();
        if(searched[x][y]){
            continue;
        }
        if(area[x][y] != 1 ){
            searched[x][y] = true; 
            //4近傍で壁があるかをチェック(field外は除外)
            for(int k = 0; k < 4; k++){
                int nx = x + dx4[k];
                int ny = y + dy4[k];
                if(nx < 0 || nx >= width || ny < 0 || ny >= height){
                    continue;
                }
                if(territory[nx][ny] == 2 && area[nx][ny] == 2){
                    // this->target = std::make_pair(x, y);
                    return std::make_pair(nx, ny);
                }
            }
            //8近傍で探索継続
            for(int k = 0; k < 8; k++){
                int nx = x + dx8[k];
                int ny = y + dy8[k];
                if(nx < 0 || nx >= width || ny < 0 || ny >= height){
                    continue;
                }
                head.push_back(std::make_pair(nx, ny));
            }
        }
    }
    
    // this->target = std::make_pair(random(0, width-1), random(0, height-1));
    return std::make_pair(1000, 1000); //絶対的に遠い場所(優先度を下げる目的)
}

int Agent::targetDistance(std::pair<int, int> tage){
    int dx = tage.first - this->position.first;
    int dy = tage.second - this->position.second;
    return abs(dx)+abs(dy);
}

std::vector<int> Agent::breakerAction(Field *map){
    return {0,0};
}


std::vector<int> Agent::testerAction(Field *map, wallplan *plan){
    int width = map->getWidth();
    int height = map->getHeight();
    std::vector<std::vector<int> > walls = map->getWalls();
    std::vector<std::vector<int> > area = map->getArea();
    int x = this->position.first;
    int y = this->position.second;
    
    std::cout << "(x, y) = (" << x << ", " << y << ")" << std::endl;
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            if(i == x && j == y){
                std::cout << "X";
                continue;
            }
            std::cout << walls[i][j];
        }
        std::cout << std::endl;
    }

    // 4近傍に壁を作る
    for(int k = 0; k < 4; k++){
        int nx = x + dx4[k];
        int ny = y + dy4[k];
        if(nx < 0 || nx >= width || ny < 0 || ny >= height){
            continue;
        }
        // std::cout << "(nx, ny) = (" << nx << ", " << ny << ") : " << walls[nx][ny] << std::endl;
        if(walls[nx][ny] != 1){ //既に壁が存在しない
            int action = 2;
            int dir = 2*(k+1);
            // if(dir == 0){
            //     this->LastDir = 0;
            // }else{
            //     this->LastDir = (dir-1 + 4)%8;
            // }
            return std::vector<int>{action, dir};
        }
    }
    // 壁を4つ作り終わってたらランダム移動
    int action = 1;
    int dir = random(1,8);
    return std::vector<int>{action, dir};


}

std::vector<int> Agent::lineAction(Field *map, wallplan *plan){
    int width = map->getWidth();
    int height = map->getHeight();
    std::vector<std::vector<int> > walls = map->getWalls();
    std::vector<std::vector<int> > area  = map->getArea();
    std::vector<std::vector<int> > territory  = map->getTerritory();
    
    map->showWalls();
    
    int x = this->position.first;
    int y = this->position.second;
    for(int k = 0; k < 8; k++){
        int nx = x + dx8[k];
        int ny = y + dy8[k];
        if(nx < 0 || nx >= width || ny < 0 || ny >= height){
            continue;
        }
        if(area[nx][ny] == 2 && walls[nx][ny] != 2){
            int action = 1;
            int dir = k+1;

            if(dir == 0){
                this->LastDir = 0;
            }else{
                this->LastDir = dir;
            }
            return std::vector<int>{action,dir};
        }
    }
    // std::cout <<"--- --- ---" << std::endl;
    //     for(std::vector<int> obj : walls){
    //     for(int c : obj){
    //         char cell = ' ';
    //         switch (c){
    //         case 0: //None
    //             cell = '.'; break;
    //         case 1: //自陣
    //             cell = 'f'; break;
    //         case 2: //相手陣
    //             cell = 'e'; break;
    //         default: //その他
    //             // cell = '.';
    //             cell = '@';
    //             break;
    //         }
    //         std::cout << cell;
    //     }
    //     std::cout << std::endl;
    // }
    for(int k = 0; k < 4; k++){
        int nx = x + dx4[k];
        int ny = y + dy4[k];
        if(nx < 0 || nx >= width || ny < 0 || ny >= height){
            continue;
        }
        if(walls[nx][ny] != 1 && territory[nx][ny] != 1){
            
            int action = 2;
            int dir = 2*(k+1);

            if(walls[nx][ny] == 2){
                if(random(1,7) < 10){//破壊を優先しすぎて築城を滞らせるのを防ぐ
                    continue;
                }
                int action = 3;
            }
            return std::vector<int>{action,dir};
        }
    }
    int rand = random(0,3);
    for(int k = rand; k < 4+rand; k++){
        int nx = x + dx8[(k*2)%8];
        int ny = y + dy8[(k*2)%8];
        if(nx < 0 || nx >= width || ny < 0 || ny >= height){
            continue;
        }
        if(walls[nx][ny] != 2){
            int action = 1;
            int dir = ((2*k)%8) + 1;

            return std::vector<int>{action,dir};
        }
    }
    int action = 1;
    int dir = 0;
    for(int i = 0; i < 500 && dir == 0; i++){
        dir = random(1,8);
        if(area[x+dx8[dir-1]][y+dy8[dir-1]] ==1){
            dir = 0;
        }else if(walls[x+dx8[dir-1]][y+dy8[dir-1]] == 2){
            dir = 0;
        }
    }

    if(dir == 0){
        this->LastDir = 0;
    }else{
        this->LastDir = (dir-1 + 4)%8;
    }
    return std::vector<int>{action,dir};
}
std::vector<int> Agent::rndAction(Field *map, wallplan *plan){
    int width = map->getWidth();
    int height = map->getHeight();
    std::vector<std::vector<int> > walls = map->getWalls();
    std::vector<std::vector<int> > area  = map->getArea();
    
    int x = this->position.first;
    int y = this->position.second;
    for(int k = 0; k < 4; k++){
        int nx = x + dx4[k];
        int ny = y + dy4[k];
        if(nx < 0 || nx >= width || ny < 0 || ny >= height){
            continue;
        }
        if(plan->walls[nx][ny] >= 2 && walls[nx][ny] != 1 ){
            int action = 2;//build
            int dir = 2*(k+1); //4近傍

            if(walls[nx][ny] == 2){ // 相手の壁の時
                action = 3;
            }
            return std::vector<int>{action, dir};
        }
        if(walls[nx][ny] == 2){
            int action = 3;
            int dir =2*(k+1);
            return std::vector<int>{action, dir};
        }
    }
    int action = 1;
    int dir = 0;
    for(int i = 0; i < 500 && dir == 0; i++){
        dir = random(1,8);
        int nx = x + dx8[dir-1];
        int ny = y + dy8[dir-1];
        if(nx < 0 || nx >= width || ny < 0 || ny >= height){
            continue;
        }
        if(area[nx][ny] ==1){
            dir = 0;
        }else  if(walls[nx][ny] == 2){
            dir = 0;
        }
    }

    if(dir == 0){
        this->LastDir = 0;
    }else{
        this->LastDir = (dir-1 + 4)%8;
    }
    return std::vector<int>{action,dir};
}

std::vector<int> Agent::builderAction(Field *map, wallplan *plan){
    int width = map->getWidth();
    int height = map->getHeight();
    std::vector<std::vector<int> > walls = map->getWalls();
    std::vector<std::vector<int> > area  = map->getArea();
    
    int dx = this->target.first - this->position.first;
    int dy = this->target.second - this->position.second;

    if(dx == 0 && dy == 0){
        return std::vector<int>{1, 2*random(1, 4)};
    }if(abs(dx) <= 1 && abs(dy) <= 1){
        //壁がおける場合
        int x = this->position.first;
        int y = this->position.second;
        for(int k = 0; k < 4; k++){
            int nx = x + dx4[k];
            int ny = y + dy4[k];
            if(nx < 0 || nx >= width || ny < 0 || ny >= height){
                continue;
            }
            if(plan->walls[nx][ny] == 2){
                int action = 2;//build
                int dir = 2*(k+1); //4近傍

                if(walls[nx][ny] == 2){ // 相手の壁の時
                    action = 3;
                }
                return std::vector<int>{action, dir};
            }
        }
    }else{
        int action = 1; //移動
        auto dir = ((int)(atan2(dx, dy)*4/M_PI/*[0,2π)から[0, 8)へ*/ + 8/*負の角度を排除*/ + 3.5) % 8);
        // int dir = ((int)(-atan2(dx, dy)*4/M_PI +17) % 8) + 1;
        int x = this->position.first;
        int y = this->position.second;
        // 壁に沿うように移動する
        if(area[x + dx8[dir]][y + dy8[dir]] != 1){
            int rdir = ((LastDir -1)+4 % 8) + 1;
            if(dir == 1){                       // 左上に壁がある
                vector<int> d = {2,3,8};        // 上か左か右上
                erase(d, rdir);
                dir = d[random(0,d.size()-1)];
            }else if(dir == 3){                 // 右上に壁がある
                vector<int> d = {1,2,4};        // 上か右か左上
                erase(d, rdir);
                dir = d[random(0,d.size()-1)];
            }else if(dir == 5){                 // 右下に壁がある
                vector<int> d = {7,6,8};        // 下か左か右下
                erase(d, rdir);
                dir = d[random(0,d.size()-1)];
            }else if(dir == 7){                 // 左下に壁がある
                vector<int> d = {5,6,8};        // 下か左か右下
                erase(d, rdir);
                dir = d[random(0,d.size()-1)];
            }else if(dir == 2 || dir == 6){     //上に壁があるとき
                vector<int> d = {4,8};           // 右か左
                erase(d, rdir);
                dir = d[random(0,d.size()-1)]; 
            }else if(dir == 4 || dir == 8){     // 左右に壁があるとき
                vector<int> d = {2,6};          // 上か下
                erase(d, rdir);
                dir = d[random(0,d.size()-1)];
            }
        }
        LastDir = dir;
        return std::vector<int>{action, dir};
    }
    LastDir = 2*random(1, 4);
    return std::vector<int>{1,this->LastDir};//最後まで来たらrandom移動
}

// 配列要素の削除
void Agent::erase(std::vector<int> &v, int key){
    for (auto it = v.begin(); it != v.end();)
    {
        if (*it == key) {
            it = v.erase(it);
        }
        else {
            ++it;
        }
    }
}

#endif // AGENT_HPP
