
#include <random>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>


using namespace std;


void print_usage(const char* prog)
{
    cerr << 
        "usage: " << prog << " -m <machines> -n <jobs> -tmax <tmax> [options]" << endl <<
        "options:" << endl <<
        "-tmin <tmin>" << endl <<
        "-o <file>" << endl <<
        "-dom <x>" << endl <<
        "-groups <groups>" << endl <<
        "-gmin <gmin>" << endl <<
        "-gmax <gmax>" << endl <<
        "--distr=normal mu sigma" << endl <<
        "--groupsizedistr=normal mu sigma" << endl;

    exit(EXIT_FAILURE);
}

struct opt_set
{
    int m;
    int n;
    int tmax;
    int tmin;
    vector<int> dom;
    int groups;
    int gmin;
    int gmax;

private:
    bool is_file_output;
    ofstream file;
    default_random_engine generator;
    bool is_distr_norm;
    normal_distribution<double> distr_norm;
    uniform_int_distribution<int> distr_uni;
    bool is_groupsizedistr_norm;
    normal_distribution<double> groupsizedistr_norm;
    uniform_int_distribution<int> groupsizedistr_uni;

public:
    ostream& output()
    {
        if(is_file_output)
            return file;
        cout << "cout" << endl;
        return cout;
    }

    int distr()
    {
        if(is_distr_norm)
            return max(tmin, min(tmax, (int)distr_norm(generator)));
        return distr_uni(generator);
    }

    int distrundom()
    {
        return max(0, distr()*tmin/tmax);
    }

    int groupsizedistr()
    {
        if(is_groupsizedistr_norm)
            return max(gmin, min(gmax, (int)groupsizedistr_norm(generator)));
        return groupsizedistr_uni(generator);
    }

    opt_set(int argc, char** argv)
        :m(-1),
        n(-1),
        tmax(-1),
        tmin(0),
        dom(),
        groups(-1),
        gmin(1),
        gmax(1),
        is_file_output(false),
        generator(),
        is_distr_norm(false),
        is_groupsizedistr_norm(false)
    {
        for(int i = 1; i < argc; i++)
        {
            try
            {
                string argvi = argv[i];
                if(argvi == "-m")
                {
                    i++;
                    if(i == argc)
                        print_usage(argv[0]);
                    m = stoi(argv[i]);
                    if(m <= 0)
                        print_usage(argv[0]);
                }
                else if(argvi == "-n")
                {
                    i++;
                    if(i == argc)
                        print_usage(argv[0]);
                    n = stoi(argv[i]);
                    if(n <= 0)
                        print_usage(argv[0]);
                }
                else if(argvi == "-tmax")
                {
                    i++;
                    if(i == argc)
                        print_usage(argv[0]);
                    tmax = stoi(argv[i]);
                    if(tmax < tmin)
                        print_usage(argv[0]);
                }
                else if(argvi == "-tmin")
                {
                    i++;
                    if(i == argc)
                        print_usage(argv[0]);
                    tmin = stoi(argv[i]);
                    if(tmax < tmin)
                        print_usage(argv[0]);
                }
                else if(argvi == "-o")
                {
                    i++;
                    if(i == argc)
                        print_usage(argv[0]);
                    file.open(argv[i]);
                    if(!file)
                        print_usage(argv[0]);
                    is_file_output = true;
                } 
                else if(argvi == "-dom")
                {
                    i++;
                    if(i == argc)
                        print_usage(argv[0]);
                    int d = stoi(argv[i]);
                    if(d < 0 || d >= m)
                        print_usage(argv[0]);
                    dom.push_back(d);
                }
                else if(argvi == "-groups")
                {
                    i++;
                    if(i == argc)
                        print_usage(argv[0]);
                    groups = stoi(argv[i]);
                    if(groups <= 0)
                        print_usage(argv[0]);
                } 
                else if(argvi == "-gmin")
                {
                    i++;
                    if(i == argc)
                        print_usage(argv[0]);
                    gmin = stoi(argv[i]);
                    if(gmin <= 0)
                        print_usage(argv[0]);
                }
                else if(argvi == "-gmax")
                {
                    i++;
                    if(i == argc)
                        print_usage(argv[0]);
                    gmax = stoi(argv[i]);
                    if(gmax < gmin)
                        print_usage(argv[0]);
                }
                else if(argvi == "--distr=normal")
                {
                    i += 2;
                    if(i >= argc)
                        print_usage(argv[0]);

                    double mu = stod(argv[i-1]);
                    double sigma = stod(argv[i]);

                    is_distr_norm = true;
                    distr_norm = normal_distribution<double>(mu, sigma);
                }
                else if(argvi.substr(0, 24) == "--groupsizedistr=normal")
                {
                    i += 2;
                    if(i >= argc)
                        print_usage(argv[0]);

                    double mu = stod(argv[i-1]);
                    double sigma = stod(argv[i]);

                    is_groupsizedistr_norm = true;
                    groupsizedistr_norm = normal_distribution<double>(mu, sigma);
                }
                else
                {
                    print_usage(argv[0]);
                }
            }
            catch(...)
            {
                print_usage(argv[0]);
            }
        }

        if(m < 0 || n < 0 || tmax < tmin || gmax < gmin)
            print_usage(argv[0]);

        if(dom.empty())
        {
            for(int j = 0; j < m; ++j)
                dom.push_back(j);
        }
        else
        {
            dom.resize(distance(dom.begin(), unique(dom.begin(), dom.end())));
            sort(dom.end(), dom.end());
        }

        if(groups < 0)
            groups = n;
        gmax = n / groups;
        groupsizedistr_uni = uniform_int_distribution<int>(gmin, gmax);

        distr_uni = uniform_int_distribution<int>(tmin, tmax);
    }
};

int main(int argc, char** argv)
{
    opt_set set(argc, argv);

    ostream& out = set.output();

    out << set.m << " " << set.n << endl;

    vector<int> groupsizes;
    groupsizes.reserve(set.n);
    int sum = 0;
    while(sum < set.n)
    {
        groupsizes.push_back(set.groupsizedistr());
        sum += groupsizes.back();
        if(sum > set.n)
            groupsizes.back() -= sum - set.n;
    }

    for(int group : groupsizes)
    {
        vector<int> times(set.m);
        for(int j = 0; j < set.m; ++j)
        {
            if(find(set.dom.begin(), set.dom.end(), j) == set.dom.end()) //j is non dom
            {
                times[j] = set.distrundom();
            }
            else //j is dom
            {
                times[j] = set.distr();
            }
        }

        for(int i = 0; i < group; ++i)
        {
            for(int t : times)
            {
                out << t << " ";
            }
            out << endl;
        }
    }
}
