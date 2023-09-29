#ifndef CURL_HPP
#define CURL_HPP
#include <curl/curl.h>
#include <nlohmann/json.hpp> 
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <filesystem>
using json = nlohmann::json;
using namespace std;
namespace fs = std::filesystem;

class API{
    private:
        string token;
        string id;
        string baseURL;
        json postData;
        int UNIXTIME = 0;
        const int TIMEOUT = 1;
        int turn;

    public:
        string getTimeStamp(){
            auto now = chrono::system_clock::now();
            auto now_c = chrono::system_clock::to_time_t(now);
            stringstream ss;
            ss << put_time(localtime(&now_c), "%H%M%S");
            return ss.str();
        }
        static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp){
            ((std::string*)userp)->append((char*)contents, size * nmemb);
            return size * nmemb;
        }
        API(string token,string id){
            this->token = "?token="+token;
            baseURL = "http://localhost:3000";
            this->id = id;
            turn = -1;
        }
        
        json get(string url){
            url  = baseURL + url + token;
            CURL * curl;
            CURLcode res;
            string response;
            json j;
            curl = curl_easy_init();
            if(curl) {
                curl_slist* hs = NULL;
                hs = curl_slist_append(hs,"Content-Type: application/json");
                // タイムアウト設定
                curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIMEOUT);
                // 接続先指定
                curl_easy_setopt(curl, CURLOPT_URL,url.c_str());
                // ヘッダー設定
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hs);
                // 
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
                // レスポンス保存先設定
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
                // 接続
                res = curl_easy_perform(curl);
                // 処理結果判定
                if(response == "TooEarly"){ // 競技開始前
                    j["status_code"] = -100;
                    j["message"] =  "TooEarly";
                }else if(res == CURLE_OK){ // 正常処理
                    j = json::parse(response);
                    j["status_code"] = res;
                    j["message"] = "succeeded";
                }else{                    // 接続エラー
                    string message =  "failed"  ;
                    j["message"] = message;
                    cout << "failed:" << curl_easy_strerror(res) << endl;
                    j["status_code"] = res;
                }
                
            }else{                      // 接続エラー
                cout << "curl_easy_init() failed on " + url << endl;
                j["status_code"] = -99;
            }
            curl_easy_cleanup(curl);
            return j;
        }
        json post(json post_data,string url){
            CURL *curl;
            CURLcode res;
            curl = curl_easy_init();
            string data = post_data.dump();
            string response;
            url = baseURL + url + token;
            json j;
            // ターン数のチェック
            json matchInfo = getMatchInfo();
            if(matchInfo["status_code"] != 0){
                j["status_code"] = matchInfo["status_code"];
                j["message"] = matchInfo["message"];
                return j;
            }

            turn = matchInfo["turn"];
            j["next_turn"] = turn+1;
            if(post_data["turn"] != turn + 1){
                j["status_code"] = 100;
                j["message"] = "Trun is incorrect";
                
                return j;
            }
            if (curl){
                curl_slist* hs = NULL;
                hs = curl_slist_append(hs,"Content-Type: application/json");
                // 接続先設定
                curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                curl_easy_setopt(curl, CURLOPT_POST, 1);
                // ヘッダー設定
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hs);
                // タイムアウト設定
                curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIMEOUT);
                // 送信データ設定
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
                // おまじない
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
                // レスポンス取得設定
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, (string*)&response);
                // post
                res = curl_easy_perform(curl);
                // 処理結果判定
                j["status_code"] = res;
                if(response == "TooEarly"){ // 競技開始前
                    j["status_code"] = -1;
                    j["message"] = "TooEarly";
                }else if(res == CURLE_OK){  // 正常処理
                    j = json::parse(response);
                    j["status_code"] = res;
                    j["message"] = "succeeded";
                    if(UNIXTIME < j["accepted_at"]){    // 行動計画が更新されたか?
                        postData = post_data;
                        UNIXTIME = j["accepted_at"];
                    }
                }else{                      // 接続エラー
                    string message =  "failed"  ;
                    j["message"] = message;
                    cout << "failed:" << curl_easy_strerror(res) << endl;
                    j["status_code"] = res;
                }

            }else{                          // 接続エラー
                cout << "curl_easy_init() failed on " + url << endl;
                j["status_code"] = -2;
            }
            curl_easy_cleanup(curl);
            return j;
        }
        

        void saveAsJson(json j,string pass,string filename){
            ofstream ofs(pass+filename+".json");
            if (!ofs.bad())
            {
                ofs << j<< endl;
                ofs.close();
            }
        }

        bool makeDir(string name,string pass){
            pass += name;
            bool result =  fs::create_directory(pass);
            return result;
        }

        void changeId(string id){
            this->id = id;
        }

        string getID(){
            return id;
        }


        int getTrunFromWeb(){
            json j = get("/matches/" + id);
            if(j["status_code"] == 0){ 
                return j["turn"];
            }
            return -1;
        }
        
        void setTrun(int t){
            turn = t;
        }
        json getMatchesInfo(){
            json j = get("/matches");
            saveAsJson(j,"","MatchesInfo-"+getTimeStamp());
            return j;
        }

        json getMatchInfo(){
            json j;
            if(id.size() == 0){
                string In;
                cout << "IDを入力" << endl;
                cin >> In;
                changeId(In);
            }
            j = get("/matches/" + id);
            if(j["status_code"] == 0){ 
                turn = j["turn"];
            }
            makeDir(id,"./");
            saveAsJson(j,"./"+id+"/",to_string(turn));
            return j;
        }

        json postActionPlan(json j){
            UNIXTIME = 0;
            json res = post(j,"/matches/"+id);
            return res;
        }

        json convertJson(vector<vector<int>> ActionPlan,int next_turn){
            json j;
            j["turn"] = next_turn;
            for(int i = 0;i < ActionPlan.size();i++){
                json temp;
                int type = ActionPlan[i][0];
                int dir = ActionPlan[i][1];
                temp["type"] = type;
                temp["dir"] = dir;
                j["actions"].push_back(temp);
            }
            return j;

        }
};
#endif // CURL_HPP