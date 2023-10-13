#include <cmath>
#include <iostream>

int dx8[8] = {1, 0, -1, -1, -1, 0, 1, 1};
int dy8[8] = {1, 1, 1, 0, -1, -1, -1, 0};


int main(){
    for(int i = 0; i < 8; i++){
        std::cout << (int)(-atan2(dx8[i], dy8[i])*4/M_PI +17) % 8 << std::endl;
        // std::cout << atan2(dy8[i], dx8[i])/M_PI  +2<< std::endl;
    }
}
