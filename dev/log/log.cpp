#ifndef LOG_HPP
#define LOG_HPP
#include <curl/curl.h>
#include <nlohmann/json.hpp> 
#include <chrono>
#include<iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <filesystem>
using json = nlohmann::json;

using namespace std;
namespace fs = std::filesystem;

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

void savePostAPILog(){
    makeDir(post,"./id");
    saveAsJson(j,"./"+id+"/",to_string(turn))

}

void savePlanningLog(){
    makeDir(plan,"./id");
    saveAsJson(j,"./"+id+"/",to_string(turn))

}

json loadAPILog(vector<vector<int>> v){
    makeDir(loadAPI,"./id");
    json loadAPI = {
		{,},
		{,},
		{,}
	};

}

json loadGetAPILog(vector<vector<int>> v){
    makeDir(loadGetAPI,"./id");
    json j;
    j = {
        {"",""},
        {"",""},
        {"".}
    };

    string filename = j[true];
	string Extension = ".json";
	filename += Extension;

    //作成したファイルに内容を書き込む
	ofstream writing_file;
	writing_file.open(filename, ios::out);
	writing_file << j.dump() << endl;
	writing_file.close();

    j.dunp()

    return j;

}

json loadWallPlanLog(vector<vector<int>> v){
    makeDir(loadWallPlan,"./id");
    json j;
    j = {
        {"",""},
        {"",""},
        {"".}
    };

    string filename = j[true];
	string Extension = ".json";
	filename += Extension;

    //作成したファイルに内容を書き込む
	ofstream writing_file;
	writing_file.open(filename, ios::out);
	writing_file << j.dump() << endl;
	writing_file.close();

    j.dunp()

    return j;
}

json loadPostAPILog(){
    makeDir(loadPostAPI,"./id");
    json j;
    j = {
        {"",""},
        {"",""},
        {"".}
    };

    string filename = j[true];
	string Extension = ".json";
	filename += Extension;

    //作成したファイルに内容を書き込む
	ofstream writing_file;
	writing_file.open(filename, ios::out);
	writing_file << j.dump() << endl;
	writing_file.close();

    j.dunp()

    return j;
}
#endif LOG_HPP
