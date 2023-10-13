///
/// main.cpp : ゲーム進行プログラム
///
#include <unistd.h>

#include "Curl.hpp"
#include "Field.hpp"
#include "Agent.hpp"

struct Match{
    int turns;      // 30~200
    int turnSeconds;// 3~15
    bool first;     // 先手かどうか
    Field field; // マップ情報
};

Match initialMatches(json js, int num){
    Match match;
    match.turns = (int)js["matches"][num]["turns"];
    match.turnSeconds = (int)js["matches"][num]["turnSeconds"];
    match.first = (bool)js["matches"][num]["first"];
    match.field = Field(js, num);
    return match;
}

void showAction(vector<vector<int> > action){
    for(int i = 0; i < action.size(); i++){
        switch (action[i][0])
        {
        case 0:
            std::cout << "待機 : ";
            break;
        case 1:
            std::cout << "移動 : ";
            break;
        case 2:
            std::cout << "建築 : ";
            break;
        case 3:
            std::cout << "破壊 : ";
            break;
        default:
            std::cout << action[i][0] << " : ";
            break;
        }
        std::cout << action[i][1] << std::endl;
    }
}

int main(int argc, char *argv[]){
    std::string token = "token"; //大会当日に書き換える
    std::string id = "1";        // 試合ID
    std::string baseURL = "http://localhost:3000";
    // std::string baseURL = "https://www.procon.gr.jp";
    int matchNum = atoi(argv[1]);

    // std::cerr << "hoge" << std::endl;
    API api = API(token, id, baseURL);
    
    //　試合データの取得
    json matchInfo = api.getMatchesInfo();
    api.changeId(to_string(matchInfo["matches"][matchNum]["id"]));

    Match match = initialMatches(matchInfo, matchNum);

   // ターンのループ開始
    for(int turnCounter = 0; turnCounter < match.turns; turnCounter++){
        // showMap(&(match.field));
        // 現在のマップ状況取得
        int now_turn = match.field.updateField(&api);
        cout << "now turn : " << now_turn << endl;

        // 方策決定
        wallplan plan = match.field.planning();
        vector<pair<int, int> > masonsVec = match.field.getFrientMasons();
        int masonNum = match.field.getMasonNum();
        vector<Agent> masons;
        for(int i = 0; i < masonNum; i++){
            masons.push_back(Agent(i, masonsVec[i]));
        }
        // agentに割り振り
        vector<vector<int> > action = vector<vector<int>>(masonNum, vector<int>{random(0,2), 2*random(1,4)});
        // vector<vector<int> > action;
        for(int k = 0; k < masonNum; k++){
            // std::cerr << "hoge" << std::endl;
            action[k] = masons[k].action(&(match.field), &plan);
            // for (int i = 0; i < action[k].size(); ++i) {
            //     std::cerr << action[k][i] << "; ";
            // }
            // std::cerr << std::endl;
            // action.push_back(masons[k].action(&(match.field), &plan));
        }

        showAction(action);


        // サーバに行動を送信
        cout << api.postActionPlan(api.convertJson(action, now_turn+1)) << endl;

        match.field.showMasons();
        match.field.showWalls();
        while(true){
            int next_turn = match.field.updateField(&api);
            if(next_turn != now_turn){
                break;
            }else{
                sleep(match.turnSeconds/3);
            }
        }
    }

}