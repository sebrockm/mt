#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <set>

using namespace std;



struct Veccomp
{
    bool operator () (const vector<int>& v1, const vector<int>& v2)
    {
        if(v1.size() == v2.size())
        {
            auto v1i = v1.begin();
            auto v2i = v2.begin();
            while(v1i != v1.end() && *v1i == *v2i)
            {
                ++v1i;
                ++v2i;
            }
            return v1i != v1.end() && *v1i < *v2i;
        }

        return v1.size() > v2.size();
    }
};


set<vector<int>, Veccomp> part(int n)
{
    if(n == 1)
    {
        return {{1}};
    }

    auto parts1 = part(n-1);
    set<vector<int>, Veccomp> parts2;
    
    for(auto it = parts1.begin(); it != parts1.end(); ++it)
    {
        auto v = *it;
        v.push_back(1);
        parts2.insert(v);
    }

    for(auto v : parts1)
    {
        for(unsigned i = 0; i < v.size();)
        {
            auto cv = v;
            cv[i]++;
            parts2.insert(cv);
            auto last = v[i];
            while(i < v.size() && last == v[i])
                i++;
        }
    }

    return parts2;
}


int main(int argc, char** argv)
{
    if(argc != 2)
    {
        cerr << "usage: " << argv[0] << " n" << endl;
        exit(EXIT_FAILURE);
    }

    int n;
    stringstream(argv[1]) >> n;

    auto parts = part(n);
    for(auto& line : parts)
    {
        for(unsigned i = 0; i < line.size() - 1; i++)
        {
            cout << line[i] << " ";
        }
        cout << line.back() << endl;
    }

    return 0;
}
