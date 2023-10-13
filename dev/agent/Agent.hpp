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
        int LastDir;

        void nearWalls(Field *map, wallplan *plan);
        std::vector<int> builderAction(Field *map, wallplan *plan);
    
    public:
        Agent(int id, std::pair<int, int> position){
            this->id = id;
            this->position = position;
            this->target = std::make_pair(0,0);
        }
        std::vector<int> action(Field *map, wallplan *plan);
        void erase(std::vector<int> &v, int key);
};

void Agent::nearWalls(Field *map, wallplan *plan){
    int width = map->getWidth();
    int height = map->getHeight();
    std::vector<std::vector<int> > area = map->getArea();

    std::vector<std::vector<bool> > searched = std::vector<std::vector<bool> >(width, std::vector<bool>(height, false));

    std::deque<std::pair<int, int> > head;
    head.push_back(this->position);
    if(plan->walls[this->position.first][this->position.second]){
        this->target = std::make_pair(this->position.first, this->position.second);
        return;
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
                    this->target = std::make_pair(x, y);
                    return;
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
    
    this->target = std::make_pair(random(0, width-1), random(0, height-1));
    return;
}

std::vector<int> Agent::builderAction(Field *map, wallplan *plan){
    int width = map->getWidth();
    int height = map->getHeight();
    std::vector<std::vector<int> > walls = map->getWalls();
    std::vector<std::vector<int> > area  = map->getArea();
    
    int dx = this->target.first - this->position.first;
    int dy = this->target.second - this->position.second;

    if(dx == 0 && dy == 0){
        return std::vector<int>(1, 2*random(1, 4));
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
                return std::vector<int>(action, dir);
            }
        }
    }else{
        int action = 1; //移動
        auto dir = ((int)(atan2(dx, dy)*4/M_PI/*[0,2π)から[0, 8)へ*/ + 8/*負の角度を排除*/ + 3.5) % 8);
        // int dir = ((int)(-atan2(dx, dy)*4/M_PI +17) % 8) + 1;
        int x = this->position.first;
        int y = this->position.second;
        // 壁に沿うように移動する
        if(area[x + dx8[dir]][y + dy8[dir]] > 0){
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
                vector<int> d = {5,6,8};        // 下か左か右下
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
        return std::vector<int>(action, dir);
    }
    LastDir = 2*random(1, 4);
    return std::vector<int>(1,this->LastDir );//最後まで来たらrandom移動
}

std::vector<int> Agent::action(Field *map, wallplan *plan){
    nearWalls(map, plan);

    return builderAction(map, plan);
}
// 配列要素の削除
void erase(std::vector<int> &v, int key){
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
