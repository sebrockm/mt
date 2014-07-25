
#include <random>
#include <fstream>
#include <iostream>
#include <vector>


using namespace std;


void print_usage(const char* prog)
{
    cerr << 
        "usage: " << prog << "-m <machines> -n <jobs> -tmax <tmax> [options]" << endl <<
        "options:" << endl <<
        "-tmin <tmin>" << endl <<
        "-o <file>" << endl <<
        "-dom <x>" << endl <<
        "-groups <groups>" << endl <<
        "--distr=[normal,uniform(mu,sigma)]" << endl <<
        "--groupsizedistr=[normal,uniform(mu,sigma)]" << endl;

    exit(EXIT_FAILURE);
}

struct opt_set
{
    int m;
    int n;
    int tmax;
    int tmin;
    ostream& output;
    vector<int> dom;
    int groups;
    bool is_distr_norm;
    normal_distribution<double> distr_norm;
    uniform_int_distribution<int> distr_uni;
    bool is_groupsizedistr_norm;
    normal_distribution<double> groupsizedistr_norm;
    uniform_int_distribution<int> groupsizedistr_uni;

    opt_set(int argc, char** argv)
        :m(-1),
        n(-1),
        tmax(-1),
        tmin(0),
        output(cout),
        dom(),
        groups(-1)
    {
        for(int i = 1; i < argc; i++)
        {
            if(string(argv[i]) == "-m")
            {
                i++;
                if(i == argc)
                    print_usage(argv[0]);
                m = stoi(argv[i]);
                if(m <= 0)
                    print_usage(argv[0]);
            }
            else if(string(argv[i]) == "-n")
            {
                i++;
                if(i == argc)
                    print_usage(argv[0]);
                n = stoi(argv[i]);
                if(n <= 0)
                    print_usage(argv[0]);
            }
            else if(string(argv[i]) == "-tmax")
            {
                i++;
                if(i == argc)
                    print_usage(argv[0]);
                tmax = stoi(argv[i]);
                if(tmax < tmin)
                    print_usage(argv[0]);
            }
            else if(string(argv[i]) == "-tmin")
            {
                i++;
                if(i == argc)
                    print_usage(argv[0]);
                tmin = stoi(argv[i]);
                if(tmax < tmin)
                    print_usage(argv[0]);
            }
            else if(string(argv[i]) == "-o")
            {
                i++;
                if(i == argc)
                    print_usage(argv[0]);
                static ofstream file(argv[i]);
                if(!file)
                    print_usage(argv[0]);
                output = file;
            } 
            else if(string(argv[i]) == "-dom")
            {
                i++;
                if(i == argc)
                    print_usage(argv[0]);
                int d = stoi(argv[i]);
                if(d < 0 || d >= m)
                    print_usage(argv[0]);
                dom.push_back(d);
            }
            else if(string(argv[i]) == "-groups")
            {
                i++;
                if(i == argc)
                    print_usage(argv[0]);
                groups = stoi(argv[i]);
                if(groups <= 0)
                    print_usage(argv[0]);
            } 
        }
    }
};

int main(int argc, char** argv)
{
    opt_set set(argc, argv);
    
}
