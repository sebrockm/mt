#include "dijkstra_improvable.hpp"
#include "augmenting_path.hpp"
#include "matching.hpp"

#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>

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
        //auto paths = dijk.add(*u, adjacency_list, match, potential, z);
        float delta = paths[z];

        for(auto it = nodes_a.begin(); it != u+1; ++it)
        {
            potential[*it] -= max(0.f, delta - paths[*it]);
        }
        for(int n : nodes_b)
        {
            potential[n] += max(0.f, delta - paths[n]);
        }

        auto p = find_augmenting_path(adjacency_list, match, *u, z, potential);
        match.add_path(p.begin(), p.end());
    }

    return match;
}

bool read_file(const char* file, vector<int>& a, vector<int>& b, unordered_map<int, unordered_map<int, float>>& d)
{
    fstream stream(file);
    if(!stream)
        return false;
    
    string line;
    do
    {
        getline(stream, line);
    } 
    while(line[0] == '#' && stream);

    if(!stream)
        return false;

    int n;
    stringstream(line) >> n;

    a.resize(n);
    b.resize(n);
    int i = 1;
    generate(a.begin(), a.end(), [&i] () { return i++;});
    generate(b.begin(), b.end(), [&i] () { return i++;});

    i = 0;
    while(stream && i < n)
    {
        getline(stream, line);
        if(line[0] == '#')
            continue;

        stringstream ss(line);
        for(int j = 0; j < n; j++)
        {
            ss >> d[ a[i] ][ b[j] ];
            d[ b[j] ][ a[i] ] = d[ a[i] ][ b[j] ];
        }
        i++;
    }
    return i == n;
}

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        cerr << "usage: " << argv[0] << " file" << endl;
        return 1;
    }

    vector<int> a;
    vector<int> b;

    unordered_map<int, unordered_map<int, float>> d;
    /*d[1][4] = d[4][1] = 15;
    d[1][5] = d[5][1] = 19;
    d[1][6] = d[6][1] = 7;
    d[2][4] = d[4][2] = 21;
    d[2][5] = d[5][2] = 10;
    d[2][6] = d[6][2] = 1;
    d[3][4] = d[4][3] = 6;
    d[3][5] = d[5][3] = 1;
    d[3][6] = d[6][3] = 1;*/

    if(!read_file(argv[1], a, b, d))
        return 1;

    auto m = hungarian_method(a, b, d);

    float sum = 0;
    vector<pair<int, int>> lines;
    for(auto e : m.edges)
    {
        if(e.second)
        {
            sum += d[e.first.first][e.first.second];
            lines.push_back(e.first);
        }
    }
    sort(lines.begin(), lines.end(), [] (const pair<int, int>& a, const pair<int, int>& b) 
            {
                if(a.first == b.first)
                    return a.second < b.second;
                return a.first < b.first;
            }
    );
    for(unsigned i = 0; i < lines.size()/2; i++)
    {
        cout << "x[" << lines[i].first << "," << lines[i].second << "]" << endl;
    }
    cout << "sum=" << sum/2 << endl;

}
