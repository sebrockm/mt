#include "nonfull_schedule.hpp"

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;


typedef typename nonfull_schedule<int>::job job;

vector<job> read_jobs(const char* file_name)
{
    ifstream file(file_name);
    if(!file)
        return {};

    unsigned m, n;
    bool read_mn = false;

    vector<job> jobs;

    string line;
    getline(file, line);
    while(file)
    {
        if(line.front() == '#')
            continue;

        if(!read_mn)
        {
            stringstream(line) >> m >> n;
            jobs.reserve(n);
            read_mn = true;
        }
        else
        {
            job j(m);
            stringstream ss(line);
            for(unsigned i = 0; i < m; ++i)
            {
                ss >> j[i];
            }
            jobs.push_back(move(j));
            
            if(jobs.size() == n)
                break;
        }        

        getline(file, line);
    }

    cout << jobs.size() << endl;

    return jobs.size() == n ? jobs : vector<job>();
}


int main(int argc, char** argv)
{
    /*constexpr int m = 3;

    vector<job> jobs = 
    {
        {3,5,1},
        {5,6,8},
        {4,9,1},
        {4,3,3},
        {6,8,1},
        {6,5,9},
        {3,5,1},
        {1,5,8}
    };
    */

    if(argc != 2)
    {
        cerr << "usage: " << argv[0] << " file" << endl;
        return 1;
    }

    auto jobs = read_jobs(argv[1]);
    int m = jobs.front().size();

    for(int i = 0; i < m; ++i)
    {
        for(int j = 0; j < i; ++j)
        {
            cout << " ";
        }
        for(auto& j : jobs)
        {
            cout << j[i];
        }
        cout << endl;
    }
    cout << endl;

    auto sch = create_schedule<int>(m, jobs);

    cout << sch << endl;

    cout << "Cmax: " << sch.get_cost() << endl;
    
}
