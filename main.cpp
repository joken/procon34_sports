#include <iostream>
#include <random>
using namespace std;

int main(){

    int v1 = rand() % 10 +1;
    int v2 = rand() % 10 +1;

    vector<vector<string> > map(v1, vector<string>(v2));

    for (int i = 0; i < v1; i++){
        for (int j = 0; j < v2; j++){
            map.at(i).at(j) = "* ";
        }
    }

    map.at(rand() % 10).at(rand() % 10) = "c1 ";
    map.at(rand() % 10).at(rand() % 10) = "c2 ";

    map.at(rand() % 10).at(rand() % 10) = "b1 ";
    map.at(rand() % 10).at(rand() % 10) = "b2 ";
    map.at(rand() % 10).at(rand() % 10) = "b3 ";

    map.at(rand() % 10).at(rand() % 10) = "e1 ";
    map.at(rand() % 10).at(rand() % 10) = "e2 ";
    map.at(rand() % 10).at(rand() % 10) = "e3 ";

    for(int i = 0; i < v1; i++){
        for(int j = 0; j <v2; j++){
            cout << map.at(i).at(j);
        }
        cout << " " << endl;
    }
    cout << v1 << v2 <<endl;
}
