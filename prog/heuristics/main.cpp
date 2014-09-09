#include "nonfull_schedule.hpp"
#include "de_nonfull_schedule.hpp"
#include "gg_heuristik.hpp"
#include "simulated_annealing.hpp"
#include "soylu.hpp"

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <unistd.h>

#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost::algorithm;


typedef typename nonfull_schedule<int>::job job;

//read jobs from file
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
        trim(line);
        if(line.empty() || line.front() == '#')
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
    }

    return jobs.size() == n ? jobs : vector<job>();
}


//solve with cplex on terminal
//returns the lower and upper bound
pair<double, double> do_cplex(const char* file_name)
{
    //invoke zimpl
    stringstream ss;
    ss << "zimpl ../synmvCmax.zpl -v0 -D file=" << file_name;    
    int tmp = system(ss.str().c_str());

    //invoce cplex and write its output to .tmp-file
    ss.str("");
    ss << "cplex -c \"re synmvCmax.lp\" \"set timelimit 1800\" \"o\" \"dis sol obj\" \"dis sol bestb\" > " << file_name << ".tmp";
    tmp += system(ss.str().c_str());

    //read that .tmp-file
    ss.str("");
    ss << file_name << ".tmp";
    ifstream output(ss.str());

    //read last 4 lines
    string line[4];
    while(getline(output, line[3]))
    {
        line[0] = line[1];
        line[1] = line[2];
        line[2] = line[3];
    }

    //delete .tmp-file
    output.close();
    tmp += system(("rm " + ss.str()).c_str());

    if(tmp > 0) //at least one of the system()-calls failed
        exit(tmp);

    //search for one of these strings in the last 4 lines
    string optimal = "CPLEX> MIP - Integer optimal solution:  Objective =";
    string tolerance = "CPLEX> MIP - Integer optimal, tolerance (0.0001/1e-06):  Objective =";
    string aborted = "CPLEX> MIP - Aborted, integer feasible:  Objective =";
    string exceeded= "CPLEX> MIP - Time limit exceeded, integer feasible:  Objective =";
    string bound = "CPLEX> Current MIP best bound =";
    pair<double, double> res(0,0);

    int gappos = line[1].find("(gap");
    
    //read the lower bound
    if(line[1].find(bound) == 0)
    {
        res.first = stod(line[1].substr(bound.size(), gappos - bound.size()));
    }
    else
    {
        cerr << "second last line: " << line[1] << endl;
    }

    //read the upper bound/best solution
    if(line[0].find(optimal) == 0)
    {
        res.second = stod(line[0].substr(optimal.size()));
    }
    else if(line[0].find(aborted) == 0)
    {
        res.second = stod(line[0].substr(aborted.size()));
    }
    else if(line[0].find(exceeded) == 0)
    {
        res.second = stod(line[0].substr(exceeded.size()));
    }
    else if(line[0].find(tolerance) == 0)
    {
        res.second = stod(line[0].substr(tolerance.size()));
    }
    else
    {
        cerr << "last line: " << line[0] << endl;
    }

    return res;
}


//read file and calculate Cmax with all heuristics
//output info on stderr
//output measured time and Cmax on stdout
int main(int argc, char** argv)
{
    /*
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

    ///*
    if(argc != 2)
    {
        cerr << "usage: " << argv[0] << " file" << endl;
        return 1;
    }
    //*/
    
    cerr << "start reading " << argv[1] << endl;
    auto jobs = read_jobs(argv[1]);
    int m = jobs.front().size();
    cerr << "read " << argv[1] << endl;


    cout << jobs.size() << "\t\t";

    //cplex only for small instances
    auto t1 = chrono::high_resolution_clock::now();
    auto p = jobs.size() <= 1500 ? do_cplex(argv[1]) : make_pair(0., 0.);
    auto t2 = chrono::high_resolution_clock::now();

    cout <<  p.first << "\t" << p.second << "\t" << (chrono::duration_cast<chrono::duration<double>>(t2-t1)).count() << "\t\t";
    if(jobs.size() <= 1500)
        cerr << "done cplex" << endl;

    nonfull_schedule<int> sch(m);
    sch.jobs = jobs;

    //cout << sch;
    //cout << "initial Cmax: " << sch.get_cost() << endl;

    //SA
    t1 = chrono::high_resolution_clock::now();
    sch.jobs = simulated_annealing(sch.jobs, sch.get_cost());
    t2 = chrono::high_resolution_clock::now();
    
    cout << sch.get_cost() << "\t" << (chrono::duration_cast<chrono::duration<double>>(t2-t1)).count() << "\t\t";
    cerr << "done SA" << endl;


    //nfs
    t1 = chrono::high_resolution_clock::now();
    sch.jobs = create_schedule<int>(m, jobs).jobs;
    t2 = chrono::high_resolution_clock::now();

    cout << sch.get_cost() << "\t" << (chrono::duration_cast<chrono::duration<double>>(t2-t1)).count() << "\t\t";
    cerr << "done nfs" << endl;


    jobs = sch.jobs;

    //denfs
    t1 = chrono::high_resolution_clock::now();
    auto de_sch = create_de_schedule(m, jobs);
    t2 = chrono::high_resolution_clock::now();

    cout << de_sch.get_cost() << "\t" << (chrono::duration_cast<chrono::duration<double>>(t2-t1)).count() << "\t\t";
    cerr << "done denfs" << endl;


    //gg with 3,4 dom machines
    t1 = chrono::high_resolution_clock::now();
    sch.jobs = gg_heuristik(sch.jobs, 3, 4);
    t2 = chrono::high_resolution_clock::now();

    cout << sch.get_cost() << "\t" << (chrono::duration_cast<chrono::duration<double>>(t2-t1)).count() << "\t\t";
    cerr << "done gg" << endl;


    //soylu
    t1 = chrono::high_resolution_clock::now();
    int cost = h1(sch.jobs);
    t2 = chrono::high_resolution_clock::now();

    cout << cost << "\t" << (chrono::duration_cast<chrono::duration<double>>(t2-t1)).count() << "\t\t";
    cerr << "done soylu H1" << endl;


    t1 = chrono::high_resolution_clock::now();
    cost = h2(sch.jobs);
    t2 = chrono::high_resolution_clock::now();

    cout << cost << "\t" << (chrono::duration_cast<chrono::duration<double>>(t2-t1)).count() << "\t\t";
    cerr << "done soylu H2" << endl;


    t1 = chrono::high_resolution_clock::now();
    cost = h3(sch.jobs);
    t2 = chrono::high_resolution_clock::now();

    cout << cost << "\t" << (chrono::duration_cast<chrono::duration<double>>(t2-t1)).count() << "\t\t";
    cerr << "done soylu H3" << endl;


    cout << endl;
}
