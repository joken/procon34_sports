#include "Agent.hpp"
#include "Field.hpp"

int main(){
    Field map = Field();
    wallplan plan = map.planning();

    for(int i = 0; i < 8; i++){
        int dx = dx8[i];
        int dy = dy8[i];
        auto dir = ((int)(atan2(dx, dy)*4/M_PI/*[0,2π)から[0, 8)へ*/ + 8/*負の角度を排除*/ + 3.5) % 8);
        std::cout << "( " << dx << " , " << dy << " ) : " << dir << std::endl;
    }

    std::cout << std::endl;

    for(int i = 0; i < 10; i++){
        int dx = random(-5, 5);
        int dy = random(-5, 5);
        auto dir = ((int)(atan2(dx, dy)*4/M_PI/*[0,2π)から[0, 8)へ*/ + 8/*負の角度を排除*/ + 3.5) % 8);
        // int dir = ((int)(atan2(dx, dy)*4/M_PI + 8 + 3) % 8);
        // int dir = ((int)(-atan2(dx, dy)*4/M_PI +17) % 8) + 1;
        std::cout << "( " << dx << " , " << dy << " ) : " << dir << std::endl;
    }

    
}