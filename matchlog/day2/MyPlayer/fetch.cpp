#include "Curl.hpp"
#include "Field.hpp"
#include "Agent.hpp"

int main(){
    std::string token = "narae27ec497de2b1d099afa2659f429c4d1cb4eb8ef725f520a05842ff65171"; //大会当日に書き換える
    std::string id = "1";        // 試合ID
    // std::string baseURL = "http://localhost:3000";
    std::string baseURL = "http://172.28.0.1:8080";
    // int matchNum = atoi(argv[1]);
    wallplan plan;

    // std::cerr << "hoge" << std::endl;
    API api = API(token, id, baseURL);

    json matchInfo = api.getMatchesInfo();

}