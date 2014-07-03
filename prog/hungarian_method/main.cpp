#include "dijkstra_improvable.hpp"
#include "augmenting_path.hpp"
#include "matching.hpp"

#include <iostream>
#include <algorithm>

using namespace std;


matching hungarian_method(const vector<int>& nodes_a, const vector<int>& nodes_b, const unordered_map<int, unordered_map<int, float>>& adjacency_list)
{
    matching match(nodes_a.size());
    unordered_map<int, float> potential(3*nodes_a.size());

    for(int n : nodes_b)
    {
        potential[n] = 0;
    }

    dijkstra_improvable dijk(nodes_b.begin(), nodes_b.end());

    //main loop
    for(auto u = nodes_a.begin(); u != nodes_a.end(); ++u)
    {
        potential[*u] = 0;
        for(auto& p : adjacency_list.at(*u))
        {
            potential[*u] = max(potential[*u], p.second - potential[p.first]);
        }

        int z;
        auto paths = dijk.add(*u, adjacency_list.at(*u), match, potential, z);
        float delta = paths[z];

        for(auto it = nodes_a.begin(); it != u+1; ++it)
        {
            potential[*it] -= max(0.f, delta - paths[*it]);
        }
        for(int n : nodes_b)
        {
            potential[n] += max(0.f, delta - paths[n]);
        }

        auto p = find_augmenting_path(adjacency_list, match, *u, z);
        match.add_path(p.begin(), p.end());
    }

    return match;
}

int main()
{
    vector<int> a = {1,2,3};
    vector<int> b = {4,5,6};

    unordered_map<int, unordered_map<int, float>> d;
    d[1][4] = d[4][1] = 15;
    d[1][5] = d[5][1] = 19;
    d[1][6] = d[6][1] = 7;
    d[2][4] = d[4][2] = 21;
    d[2][5] = d[5][2] = 10;
    d[2][6] = d[6][2] = 1;
    d[3][4] = d[4][3] = 6;
    d[3][5] = d[5][3] = 1;
    d[3][6] = d[6][3] = 1;

    auto m = hungarian_method(a, b, d);

    float sum = 0;
    for(auto e : m.edges)
    {
        if(e.second)
        {
            sum += d[e.first.first][e.first.second];
            cout << e.first.first << "," << e.first.second << endl;
        }
    }
    cout << "sum=" << sum/2 << endl;

}
