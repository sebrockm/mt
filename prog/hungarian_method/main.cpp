#include "dijkstra_improvable.hpp"
#include "augmenting_path.hpp"
#include <iostream>

using namespace std;

int main()
{
    vector<int> a = {1,2,3};
    vector<int> b = {4,5,6};
    dijkstra_improvable dijk(b.begin(), b.end());

    unordered_map<int, unordered_map<int, float>> d;
    d[1][4] = 15;
    d[1][5] = 19;
    d[1][6] = 7;
    d[2][4] = 21;
    d[2][5] = 10;
    d[2][6] = 1;
    d[3][4] = 6;
    d[3][5] = 1;
    d[3][6] = 1;

    /*for(int i = a.size()-1; i >= 0; i--)
    {
        int n = a[i];
        auto w = dijk.add(n, move(d[n]), matching())
        for(auto p : w)
        {
            cout << n << "->" << p.first << ": " << p.second << endl;
        }
    }*/

    matching m;
    for(int i : a)
    {
        for(int j : b)
        {
            cout << "================" << i << "," << j << "===============" << endl;
            auto path = find_augmenting_path(d, m, i, j);
            for(auto p : path)
                cout << p << ",";
            cout << endl << endl;

            m.add_path(path.begin(), path.end());

            for(auto& p : m.vertices)
            {
                cout << p.first << ": " << p.second << endl;
            }

            for(auto& p : m.edges)
            {
                cout << p.first.first << "," << p.first.second << ": " << p.second << endl;
            }
        }
    }
}
