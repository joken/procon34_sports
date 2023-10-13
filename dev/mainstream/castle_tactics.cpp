//
// 城郭計画プログラム
//
#include <vector>
#include <iostream>
#include <deque>
#include <random>

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
// -- * 城壁 : 1XX(2)
struct fieldmap{
    int width; //11~25
    int height;//11~25
    int masonNum;//陣ごとの職人数
    int castleRate;
    int territoryRate;
    int wallRate;
    std::vector<std::vector<int> > area;
    std::vector<std::vector<int> > masons;
    std::vector<std::vector<int> > walls;
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

//　近傍のマクロ
// 上から時計回り
int dx4[4] = {0, 1, 0, -1};
int dy4[4] = {1, 0, -1, 0};
// 左上から時計回り
int dx8[8] = {-1,-1,-1,0,1,1,1,0};
int dy8[8] = {-1,0,1,1,1,0,-1,-1};



std::random_device rd;
std::mt19937 gen(rd());
 
int random(int low, int high)
{
    std::uniform_int_distribution<> dist(low, high);
    return dist(gen);
}

// fieldmap getmap(){ //ダミーデータ
//     fieldmap map = {
//         11,
//         11,
//         2,
//         100,
//         30,
//         10,
//         std::vector<std::vector<int> > {
//             {0,0,0,0,0,0,0,0,0,0,0},
//             {0,1,0,0,1,0,1,0,0,1,0},
//             {0,0,2,0,0,1,0,0,2,0,0},
//             {0,0,0,1,0,0,0,1,0,0,0},
//             {0,1,0,0,2,0,2,0,0,1,0},
//             {0,0,1,0,0,1,0,0,1,0,0},
//             {0,1,0,0,2,0,2,0,0,1,0},
//             {0,0,0,1,0,0,0,1,0,0,0},
//             {0,0,2,0,0,1,0,0,2,0,0},
//             {0,1,0,0,1,0,1,0,0,1,0},
//             {0,0,0,0,0,0,0,0,0,0,0},
//         },
//         std::vector<std::vector<int> > {
//             {0,0,0,0,0,0,0,0,0,0,0},
//             {0,0,0,0,0,1,0,0,0,0,0},
//             {0,0,0,0,0,0,0,0,0,0,0},
//             {0,0,0,0,0,0,0,0,0,0,0},
//             {0,0,0,0,0,0,0,0,0,0,0},
//             {0,-1,0,0,0,0,0,0,0,-2,0},
//             {0,0,0,0,0,0,0,0,0,0,0},
//             {0,0,0,0,0,0,0,0,0,0,0},
//             {0,0,0,0,0,0,0,0,0,0,0},
//             {0,0,0,0,0,2,0,0,0,0,0},
//             {0,0,0,0,0,0,0,0,0,0,0},
//         },
//         std::vector<std::vector<int> > {
//             {0,0,0,0,0,0,0,0,0,0,0},
//             {0,0,0,0,0,0,0,0,0,0,0},
//             {0,0,0,0,0,0,0,0,0,0,0},
//             {0,0,0,0,0,0,0,0,0,0,0},
//             {0,0,0,0,0,0,0,0,0,0,0},
//             {0,0,0,0,0,0,0,0,0,0,0},
//             {0,0,0,0,0,0,0,0,0,0,0},
//             {0,0,0,0,0,0,0,0,0,0,0},
//             {0,0,0,0,0,0,0,0,0,0,0},
//             {0,0,0,0,0,0,0,0,0,0,0},
//             {0,0,0,0,0,0,0,0,0,0,0},
//         },
//     };
//     return map;
// }

std::vector<std::pair<int, int> > getMasons(fieldmap const *map){
    int width = map->width;
    int height = map->height;

    std::vector<std::pair<int, int>> masons = std::vector<std::pair<int, int>>(map->masonNum);

    for(int i = 0; i < width; i++){
        for(int j =0; j < height; j++){
            if(map->masons[i][j] > 0){
                masons[map->masons[i][j] -1] = std::make_pair(i, j);
            }
        }
    }

    return masons;
}

void showMap(fieldmap const *map){
    std::cout << "=========================" << std::endl;
    std::cout << "width : " << map->width << std::endl;
    std::cout << "height: " << map->height << std::endl;
    std::cout << "masonNum: " << map->masonNum << std::endl;
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
    std::cout << "--- masons ---" << std::endl;
    for(std::vector<int> obj : map->masons){
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
    std::cout << "--- walls ---" << std::endl;
    for(std::vector<int> obj : map->walls){
        for(int c : obj){
            char cell = ' ';
            switch (c){
            case 0: //None
                cell = '.'; break;
            case 1: //自陣
                cell = 'f'; break;
            case 2: //相手陣
                cell = 'e'; break;
            default: //その他
                // cell = '.';
                cell = '@';
                break;
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

void showPlan(wallplan const *plan){
    std::cout << "=========================" << std::endl;
    std::cout << "width : " << plan->width << std::endl;
    std::cout << "height: " << plan->height << std::endl;
    std::cout << "--- wallplan map ---" << std::endl;
    for(std::vector<int> obj : plan->walls){
        for(int c : obj){
            char cell = ' ';
            switch (c){
            case 0: //None
                cell = '.'; break;
            case 1: //exist wall
                cell = 'X'; break;
            case 2: //planed wall
                cell = '+'; break;
            case 3: //planed wall
                cell = '*'; break;
            default: //その他
                // cell = '.';
                cell = '@';
                break;
            }
            std::cout << cell;
        }
        std::cout << std::endl;
    }
    std::cout << "=========================" << std::endl;
}

// wallplan planning(fieldmap const *map){ // ダミーデータ
//     wallplan plan;
//     plan.width = map->width;
//     plan.height = map->height;
//     std::vector<std::vector<int> > walls = {
//         {0,0,0,0,0,0,0,0,0,0,0},
//         {0,0,1,1,2,0,0,0,0,0,0},
//         {0,0,2,0,0,2,2,2,0,0,0},
//         {0,0,0,2,2,0,0,2,0,1,0},
//         {0,2,2,0,0,0,0,2,0,1,0},
//         {0,2,0,0,0,0,0,0,2,0,0},
//         {0,2,0,0,0,0,0,2,0,0,0},
//         {2,0,0,0,0,0,0,2,0,0,0},
//         {0,2,2,0,0,0,0,2,0,0,0},
//         {0,0,0,2,0,0,0,2,0,0,0},
//         {0,0,0,0,2,2,2,2,0,0,0},
//     };
//     plan.walls = walls;
//     return plan;
// }

wallplan planning(fieldmap const *map){
    int width = map->width;
    int height = map->height;

    wallplan plan = {
        width,
        height,
        std::vector<std::vector<int> >(width, std::vector<int>(height, 4))
    };
    //既存の壁をコピー
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            if(map->walls[i][j] == 1){
                plan.walls[i][j] = 1;
            }
        }
    }

    // マップの壁を置く比重(比重の単位は[%])
    const int BASE_WEIGHT = 80;
    const int CASTLE_WEIGHT = (BASE_WEIGHT * (100 - abs(map->castleRate - map->wallRate)))/100;
    std::vector<std::vector<int> > weightmap(width, std::vector<int>(height, BASE_WEIGHT));
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            int cell = map->area[i][j];
            if(cell == 2){ //城に壁は建てづらくする
                weightmap[i][j] = CASTLE_WEIGHT;
            }else if(cell == 1){
                //4方が池なら建築不可領域
                bool canBuild = false;
                for(int k = 0; k < 4; k++){
                    int x = i + dx4[k];
                    int y = j + dy4[k];
                    if(x < 0 || x >= width || y < 0 || y >= height){
                        continue;
                    }
                    if(map->area[x][y] != 1){
                        canBuild = true;
                        break;
                    }
                }
                if(canBuild){
                    weightmap[i][j] = BASE_WEIGHT;
                }else{
                    weightmap[i][j] = 0;
                }
            }else{
                weightmap[i][j] = BASE_WEIGHT;
            }
        }
    }

    //仮壁案をweightmapに基づいて作成
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            if(plan.walls[i][j] == 1){//既に壁があるならスキップ
                continue;
            }else if(random(1,100) <= weightmap[i][j]){
                plan.walls[i][j] = 3; //仮壁案
            }
        }
    }

    // >領域内の壁の削除処理
    // 仮壁込みで領域を塗り潰し
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
        if(plan.walls[x][y] == 4){
            plan.walls[x][y] = 0; 
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
        }else if(plan.walls[x][y] == 3){
            plan.walls[x][y] = 2; //境界の壁を正式な壁(仮)に変更
        }
    }
    // showPlan(&plan);

    //この段階で 既壁 : 1, 仮壁 : 3, 領域 : 4　で案ができている.
    //線状の仮壁を削除
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            if(plan.walls[i][j] >= 1){// 既に壁が存在するならば線状かの判定を行う
                bool isLine = false;
                for(int k = 0; k < 4; k++){
                    int x = i + dx8[k];
                    int xOpp = i + dx8[k+4];
                    int y = j + dy8[k];
                    int yOpp = j + dy8[k+4];
                    int target, opposit;

                    if(x < 0 || x >= width || y < 0 || y >= height){
                        target = 0;
                    }else{
                        target = plan.walls[x][y];
                    }
                    if(xOpp < 0 || xOpp >= width || yOpp < 0 || yOpp >= height){
                        opposit = 0;
                    }else{
                        opposit = plan.walls[xOpp][yOpp];
                    }

                    if(target == 0 && opposit == 0){
                        isLine = true;
                        break;
                    }
                }
                if(isLine){
                    plan.walls[i][j] = 0;
                    for(int k = 0; k < 4; k++){
                        int x = i + dx4[k];
                        int y = j + dy4[k];
                        if(x < 0 || x >= width || y < 0 || y >= height){
                            continue;
                        }
                        if(plan.walls[x][y] > 2){
                            plan.walls[x][y] = 2;
                        }
                    }
                }
            }
        }
    }

    // 外周以外の仮壁，領域の削除
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            if(plan.walls[i][j] > 2){ //仮設定の壁とか領域を削除
                plan.walls[i][j] = 0;
            }
        }
    }

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


    // for(std::vector<int> trr : territory){
    //     for(int c : trr){
    //         char cell = ' ';
    //         switch (c){
    //         case 0: 
    //             cell = '.'; break;
    //         case -1: 
    //             cell = 'X'; break;
    //         case 1: 
    //             cell = '0'; break;
    //         default: //その他
    //             // cell = '.';
    //             cell = '+';
    //             break;
    //         }
    //         std::cout << cell;
    //     }
    //     std::cout << std::endl;
    // }

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

    // for(std::vector<int> trr : territory){
    //     for(int c : trr){
    //         char cell = ' ';
    //         switch (c){
    //         case 0: 
    //             cell = '.'; break;
    //         case -1: 
    //             cell = 'X'; break;
    //         case 1: 
    //             cell = '0'; break;
    //         default: //その他
    //             // cell = '.';
    //             cell = '+';
    //             break;
    //         }
    //         std::cout << cell;
    //     }
    //     std::cout << std::endl;
    // }

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



// int main(){
//     // 1.マップ情報の取得
//     fieldmap map = getmap();
//     showMap(&map);
    
//     // 2.城郭の計画を立案
//     wallplan plan = planning(&map);
//     showPlan(&plan);

//     // 3.城郭の得点計算
//     int point = calcTerritoryPoint(&map, &plan);
//     std::cout << "point : " << point << std::endl;

//     return 0;
// }