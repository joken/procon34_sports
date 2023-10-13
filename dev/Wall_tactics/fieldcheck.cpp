#include "Field.hpp"

int main(){
    Field map = Field();
    wallplan plan = map.planning();

    map.showMap();
    showPlan(&plan);
    std::cout << map.calcTerritoryPoint(&plan) << std::endl;

    map.showFriendMasons();
}