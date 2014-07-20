#include "nonfull_schedule.hpp"
#include "de_nonfull_schedule.hpp"

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost::algorithm;


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
    while(getline(file, line))
    {
        //cout << "read " << line << endl;
        trim(line);
        if(line.empty() || line.front() == '#')
            continue;

        if(!read_mn)
        {
            stringstream(line) >> m >> n;
            jobs.reserve(n);
            read_mn = true;
            //cout << "m=" << m << " n=" << n << endl;
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
    }

    //cout << jobs.size() << endl;

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

    nonfull_schedule<int> sch(m);
    sch.jobs = jobs;

    //cout << sch;
    cout << "Cmax: " << sch.get_cost() << endl;

    sch.jobs = create_schedule<int>(m, jobs).jobs;

    //cout << sch;
    cout << "Cmax: " << sch.get_cost() << endl;
    
    jobs = sch.jobs;

    auto de_sch = create_de_schedule(m, jobs);

    //cout << de_sch;
    cout << "Cmax: " << de_sch.get_cost() << endl;

}
