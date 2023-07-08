void HexBoard::generate_maze(pair<int, int> start_coord, double branch_prob) {
    this->start_coord = start_coord;
    map<pair<int, int>, bool> visited;
    Stack<HexCell*> stack;

    stack.push(cells.find(start_coord)->second);

    visited[start_coord] = true;


    cells.find(start_coord)->second->path_dist_from_start = 0;
    int longest_edge_path = 0;

    while (!stack.isEmpty()) {
        HexCell* curr_cell = stack.peek();
        HexCell* neigh_cell = chooseNeighbour(curr_cell, visited, branch_prob);
        if (neigh_cell != nullptr) {
            curr_cell->link(neigh_cell);
            visited[{neigh_cell->q, neigh_cell->r}] = true;
            stack.push(neigh_cell);

            int cur_diff = curr_cell->path_dist_from_start + neigh_cell->distance(curr_cell);
            neigh_cell->path_dist_from_start = cur_diff;
            if (cur_diff > longest_path_dist) {
                longest_path_dist = cur_diff;
            }
            if (neigh_cell->is_edge() && cur_diff > longest_edge_path) {
                longest_edge_path = cur_diff;
                end_coord = {neigh_cell->q, neigh_cell->r};
            }
        } else {
            stack.pop();
        }
    }
}