#include <iostream>
#include "Curl.hpp"
#include <nlohmann/json.hpp> 
using namespace std;


int main(){
    std::string token = "narae27ec497de2b1d099afa2659f429c4d1cb4eb8ef725f520a05842ff65171"; //大会当日に書き換える
    std::string id = "1";        // 試合ID
    std::string baseURL = "http://172.28.0.1:8080";
    
    API api = API(token, id, baseURL);
    json j =  api.getMatchesInfo();
    return 0;
}