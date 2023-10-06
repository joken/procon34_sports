///
/// main.cpp : ゲーム進行プログラム
///
#include <unistd.h>

#include "Curl.hpp"
#include "castle_tactics.cpp"
#include "agent.cpp"

struct Match{
    int turns;      // 30~200
    int turnSeconds;// 3~15
    bool first;     // 先手かどうか
    fieldmap field; // マップ情報
};

int updateField(fieldmap *map, API *api){
    json matchData = api->getMatchInfo();
    map->masons = vector<vector<int> >(matchData["board"]["masons"]);
    map->walls = vector<vector<int> >(matchData["board"]["walls"]);
    map->territory = vector<vector<int> >(matchData["board"]["territories"]);
    // for(int i = 0; i < map->width; i++){
    //     for(int j = 0; j < map->height; j++){
    //         map->masons[i][j] = (int)matchData["matches"][0]["board"]["masons"][i][j];
    //         map->walls[i][j] = (int)matchData["matches"][0]["board"]["walls"][i][j];
    //     }
    // }
    return (int)matchData["turn"];
}

Match initialMatches(json js){
    Match match;
    match.turns = (int)js["matches"][0]["turns"];
    match.turnSeconds = (int)js["matches"][0]["turnSeconds"];
    match.first = (bool)js["matches"][0]["first"];
    fieldmap field; // returnにファイルが付いてきていない気がする
    field.width = (int)js["matches"][0]["board"]["width"];
    field.height= (int)js["matches"][0]["board"]["height"];
    field.masonNum = (int)js["matches"][0]["board"]["mason"];
    field.castleRate    = (int)js["matches"][0]["bonus"]["castle"];
    field.territoryRate = (int)js["matches"][0]["bonus"]["territory"];
    field.wallRate      = (int)js["matches"][0]["bonus"]["wall"];
    
    field.area = vector<vector<int> >(js["matches"][0]["board"]["structures"]); 
    field.masons = vector<vector<int> >(js["matches"][0]["board"]["masons"]); 
    field.walls = vector<vector<int> >(field.width, vector<int>(field.height, 0));
    field.territory = vector<vector<int> >(field.width, vector<int>(field.height, 0));
    // vector<vector<int> > area = vector<vector<int> >(field.width, vector<int>(field.height, 0));
    // vector<vector<int> > objects = vector<vector<int> >(field.width, vector<int>(field.height, 0));
    // vector<vector<int> > territory = vector<vector<int> >(field.width, vector<int>(field.height, 0));
    
    // for(int i = 0; i < field.width; i++){
    //     for(int j = 0; j < field.height; j++){
    //         area[i][j] = (int)js["matches"][0]["board"]["masons"][i][j];
    //         objects[i][j] = (int)js["matches"][0]["board"]["masons"][i][j];
    //         // territory[i][j] = 0;
    //     }
    // }

    // field.area = area;
    // field.objects = objects;
    // field.territory = territory;
    match.field = field;
    return match;
}

int main(){
    std::string token = "token"; //大会当日に書き換える
    std::string id = "1";        // 試合ID
    API api = API(token, id);

    //　試合データの取得
    json matchInfo = api.getMatchesInfo();
    api.changeId(to_string(matchInfo["matches"][0]["id"]));

    Match match = initialMatches(matchInfo);
    // showMap(&(match.field));
    cerr << match.turns << endl;

    // ターンのループ開始
    for(int turnCounter = 0; turnCounter < match.turns; turnCounter++){
        showMap(&(match.field));
        // 現在のマップ状況取得
        int now_turn = updateField(&(match.field), &api);
        cout << "now turn : " << now_turn << endl;

        // 方策決定
        wallplan plan = planning(&(match.field));
        vector<pair<int, int> > masons = getMasons(&(match.field));

        // agentに割り振り
        vector<vector<int> > action = vector<vector<int>>(match.field.masonNum, vector<int>({random(0,2), 2*random(1,4)}));
        for(int k = 0; k < match.field.masonNum; k++){
            action[k] = agentAction(&(masons[k]), &(match.field), &plan);
        }


        // サーバに行動を送信
        cout << api.postActionPlan(api.convertJson(action, now_turn)) << endl;

        while(true){
            int next_turn = updateField(&(match.field), &api);
            if(next_turn != now_turn){
                break;
            }else{
                sleep(match.turnSeconds/3);
            }
        }
    }

}