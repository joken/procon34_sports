///
/// main.cpp : ゲーム進行プログラム
///
#include "Curl.hpp"
#include "castle_tactics.cpp"

struct Match{
    int turns;      // 30~200
    int turnSeconds;// 3~15
    bool first;     // 先手かどうか
    fieldmap field; // マップ情報
};

void initialMatch(Match *match, json j){
    cerr << "hoge" << endl;
    match->turns = j["matches"][0]["turns"];
    match->turnSeconds = j["matches"][0]["turnSeconds"];
    match->first = j["matches"][0]["first"];
    fieldmap field = {
        j["matches"][0]["board"]["width"],
        j["matches"][0]["board"]["height"],
        j["matches"][0]["board"]["mason"],
        j["matches"][0]["board"]["bonus"]["castle"],
        j["matches"][0]["board"]["bonus"]["territory"],
        j["matches"][0]["board"]["bonus"]["wall"],
    };
    cerr << "hoge" << endl;
    vector<vector<int> > area = vector<vector<int> >(field.width, vector<int>(field.height, 0));
    vector<vector<int> > objects = vector<vector<int> >(field.width, vector<int>(field.height, 0));
    vector<vector<int> > territory = vector<vector<int> >(field.width, vector<int>(field.height, 0));
    cerr << "hoge" << endl;
    
    for(int i = 0; i < field.width; i++){
        for(int j = 0; j < field.height; j++){
            area[i][j] = vector<vector<int> >(json(j["matches"])[0]["board"]["structures"])[i][j];
            objects[i][j] = vector<vector<int> >(json(j["matches"])[0]["board"]["masons"])[i][j];
            territory[i][j] = 0;
        }
    }
    cerr << "hoge" << endl;

    field.area = area;
    field.objects = objects;
    field.territory = territory;
    match->field;
}

int main(){
    std::string token = "token"; //大会当日に書き換える
    std::string id = "1";        // 試合ID
    API api = API(token, id);
    Match match;

    //　試合データの取得
    json matchInfo = api.getMatchesInfo();
    api.changeId(json(matchInfo["matches"])[0]["id"]); //-
    cerr << "hoge" << endl;

    initialMatch(&match, matchInfo);
    showMap(&(match.field));
    cerr << "hoge" << endl;

    // ターンのループ開始

    

    // 現在のマップ状況取得

    // 方策決定
    // ローカルに保存

    // agentに割り振り


    // サーバに行動を送信

}