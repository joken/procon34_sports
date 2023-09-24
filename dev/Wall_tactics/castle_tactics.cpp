//
// 城郭計画プログラム
//
#include <vector>
#include <iostream>
#include <deque>

void alart(){
    std::cerr << "hoge" << std::endl;
}
void alart(int any){
    std::cerr << "hoge " << any << std::endl;
}
void alart(int a1, int a2){
    std::cerr << "hoge " << a1 << "," << a2 << std::endl;
}

// ## 領域情報(area)
// * 平地: 0
// * 池: 1
// * 城: 2
// ## 物体(objects)
// * None : 0
// * 味方職人 : 正の整数( 1~ 6 <= [2,6])
// * 相手職人 : 負の整数(-1~-6 <= [-2,-6])
// * 味方城壁 :  9
// * 相手城壁 : -9
// ## 領地(territory)
// * None : 000(2)
// * 味方領地 : 0X1(2)
// * 相手領地 : 01X(2)
// * 城壁 : 1XX(2)
struct fieldmap{
    int width; //11~25
    int height;//11~25
    int castleRate;
    int territoryRate;
    int wallRate;
    std::vector<std::vector<int> > area;
    std::vector<std::vector<int> > objects;
    std::vector<std::vector<int> > territory;
};

// ## 城郭計画
// * None : 0
// * existed wall : 1
// * planed wall : 2
struct wallplan{
    int width; //11~25
    int height;//11~25
    std::vector<std::vector<int> > walls;
};


fieldmap getmap(){ //ダミーデータ
    fieldmap map = {
        11,
        11,
        100,
        30,
        10,
        std::vector<std::vector<int> > {
            {0,0,0,0,0,0,0,0,0,0,0},
            {0,1,0,0,1,0,1,0,0,1,0},
            {0,0,2,0,0,1,0,0,2,0,0},
            {0,0,0,1,0,0,0,1,0,0,0},
            {0,1,0,0,2,0,2,0,0,1,0},
            {0,0,1,0,0,1,0,0,1,0,0},
            {0,1,0,0,2,0,2,0,0,1,0},
            {0,0,0,1,0,0,0,1,0,0,0},
            {0,0,2,0,0,1,0,0,2,0,0},
            {0,1,0,0,1,0,1,0,0,1,0},
            {0,0,0,0,0,0,0,0,0,0,0},
        },
        std::vector<std::vector<int> > {
            {0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,1,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0},
            {0,-1,0,0,0,0,0,0,0,-2,0},
            {0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,2,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0},
        },
        std::vector<std::vector<int> > {
            {0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0},
        },
    };
    return map;
}

void showMap(fieldmap const *map){
    std::cout << "=========================" << std::endl;
    std::cout << "width : " << map->width << std::endl;
    std::cout << "height: " << map->height << std::endl;
    std::cout << "castle Rate   : " << map->castleRate << std::endl;
    std::cout << "territory Rate: " << map->territoryRate << std::endl;
    std::cout << "wall Rate     : " << map->wallRate << std::endl;
    std::cout << "--- area ---" << std::endl;
    for(std::vector<int> mp : map->area){
        for(int m : mp){
            char cell = ' ';
            switch (m){
            case 0: //平原の時
                cell = '.'; break;
            case 1: //池の時
                cell = '0'; break;
            case 2: //城の時
                cell = 'C'; break;
            default: //その他
                // cell = '.';
                cell = '@';
                break;
            }
            std::cout << cell;
        }
        std::cout << std::endl;
    }
    std::cout << "--- objects ---" << std::endl;
    for(std::vector<int> obj : map->objects){
        for(int c : obj){
            char cell = ' ';
            if(c == 0){
                cell = '.';
            }else if(c == 9){
                cell = 'f';
            }else if(c > 0){
                cell = 'F';
            }else if(c == -9){
                cell = 'e';
            }else if(c < 0){
                cell = 'E';
            }else{
                cell = 'X';
            }
            std::cout << cell;
        }
        std::cout << std::endl;
    }
    std::cout << "--- territory ---" << std::endl;
    for(std::vector<int> trr : map->territory){
        for(int c : trr){
            char cell = ' ';
            if(c == 0){
                cell = '.';
            }else if((c & 3)> 0){
                cell = 'X';
            }else if((c & 1) > 0){
                cell = '\\';
            }else if((c & 2) > 0){
                cell = '/';
            }else{
                cell = '+';
            }
            std::cout << cell;
        }
        std::cout << std::endl;
    }
    std::cout << "=========================" << std::endl;
}

wallplan planning(fieldmap const *map){ // ダミーデータ
    wallplan plan;
    plan.width = map->width;
    plan.height = map->height;
    std::vector<std::vector<int> > walls = {
        {0,0,0,0,0,0,0,0,0,0,0},
        {0,0,1,1,2,0,0,0,0,0,0},
        {0,0,2,0,0,2,2,2,0,0,0},
        {0,0,0,2,2,0,0,2,0,1,0},
        {0,2,2,0,0,0,0,2,0,1,0},
        {0,2,0,0,0,0,0,0,2,0,0},
        {0,2,0,0,0,0,0,2,0,0,0},
        {2,0,0,0,0,0,0,2,0,0,0},
        {0,2,2,0,0,0,0,2,0,0,0},
        {0,0,0,2,0,0,0,2,0,0,0},
        {0,0,0,0,2,2,2,2,0,0,0},
    };

    plan.walls = walls;
    return plan;
}

int calcTerritoryPoint(fieldmap const *map, wallplan const *plan){
    int width = map->width;
    int height = map->height;
    
    int point = 0;
    
    // # 領地，城壁を調べる
    // 0 : 領地以外
    // -1: 城壁
    // 1 : 領地
    std::vector<std::vector<int> > territory(width, std::vector<int>(height, 1));
    // 城壁の位置をコピー
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            if(plan->walls[i][j] > 0){
                territory[i][j] = -1;
            }
        }
    }


    for(std::vector<int> trr : territory){
        for(int c : trr){
            char cell = ' ';
            switch (c){
            case 0: 
                cell = '.'; break;
            case -1: 
                cell = 'X'; break;
            case 1: 
                cell = '0'; break;
            default: //その他
                // cell = '.';
                cell = '+';
                break;
            }
            std::cout << cell;
        }
        std::cout << std::endl;
    }

    // マップの外周に4近傍で隣接する領域を領地以外に上書きする(深さ優先探索)
    std::deque<std::pair<int, int> > head;
    // 外周を始点とする
    for(int i = 0; i < width; i++){
        head.push_back(std::make_pair(i, 0));
        head.push_back(std::make_pair(i, height-1));
    }
    for(int j = 1; j < height -1; j++){
        head.push_back(std::make_pair(0, j));
        head.push_back(std::make_pair(width-1, j));
    }
    // 深さ優先探索
    while(!head.empty()){
        std::pair<int, int> here = head.back();
        int x = here.first;
        int y = here.second;
        // alart(x, y);
        head.pop_back();
        if(territory[x][y] == 1){
            territory[x][y] = 0; 
            //4近傍を探索(field外は除外)
            if((0< x-1 && x-1 < width-1) && (0< y && y < height-1)){// 上
                head.push_back(std::make_pair(x-1, y));
            }
            if((0< x+1 && x+1 < width-1) && (0< y && y < height-1)){// 下
                head.push_back(std::make_pair(x+1, y));
            }
            if((0< x && x < width-1) && (0< y-1 && y-1 < height-1)){// 左
                head.push_back(std::make_pair(x, y-1));
            }
            if((0< x && x < width-1) && (0< y+1 && y+1 < height-1)){// 右
                head.push_back(std::make_pair(x, y+1));
            }
        }
    }

    for(std::vector<int> trr : territory){
        for(int c : trr){
            char cell = ' ';
            switch (c){
            case 0: 
                cell = '.'; break;
            case -1: 
                cell = 'X'; break;
            case 1: 
                cell = '0'; break;
            default: //その他
                // cell = '.';
                cell = '+';
                break;
            }
            std::cout << cell;
        }
        std::cout << std::endl;
    }

    //ポイント計算
    int area = 0;
    int walls = 0;
    int castles = 0;
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            switch(territory[i][j]){
            case -1:
                walls++; break;
            case 1:
                if(map->area[i][j] == 2){
                    castles++;
                }else{
                    area++;
                }
            }
        }
    }
    std::cout << "area   : " << area << std::endl;
    std::cout << "walls  : " << walls << std::endl;
    std::cout << "castles: " << castles << std::endl;
    point = (area * map->territoryRate) + (walls * map->wallRate) + (castles * map->castleRate);

    return point;
}



int main(){
    // 1.マップ情報の取得
    fieldmap map = getmap();
    showMap(&map);
    
    // 2.城郭の計画を立案
    wallplan plan = planning(&map);

    // 3.城郭の得点計算
    int point = calcTerritoryPoint(&map, &plan);
    std::cout << "point : " << point << std::endl;

    return 0;
}