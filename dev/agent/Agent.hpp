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

        void nearWalls(Field *map, wallplan *plan);
        std::vector<int> builderAction(Field *map, wallplan *plan);
    
    public:
        Agent(int id, std::pair<int, int> position){
            this->id = id;
            this->position = position;
        }
        std::vector<int> action(Field *map, wallplan *plan);
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
        int dir = ((int)(-atan2(dx, dy)*4/M_PI +17) % 8) + 1;
        return std::vector<int>(action, dir);
    }

    return std::vector<int>(1, 2*random(1, 4));//最後まで来たらrandom移動
}

std::vector<int> Agent::action(Field *map, wallplan *plan){
    nearWalls(map, plan);

    return builderAction(map, plan);
}

#endif // AGENT_HPP