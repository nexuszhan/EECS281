// Project Identifier: 9B734EC0C043C5A836EA0EBE4BEFEA164490B2C7

#ifndef AMGUS_H
#define AMGUS_H

#include <iostream>
#include <string>
#include <vector>

#include "errorcheck.h"

using namespace std;

struct vertex
{
    int x;
    int y;
};

class AmongUs
{
    private:
        string mode;
        vector<vertex> vertices;
        vector<char> area;

        vector<size_t> cur_path;
        vector<size_t> opt_path;
        double opt_weight;
        double cur_weight;

    public:
        void get_mode(int argc, char* argv[]); // Read the command line

        void read_graph(); // Read vertices and generate adjacent matrix 

        bool adjacent(size_t v, size_t w);
        double get_dist_square(size_t v, size_t w);
        bool MST(); // Use Prim's algorithm

        size_t findNearestNeighbor(vector<int> & visited, size_t v);
        double get_cost(size_t v, size_t w, size_t new_vertex);
        int findMinCostPos(size_t new_vertex, int path_size);
        void FASTTSP_AI(); // arbitrary insertion

        bool promising(size_t permLength);
        void genPerms(size_t permLength);
        void OPTTSP();

        void solve();
};

#endif