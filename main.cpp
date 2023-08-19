#include <iostream>
#include <random>
using namespace std;

class printMap{

public:

    void printMapInfo(string gameMap[], int widthSize, int heightSize){
        for (int i = 0; i < heightSize; i++){
            for (int j = 0; j < widthSize; j++){

                std::cout << gameMap[i][j];

            }

            std::cout << " " << std::endl;

        }

    }
};

class moveBuilder{
public:
    void move(){

    }
};

int main(){
    printMap out;

    int v1 = rand() % 10 + 1;
    int v2 = rand() % 10 + 1;

    string map[v1][v2];

    for (int i = 0; i < v1; i++){
        for (int j = 0; j < v2; j++){

            map[i][j] = "* ";

        }
    }

    map[rand() % 10][rand() % 10] = "c1 ";
    map[rand() % 10][rand() % 10] = "c2 ";

    map[rand() % 10][rand() % 10] = "b1 ";
    map[rand() % 10][rand() % 10] = "b2 ";
    map[rand() % 10][rand() % 10] = "b3 ";

    map[rand() % 10][rand() % 10] = "e1 ";
    map[rand() % 10][rand() % 10] = "e2 ";
    map[rand() % 10][rand() % 10] = "e3 ";

    out.printMapInfo(map[v2], v1, v2);

    std::cout << v1 << v2 << std::endl;
    
}