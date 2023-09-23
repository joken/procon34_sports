//
// 城郭計画プログラム
//
#include <vector>
#include <iostream>


// ## 領域情報(area)
// * **平地**: 0
// * **池**: 1
// * **城**: 2
struct fieldmap{
    int width; //11~25
    int height;//11~25
    std::vector<std::vector<int> > area;
};


fieldmap getmap(){
    fieldmap map = {
        11,
        11,
        std::vector<std::vecotr<int> > {
            {0,1,0,0,1,0,1,0,0,1,0},
            {0,0,2,0,0,1,0,0,2,0,0},
            {0,0,0,1,0,0,0,1,0,0,0},
            {0,1,0,0,2,0,2,0,0,1,0},
            {0,0,0,0,0,0,0,0,0,0,0},
            {0,0,1,0,0,1,0,0,1,0,0},
            {0,1,0,0,2,0,2,0,0,1,0},
            {0,0,0,1,0,0,0,1,0,0,0},
            {0,0,2,0,0,1,0,0,2,0,0},
            {0,1,0,0,1,0,1,0,0,1,0},
            {0,0,0,0,0,0,0,0,0,0,0},
        }
    };
    return map;
}

int main(){
    
    // 1.マップ情報の取得
    fieldmap map = getmap();
    std::cout << map.width << std::endl;
    // 2.城郭の計画を立案

    // 3.城郭の得点計算


    return 0;
}