// Project Identifier: 9B734EC0C043C5A836EA0EBE4BEFEA164490B2C7

#include <algorithm>
#include <getopt.h>
#include <limits>
#include <math.h>
#include <cstdlib>
#include <iomanip>

#include "amgus.h"

void AmongUs::get_mode(int argc, char* argv[])
{
    int choice;
    int option_index = 0;
    option long_options[] = 
    {
        { "mode", required_argument, nullptr, 'm' },
        { "help",  no_argument, nullptr, 'h' },
        { nullptr, 0, nullptr, '\0' }
    };

    while ((choice = getopt_long(argc, argv, ":m:h", long_options, &option_index)) != -1)
    {
        switch (choice)
        {
            case 'm':
                mode = optarg;
                break;

            case 'h':
                throw NeedHelp();
                break;

            case '?':
                throw InvalidCommand();
                break;

            case ':':
                throw NoMode();
                break;
        } 
    }
    
    if (mode != "MST" && mode != "FASTTSP" && mode != "OPTTSP")
        throw InvalidMode();
}

void AmongUs::read_graph()
{
    size_t N;
    cin >> N;
    vertices.resize(N);
    area.resize(N);
    cur_path.reserve(N);

    for (size_t i=0; i<N; i++)
    {
        int x,y;
        cin >> x >> y;

        vertices[i].x = x;
        vertices[i].y = y;

        if (x > 0 || y > 0)
            area[i] = 'O';
        else if (x < 0 && y < 0)
            area[i] = 'L';
        else
            area[i] = 'D';
    }
}

bool AmongUs::adjacent(size_t v, size_t w)
{
    char area_v = area[v];
    char area_w = area[w];
    return (area_v == area_w || area_v == 'D' || area_w == 'D'); 
}

double AmongUs::get_dist_square(size_t v, size_t w)
{
    double x1 = vertices[v].x;
    double x2 = vertices[w].x;
    double y1 = vertices[v].y;
    double y2 = vertices[w].y;
    return (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
}

bool AmongUs::MST()
{
    size_t N = vertices.size();
    vector<int> visited(N, 0);
    vector<double> distance_square(N, numeric_limits<double>::infinity());
    distance_square[0] = 0.;
    vector<size_t> predecessor(N);

    double weight = 0.;
    while (true)
    {
        double min_dist_square = numeric_limits<double>::infinity();
        bool flag = true;
        size_t v = 0;
        for (size_t i=0; i<N; i++)
        {
            if (!visited[i] && distance_square[i] < min_dist_square)
            {
                v = i;
                min_dist_square = distance_square[i];
                flag = false;
            }
        }

        if (flag)
            break;

        weight += sqrt(min_dist_square);
        visited[v] = 1;

        for (size_t w=0; w<N; w++)
        {
            if (!visited[w] && adjacent(v, w))
            {
                double tmp = get_dist_square(v, w);
                if (tmp < distance_square[w])
                {
                    distance_square[w] = tmp;
                    predecessor[w] = v;
                }
            }
        }
    }

    for (size_t i=0; i<N; i++)
    {
        if (!visited[i])
            return false;
    }

    cout << weight << "\n";
    for (size_t i=1; i<N; i++)
    {
        size_t p = predecessor[i];
        if (i < p)
            cout << i << " " << p << "\n";
        else
            cout << p << " " << i << "\n";
    }

    return true;
}

size_t AmongUs::findNearestNeighbor(vector<int> & visited, size_t v)
{
    size_t N = vertices.size();

    double min_dist_square = numeric_limits<double>::infinity();
    size_t neighbor = 0;
    for (size_t w=0; w<N; w++)
    {
        if (!visited[w])
        {
            double tmp = get_dist_square(v, w);
            if (tmp < min_dist_square)
            {
                min_dist_square = tmp;
                neighbor = w;
            }
        }
    }
    return neighbor;
}

double AmongUs::get_cost(size_t v, size_t w, size_t new_vertex)
{
    double d1 = sqrt(get_dist_square(v, new_vertex));
    double d2 = sqrt(get_dist_square(w, new_vertex));
    double d3 = sqrt(get_dist_square(v, w));
    return (d1+d2-d3);
}

int AmongUs::findMinCostPos(size_t new_vertex, int path_size)
{
    int pos = path_size;
    double min_cost = get_cost(0, cur_path.back(), new_vertex);
    for (int i=0; i<path_size-1; i++)
    {
        size_t tmp = size_t(i);
        double cost = get_cost(cur_path[tmp], cur_path[tmp+1], new_vertex);
        if (cost < min_cost)
        {
            pos = i+1;
            min_cost = cost;
        }
    }
    cur_weight += min_cost;
    return pos;
}

void AmongUs::FASTTSP_AI()
{
    cur_weight = 0.;

    cur_path.push_back(0); // start from 0
    
    cur_weight += 2*sqrt(get_dist_square(0, 1));
    cur_path.push_back(1);
    int path_size = 2;

    for (size_t j=2; j<vertices.size(); j++)
    {   
        int pos = findMinCostPos(j, path_size);
        
        cur_path.insert(cur_path.begin()+pos, j);
        path_size++;
    }

    if (mode == "FASTTSP")
    {
        cout << cur_weight << "\n";
        for (size_t i=0; i<cur_path.size(); i++)
            cout << cur_path[i] << " ";
        cout << "\n";
    }
}

void AmongUs::OPTTSP()
{
    FASTTSP_AI();
    opt_weight = cur_weight;
    opt_path = cur_path;

    cur_weight = 0.;
  
    genPerms(1);

    cout << opt_weight << "\n";
    for (size_t i=0; i<opt_path.size(); i++)
        cout << opt_path[i] << " ";
    cout << "\n";
}

void AmongUs::solve()
{
    if (mode == "MST")
    {
        if (!MST())
            throw NoMST();
    }
    else if (mode == "FASTTSP")
    {
        FASTTSP_AI();
    }
    else
        OPTTSP();
}

bool AmongUs::promising(size_t permLength)
{
    double estimation = cur_weight;
    if (estimation >= opt_weight)
        return false;
    
    size_t N = vertices.size() - permLength;
    vector<int> visited(N);
    vector<double> distance_square(N, numeric_limits<double>::infinity());
    distance_square[0] = 0.;
    vector<size_t> predecessor(N);
    double mstCost = 0.;

    while (true)
    {
        double min_dist_square = numeric_limits<double>::infinity();
        bool flag = true;
        size_t v = 0;
        for (size_t i=0; i<N; i++)
        {
            if (!visited[i] && distance_square[i] < min_dist_square)
            {
                v = i;
                min_dist_square = distance_square[i];
                flag = false;
            }
        }

        if (flag)
            break;

        mstCost += sqrt(min_dist_square);
        if (estimation + mstCost >= opt_weight)
            return false;
        visited[v] = 1;

        for (size_t w=0; w<N; w++)
        {
            if (!visited[w])
            {
                double tmp = get_dist_square(cur_path[v+permLength], cur_path[w+permLength]);
                if (tmp < distance_square[w])
                {
                    distance_square[w] = tmp;
                    predecessor[w] = v;
                }
            }
        }
    }
    estimation += mstCost;
    if (estimation >= opt_weight)
        return false;

    double arm1Len = numeric_limits<double>::infinity();
    for (size_t w=0; w<N; w++)
    {
        double tmp = get_dist_square(cur_path[0], cur_path[w+permLength]);
        if (tmp < arm1Len)
            arm1Len = tmp;
    }
    estimation += sqrt(arm1Len);
    if (estimation >= opt_weight)
        return false;

    double arm2Len = numeric_limits<double>::infinity();
    for (size_t w=0; w<N; w++)
    {
        double tmp = get_dist_square(cur_path[permLength-1], cur_path[w+permLength]);
        if (tmp < arm2Len)
            arm2Len = tmp;
    }
    estimation += sqrt(arm2Len);
    if (estimation >= opt_weight)
        return false; 

    /*for (size_t i = 0; i < cur_path.size(); ++i)
        cerr << setw(2) << cur_path[i] << ' ';
    cerr << setw(4) << permLength << setw(10) << cur_weight;
    cerr << setw(10) << arm1Len << setw(10) << arm2Len;
    cerr << setw(10) << mstCost << setw(10) << estimation << "  "  << '\n';*/

    return true; 
}

void AmongUs::genPerms(size_t permLength)//, double cur_weight)
{
    if (permLength == cur_path.size())
    {
        // Do something with the path
        double weight = cur_weight + sqrt(get_dist_square(cur_path[0], cur_path.back()));
        if (weight < opt_weight)
        {
            opt_weight = weight;
            opt_path = cur_path;
        }
        return;
    }  // if ..complete path

    if (!promising(permLength))
        return;

    for (size_t i = permLength; i < cur_path.size(); ++i)
    {
        swap(cur_path[permLength], cur_path[i]);
        double new_length = sqrt(get_dist_square(cur_path[permLength], cur_path[permLength-1]));
        cur_weight += new_length;
        genPerms(permLength + 1);
        cur_weight -= new_length;
        swap(cur_path[permLength], cur_path[i]);
    }  // for ..unpermuted elements
}
