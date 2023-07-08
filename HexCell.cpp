#include "HexCell.h"
#include <vector>
#include <cmath>

HexCell::HexCell(int Q, int R) {

    q = Q; 
    r = R;
    path_dist_from_start  = 0;

    for (int i = 0; i < 6; i++) {
        neighbours[i] = nullptr;
    }

    for (int i = 0; i < 6; i++) {
        links[i] = nullptr;
    }
}

void HexCell::link(HexCell* cell) {
    if (cell == nullptr) return; 
    if (distance(cell) != 1) return;

    int dq = cell->q;
    int dr = cell->r;


    int side = -1;
    if (dq == q){
        if (dr == r-1){
            side = N;
        }
        else if (dr == r+1){
            side = S;
        }
    }
    else if (dq == q+1){
        if (dr == r-1){
            side = NE;
        }
        else if (dr == r){
            side = SE;
        }
    }
    else if (dq == q-1){
        if (dr == r+1){
            side = SW;
        }
        else if (dr == r){
            side = NW;
        }
    }

    links[side] = cell;
    if (side < 3){
        cell->links[side + 3] = this; 
    }
    else {
        cell->links[side - 3] = this;
    }
}

int HexCell::get_neighbour_side(const HexCell* cell) const {

    for (int i = 0; i < 6; i++){
        if (neighbours[i] == cell){
            return i;
        }
    }
    return -1;
}
bool HexCell::is_linked(const HexCell* cell) const {

    if (cell == nullptr) return false;

    for (int i = 0 ; i < 6; i++){
        if (links[i] == cell){
            return true;
        }
    }
    return false;
}


int HexCell::distance(const HexCell* cell) const {

    if (cell == nullptr) return -1; 
    
    return (abs(q - cell->q) + abs(q + r - cell->q - cell->r) + abs(r - cell->r))/2;
}

bool HexCell::is_edge() const {
    for (int i = 0; i < 6; i++) {
        if (neighbours[i] == nullptr) return true;
    }
    return false;
}