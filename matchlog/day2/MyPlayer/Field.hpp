#ifndef FIELD_HPP
#define FIELD_HPP
#include <vector>
#include <iostream>
#include <deque>
#include <random>
#include <string>
#include "Curl.hpp"

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
int dx4[4] = {-1, 0, 1, 0};
int dy4[4] = {0, 1, 0, -1};

int dx8[8] = {-1, -1, -1, 0, 1, 1, 1, 0};
int dy8[8] = {-1, 0, 1, 1, 1, 0, -1, -1};

std::random_device rd;
std::mt19937 gen(rd());
 
int random(int low, int high)
{
    std::uniform_int_distribution<> dist(low, high);
    return dist(gen);
}

// ## 領域情報(area)
// * 平地: 0
// * 池: 1
// * 城: 2
// ## 物体(masons)
// * None : 0
// * 味方職人 : 正の整数( 1~ 6 <= [2,6])
// * 相手職人 : 負の整数(-1~-6 <= [-2,-6])
// * 味方城壁 :  9
// * 相手城壁 : -9
// ## 領地(territory)
// * None : 0
// * 味方領地 : 1
// * 相手領地 : 2
// * 両方の領地 : 3
class Field{
    private:
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

        std::vector<std::pair<int,int> > friendMasons;

        void updateFriendMasons();
    public:
        Field(json js, int num){
                this->width = (int)js["matches"][num]["board"]["width"];
                this->height= (int)js["matches"][num]["board"]["height"];
                this->masonNum = (int)js["matches"][num]["board"]["mason"];
                this->castleRate    = (int)js["matches"][num]["bonus"]["castle"];
                this->territoryRate = (int)js["matches"][num]["bonus"]["territory"];
                this->wallRate      = (int)js["matches"][num]["bonus"]["wall"];
                
                this->area = vector<vector<int> >(js["matches"][num]["board"]["structures"]); 
                this->masons = vector<vector<int> >(js["matches"][num]["board"]["masons"]); 
                this->walls = vector<vector<int> >(this->width, vector<int>(this->height, 0));
                this->territory = vector<vector<int> >(this->width, vector<int>(this->height, 0));
                updateFriendMasons();
        }
        Field(){//ダミーデータのコンストラクタ
            this->width = 11;
            this->height = 11;
            this->masonNum = 2;
            this->castleRate = 100;
            this->territoryRate = 30;
            this->wallRate = 10;
            this->area = std::vector<std::vector<int> > {
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
            };
            this->masons = std::vector<std::vector<int> > {
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
            };
            this->walls = std::vector<std::vector<int> > {
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
            };
            this->territory = std::vector<std::vector<int> > {
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
            };
            updateFriendMasons();
        }

        void showMap(){
            std::cout << "=========================" << std::endl;
            std::cout << "width : " << this->width << std::endl;
            std::cout << "height: " << this->height << std::endl;
            std::cout << "masonNum: " << this->masonNum << std::endl;
            std::cout << "castle Rate   : " << this->castleRate << std::endl;
            std::cout << "territory Rate: " << this->territoryRate << std::endl;
            std::cout << "wall Rate     : " << this->wallRate << std::endl;
            std::cout << "--- area ---" << std::endl;
            for(std::vector<int> mp : this->area){
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
            for(std::vector<int> obj : this->masons){
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
            for(std::vector<int> obj : this->walls){
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
            for(std::vector<int> trr : this->territory){
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
        void showMasons(){
            std::cout << "--- masons ---" << std::endl;
            for(std::vector<int> obj : this->masons){
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

        }
        void showWalls(){
            std::cout << "--- walls ---" << std::endl;
            for(std::vector<int> wall : this->walls){
                for(int c : wall){
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
        }
        void showArea(){
            std::cout << "--- Area --- | --- Walls --- | --- Masons --- " << std::endl;
            for(int i = 0; i < this->height; i++){
                for(int j = 0; j < this->width; j++){
                    char cell = ' ';
                    switch (this->area[i][j]){
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
                std::cout << " | ";
                for(int j = 0; j < this->width; j++){
                    char cell = ' ';
                    switch (this->walls[i][j]){
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
                std::cout << " | ";
                for(int j = 0; j < this->width; j++){
                    char cell = ' ';
                    if(this->masons[i][j] == 0){
                        cell = '.';
                    }else if(this->masons[i][j] > 0){
                        cell = '0' + this->masons[i][j];
                    }else if(this->masons[i][j] < 0){
                        cell = 'E';
                    }else{
                        cell = 'X';
                    }
                    std::cout << cell;
                } 
                std::cout << std::endl;
            }
        }
        void showPoint(){
            int FpCastle = 0;
            int FpTerritory = 0;
            int FpWall = 0;

            for(int i = 0; i < width; i++){
                for(int j = 0; j < height; j++){
                    if(this->territory[i][j] == 1){
                        if(this->area[i][j] == 2) {
                            FpCastle++;
                        }else{
                            FpTerritory++;
                        }
                    }else if(this->walls[i][j] == 1){
                        FpWall++;
                    }
                }
            }
            std::cout << "--- Friend Point ---" << std::endl;
            std::cout << "Castle    : " << FpCastle*this->castleRate << std::endl;
            std::cout << "Territory : " << FpTerritory*this->territoryRate << std::endl;
            std::cout << "Wall      : " << FpWall*this->wallRate << std::endl;
            std::cout << "*total : " << FpCastle*this->castleRate + FpTerritory*this->territoryRate + FpWall*this->wallRate << std::endl;


            int EpCastle = 0;
            int EpTerritory = 0;
            int EpWall = 0;
            for(int i = 0; i < width; i++){
                for(int j = 0; j < height; j++){
                    if(this->territory[i][j] == 2){
                        if(this->area[i][j] == 2) {
                            EpCastle++;
                        }else{
                            EpTerritory++;
                        }
                    }else if(this->walls[i][j] == 2){
                        EpWall++;
                    }
                }
            }
            std::cout << "--- Enemy Point ---" << std::endl;
            std::cout << "Castle    : " << EpCastle*this->castleRate << std::endl;
            std::cout << "Territory : " << EpTerritory*this->territoryRate << std::endl;
            std::cout << "Wall      : " << EpWall*this->wallRate << std::endl;
            std::cout << "*total : " << EpCastle*this->castleRate + EpTerritory*this->territoryRate + EpWall*this->wallRate << std::endl;

            
        }
        wallplan planning(int turn);
        int calcTerritoryPoint(wallplan const *plan);
        int updateField(API *api);

        int getWidth(){
            return this->width;
        }
        int getHeight(){
            return this->height;
        }
        int getMasonNum(){
            return this->masonNum;
        }
        
        std::vector<std::vector<int> > getArea(){
            return this->area;
        }
        std::vector<std::vector<int> > getMasons(){
            return this->masons;
        }
        std::vector<std::vector<int> > getWalls(){
            return this->walls;
        }
        std::vector<std::vector<int> > getTerritory(){
            return this->territory;
        }
        std::vector<std::pair<int, int> > getFriendMasons(){
            return this->friendMasons;
        }
        
};


void Field::updateFriendMasons(){
    int width = this->width;
    int height = this->height;

    this->friendMasons = std::vector<std::pair<int, int>>(this->masonNum);

    for(int i = 0; i < width; i++){
        for(int j =0; j < height; j++){
            if(this->masons[i][j] > 0){
                this->friendMasons[this->masons[i][j] -1] = std::make_pair(i, j);
            }
        }
    }
}

// wallplan planning(fieldmap const *map){ // ダミーデータ
//     wallplan plan;
//     plan.width = this->width;
//     plan.height = this->height;
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

wallplan Field::planning(int turn){
    int width = this->width;
    int height = this->height;

    wallplan plan = {
        width,
        height,
        std::vector<std::vector<int> >(width, std::vector<int>(height, 4))
    };
    //既存の壁をコピー
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            if((this->territory[i][j] & 1) == 1){
                plan.walls[i][j] = 1;
            }
        }
    }

    // マップの壁を置く比重(比重の単位は[%])
    const int BASE_WEIGHT = 80;
    const int CASTLE_WEIGHT = (BASE_WEIGHT * (100 - abs(this->castleRate - this->wallRate)))/100;
    std::vector<std::vector<int> > weightmap(width, std::vector<int>(height, BASE_WEIGHT));
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            int cell = this->area[i][j];
            if(cell == 2){ //城に壁は建てづらくする
                weightmap[i][j] = CASTLE_WEIGHT;
            }else if(cell == 1){
                bool canBuild = false;//4方が池なら建築不可領域
                for(int k = 0; k < 4; k++){
                    int x = i + dx4[k];
                    int y = j + dy4[k];
                    if(x < 0 || x >= width || y < 0 || y >= height){
                        continue;
                    }
                    if(this->area[x][y] != 1){
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
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            int cell = this->area[i][j];
            if(cell == 2){ //城の周りは必ず城郭(仮城壁)を設ける
                for(int k = 0; k < 4; k++){
                    int x = i + dx4[k];
                    int y = j + dy4[k];
                    if(x < 0 || x >= width || y < 0 || y >= height){
                        continue;
                    }
                    if(this->area[x][y] != 2){//城が林立している可能性を考慮
                        weightmap[x][y] = 100;

                    }
                }
            }
        }
    }


    //仮壁案をweightmapに基づいて作成
    // 完全ランダム
    // for(int i = 0; i < width; i++){
    //     for(int j = 0; j < height; j++){
    //         if(plan.walls[i][j] == 1){//既に壁があるならスキップ
    //             continue;
    //         }else if(random(1,100) <= weightmap[i][j]){
    //             plan.walls[i][j] = 3; //仮壁案
    //         }
    //     }
    // }

    //　職人付近優遇案
    std::vector<std::vector<int> > distMap = std::vector<std::vector<int> >(width, std::vector<int>(height, 1000));
    int distBoarder = 6;

    std::deque<std::pair<int, int> > headM;
    for(std::pair<int,int> ms: this->friendMasons){
        headM.push_back(ms);
        distMap[ms.first][ms.second] = 0;
    }
    
    // 幅優先探索
    while(!headM.empty()){
        std::pair<int, int> here = headM.front();
        int x = here.first;
        int y = here.second;
        int dist = distMap[x][y];

        headM.pop_front();
        if(distMap[x][y] < distBoarder){
            continue;
        }
        if(area[x][y] != 1 ){
            
            //4近傍で壁があるかをチェック(field外は除外)
            for(int k = 0; k < 4; k++){
                int nx = x + dx4[k];
                int ny = y + dy4[k];
                if(nx < 0 || nx >= width || ny < 0 || ny >= height){
                    continue;
                }
                if(dist+1 < distMap[nx][ny]){
                    distMap[nx][ny] = min(dist+1, distMap[nx][ny]);
                    if(dist <= distBoarder){
                        headM.push_back(std::make_pair(nx, ny));
                    }
                }
                
            }
        }
    }
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            if(distMap[i][j] <= distBoarder){//既に壁があるならスキップ
                plan.walls[i][j] = 3; //職人の近くを壁案とする
            }else if(random(1,100) <= weightmap[i][j]*0.8){
                plan.walls[i][j] = 3; //仮壁案
            }
        }
    }

    // >領域内の壁の削除処理
    // 仮壁込みで領域を塗り潰し
    // マップの外周に4近傍で隣接する領域を領地以外に上書きする(深さ優先探索)
    std::deque<std::pair<int, int> > headE;
    // 外周を始点とする
    for(int i = 0; i < width; i++){
        headE.push_back(std::make_pair(i, 0));
        headE.push_back(std::make_pair(i, height-1));
    }
    for(int j = 1; j < height -1; j++){
        headE.push_back(std::make_pair(0, j));
        headE.push_back(std::make_pair(width-1, j));
    }
    // 深さ優先探索
    while(!headE.empty()){
        std::pair<int, int> here = headE.back();
        int x = here.first;
        int y = here.second;
        // alart(x, y);
        headE.pop_back();
        if(plan.walls[x][y] == 4){
            plan.walls[x][y] = 0; 
            //4近傍を探索(field外は除外)
            if((0< x-1 && x-1 < width-1) && (0< y && y < height-1)){// 上
                headE.push_back(std::make_pair(x-1, y));
            }
            if((0< x+1 && x+1 < width-1) && (0< y && y < height-1)){// 下
                headE.push_back(std::make_pair(x+1, y));
            }
            if((0< x && x < width-1) && (0< y-1 && y-1 < height-1)){// 左
                headE.push_back(std::make_pair(x, y-1));
            }
            if((0< x && x < width-1) && (0< y+1 && y+1 < height-1)){// 右
                headE.push_back(std::make_pair(x, y+1));
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

    //外周の壁の数がターン数に対して多すぎる場合減らす．
    int aroundWalls = 0;
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            if(plan.walls[i][j] == 2){ //外周の壁
                aroundWalls++;
            }
        }
    }
    if(aroundWalls > masonNum * turn * 0.3){//
        //外壁を侵食させる
        //この段階で 既壁 : 1,  仮壁 : 3, 領域 : 4　で案ができている.
        //線状の仮壁を削除
        for(int i = 0; i < width; i++){
            for(int j = 0; j < height; j++){
                if(plan.walls[i][j] == 2){// 建築予定壁を削る
                    for(int k = 0; k < 8; k++){
                        int x = i + dx8[k];
                        int y = j + dy8[k];
                        int target, opposit;
                        if(x < 0 || x >= width || y < 0 || y >= height){
                            continue;
                        }
                        if(plan.walls[i][j] >= 3){ //仮の壁，領域に外周を譲る
                            plan.walls[i][j] = 2;
                        }
                    }
                    plan.walls[i][j] = 0;
                }
            }
        }
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
    }

    // 城の周りは壁を確実に生成する
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            if(weightmap[i][j] == 100){ //外周の壁
                plan.walls[i][j] = 2;
            }
            if(this->area[i][j] == 2){
                plan.walls[i][j] = 0;
            }
        }
    }

    // 外周以外の仮壁，領域の削除
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            if(plan.walls[i][j] > 2){ //仮設定の壁とか領域を削除
            // if(plan.walls[i][j] == 4){ //仮設定の壁とか領域を削除
                plan.walls[i][j] = 0;
            }
        }
    }

    return plan;
}

int Field::calcTerritoryPoint(wallplan const *plan){
    int width = this->width;
    int height = this->height;
    
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
                if(this->area[i][j] == 2){
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
    point = (area * this->territoryRate) + (walls * this->wallRate) + (castles * this->castleRate);

    return point;
}

int Field::updateField(API *api){
    json matchData = api->getMatchInfo();
    this->masons = vector<vector<int> >(matchData["board"]["masons"]);
    this->walls = vector<vector<int> >(matchData["board"]["walls"]);
    this->territory = vector<vector<int> >(matchData["board"]["territories"]);
    // for(int i = 0; i < map->width; i++){
    //     for(int j = 0; j < map->height; j++){
    //         map->masons[i][j] = (int)matchData["matches"][num]["board"]["masons"][i][j];
    //         map->walls[i][j] = (int)matchData["matches"][num]["board"]["walls"][i][j];
    //     }
    // }
    updateFriendMasons();
    return (int)matchData["turn"];
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

void updatePlan(wallplan *plan, Field *map){
    int width = map->getWidth();
    int height = map->getHeight();
    std::vector<std::vector<int> > walls = map->getWalls();
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            if(walls[i][j] == 1) {
                plan->walls[i][j] = 2;
            }
        }
    }
}


#endif // CURL_HPP