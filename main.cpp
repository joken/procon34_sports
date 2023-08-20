#include <iostream>
#include <random>
using namespace std;

class bourdInfomation{

public:
    void setBourd(string gameMap[],int widthSize, int heightSize){
        for (int i = 0; i < heightSize; i++){
        for (int j = 0; j < widthSize; j++){

            gameMap[i][j] = "* ";

        }
    }

    }

    void printBourdInfo(string gameMap[], int widthSize, int heightSize){
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
    bourdInfomation out;

    int v1 = rand() % 10 + 1;
    int v2 = rand() % 10 + 1;

    std::string map[v1][v2];

    out.setBourd(map[v1],v1,v2);

    map[rand() % 10][rand() % 10] = "c1 ";
    map[rand() % 10][rand() % 10] = "c2 ";

    map[rand() % 10][rand() % 10] = "b1 ";
    map[rand() % 10][rand() % 10] = "b2 ";
    map[rand() % 10][rand() % 10] = "b3 ";

    map[rand() % 10][rand() % 10] = "e1 ";
    map[rand() % 10][rand() % 10] = "e2 ";
    map[rand() % 10][rand() % 10] = "e3 ";

    out.printBourdInfo(map[v1], v1, v2);

    std::cout << v1 << v2 << std::endl;

}