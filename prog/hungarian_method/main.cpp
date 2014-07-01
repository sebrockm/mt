#include "dijkstra_improvable.hpp"
#include <iostream>

using namespace std;

int main()
{
    vector<int> a = {1,2,3};
    vector<int> b = {4,5,6};
    dijkstra_improvable dijk(b);

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

    for(int i = a.size()-1; i >= 0; i--)
    {
        int n = a[i];
        auto w = dijk.add(n, move(d[n]));
        for(auto p : w)
        {
            cout << n << "->" << p.first << ": " << p.second << endl;
        }
    }

}
