#include <iostream>
#include "Curl.hpp"

using namespace std;


int main(){
    // 行動計画(仮)
    vector<vector<int>> ac = {{1,1},{1,4}};
    // 先攻
    API a = API("1","10");
    // 後攻
    API b = API("2","10");
    
    // 試合一覧を取得
    cout << a.getMatchesInfo() << endl;
    // 試合id10の試合情報を取得
    cout << a.getMatchInfo() << endl;
    
    // 行動計画をjsonに変換
    json sendData = a.convertJson(ac,1);
    // 行動計画を送信
    cout << a.postActionPlan(sendData) << endl;    
}
