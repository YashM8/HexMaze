#include "HexBoard.h"

#include <iostream>

#include "queue.h"
#include "stack.h"

HexBoard::HexBoard(int radius) {
     this->radius = radius;
     create_board();
     configure_cells();
     longest_path_dist = 0;
}

/**
 * Creates all necessary HexCells up to the given radius and insert them into this->cells.
 */
void HexBoard::create_board() {
    if (radius == 0) return;
    // if (radius == 1) cells[{0, 0}] = new HexCell(0, 0);

    int start = -radius + 1;
    int stop = radius;
    for (int q = start; q < stop; q++) {

        int r_start = max(-radius, -q-radius) + 1;
        int r_stop = min(radius, -q+radius);

        for (int r = r_start; r < r_stop; ++r){
            HexCell *cell = new HexCell(q, r);
            cells[{q, r}] = cell;
        }
    }
}

void HexBoard::configure_cells() {
    
    for (auto kv : cells) {
        vector<pair<int, int>> neigh = get_neigh_coords(kv.first);
        for (int i=0; i < 6; i++) {
            if (cells.find(neigh[i]) != cells.end()) {
                kv.second->neighbours[i] = cells[neigh[i]];
            }
        }               
    }
}

void HexBoard::generate_maze(pair<int, int> start_coord, double branch_prob) {
    this->start_coord = start_coord;
    int longest_edge_path = 0;

    Stack<HexCell*> stack;
    map<pair<int, int>, bool> visited;

    stack.push(cells.find(start_coord)->second);
    visited[start_coord] = true;

    cells.find(start_coord)->second->path_dist_from_start = 0;

    while (!stack.isEmpty()) {

        HexCell* curr_cell = stack.peek();
        HexCell* neigh_cell = chooseNeighbour(curr_cell, visited, branch_prob);

        if (neigh_cell != nullptr) {

            curr_cell->link(neigh_cell);
            visited[{neigh_cell->q, neigh_cell->r}] = true;

            stack.push(neigh_cell);

            int sum = curr_cell->path_dist_from_start + neigh_cell->distance(curr_cell);
            neigh_cell->path_dist_from_start = sum;

            if (sum > longest_path_dist) {
                longest_path_dist = sum;
            }
            if (neigh_cell->is_edge() && sum > longest_edge_path) {
                    longest_edge_path = sum;
                    end_coord = {neigh_cell->q, neigh_cell->r}; 
            }

        } else {
            stack.pop();
        }
    }
}

void HexBoard::solve_maze() {
    solve(start_coord);
}

void HexBoard::solve(pair<int, int> start) {
    solution.push_back(start);
    for (auto cell : cells.find(start)->second->links) {

        if (cell != nullptr) {
            pair<int, int> check = {cell->q, cell->r};
            //pair<int, int> sol = solution[solution.size() - 2];
            if (!(solution.size() > 1 && solution[solution.size() - 2] == check)) {
                solve({cell->q, cell->r});
                if (solution.back() == end_coord) {
                    break;
                }
            }
        }
    }
    if (solution.back() != end_coord) {
        solution.pop_back();
    }
}


