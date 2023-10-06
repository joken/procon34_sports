// #include "castle_tactics.cpp"

std::pair<int, int> nearWalls(std::pair<int, int> *position, fieldmap *map, wallplan *plan){
    std::vector<std::vector<bool> > searched = std::vector<std::vector<bool> >(map->width, std::vector<bool>(map->height, false));

    int width = map->width;
    int height = map->height;
    std::deque<std::pair<int, int> > head;
    head.push_back(*position);
    if(plan->walls[position->first][position->second]){
        return std::make_pair(position->first, position->second);
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
        if(map->area[x][y] != 1 ){
            searched[x][y] = true; 
            //4近傍で壁があるかをチェック(field外は除外)
            for(int k = 0; k < 4; k++){
                int nx = x + dx4[k];
                int ny = y + dy4[k];
                if(nx < 0 || nx >= width || ny < 0 || ny >= height){
                    continue;
                }
                if(plan->walls[nx][ny] == 2){
                    return std::make_pair(x, y);
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
    
    return std::make_pair(random(0, width-1), random(0, height-1));
}

std::vector<int> builderAction(std::pair<int, int> *position, std::pair<int, int> *target, fieldmap *map, wallplan *plan){
    int dx = target->first - position->first;
    int dy = target->second - position->second;

    if(dx == 0 && dy == 0){
        return std::vector<int>(1, 2*random(1, 4));
    }if(abs(dx) <= 1 && abs(dy) <= 1){
        //壁がおける場合
        int x = position->first;
        int y = position->second;
        for(int k = 0; k < 4; k++){
            int nx = x + dx4[k];
            int ny = y + dy4[k];
            if(nx < 0 || nx >= map->width || ny < 0 || ny >= map->height){
                continue;
            }
            if(plan->walls[nx][ny] == 2){
                int action = 2;//build
                int dir = 2*(k+1); //4近傍

                if(map->walls[nx][ny] == 2){ // 相手の壁の時
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

std::vector<int> agentAction(std::pair<int, int> *position, fieldmap *map, wallplan *plan){
    std::pair<int, int> target = nearWalls(position, map, plan);

    return builderAction(position, &target, map, plan);
}