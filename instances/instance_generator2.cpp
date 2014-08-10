
#include <random>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>


using namespace std;


void print_usage(const char* prog)
{
    cerr << "usage: " << prog << " -n <jobs> *[-m (u <min> <max>|n <mu> <sigma> [<min> <max>])] [-o <file>]" << endl;
    exit(EXIT_FAILURE);
}

default_random_engine generator = default_random_engine(time(0));

struct opt_set
{
    struct machine
    {
        bool is_normal;
        int tmin, tmax;
        normal_distribution<double> distr_norm;
        uniform_int_distribution<int> distr_uni;

        machine()
            :is_normal(false), tmin(0), tmax(numeric_limits<int>::max()), distr_norm(), distr_uni() {}

        int distr()
        {
            if(is_normal)
                return max(tmin, min(tmax, (int)distr_norm(generator)));
            return distr_uni(generator);
        }
    };

    vector<machine> machines;
    int n;

private:
    bool is_file_output;
    ofstream file;

public:
    ostream& output()
    {
        if(is_file_output)
            return file;
        return cout;
    }

    opt_set(int argc, char** argv)
        :n(-1),
        is_file_output(false)
    {
        for(int i = 1; i < argc; i++)
        {
            try
            {
                string argvi = argv[i];
                if(argvi == "-n")
                {
                    i++;
                    if(i == argc)
                        print_usage(argv[0]);
                    n = stoi(argv[i]);
                    if(n <= 0)
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
                else if(argvi == "-m")
                {
                    i++;
                    if(i == argc)
                        print_usage(argv[0]);
                    argvi = argv[i];
                    if(argvi != "n" && argvi != "u")
                        print_usage(argv[0]);


                    machines.push_back(machine());

                    if(argvi == "n")
                    {
                        i += 2;
                        if(i >= argc)
                            print_usage(argv[0]);
                        
                        double mu = stod(argv[i-1]);
                        double sigma = stod(argv[i]);
                        
                        machines.back().is_normal = true;
                        machines.back().distr_norm = normal_distribution<double>(mu, sigma); 

                        if(i+1 < argc && argv[i+1][0] != '-')
                        {
                            i += 2;
                            if(i >= argc)
                                print_usage(argv[0]);

                            machines.back().tmin = stoi(argv[i-1]);
                            machines.back().tmax = stoi(argv[i]);

                            if(machines.back().tmin < 0 || machines.back().tmax < machines.back().tmin)
                                print_usage(argv[0]);
                        }
                    }
                    else //argvi == "u"
                    {
                        i += 2;
                        if(i >= argc)
                            print_usage(argv[0]);

                        int min = stoi(argv[i-1]);
                        int max = stoi(argv[i]);

                        if(min < 0 || max < min)
                            print_usage(argv[0]);
                            
                        machines.back().is_normal = false;
                        machines.back().distr_uni = uniform_int_distribution<int>(min, max); 

                        machines.back().tmin = min;
                        machines.back().tmax = max;
                    }
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
    }
};

int main(int argc, char** argv)
{
    opt_set set(argc, argv);

    ostream& out = set.output();

    out << set.machines.size() << " " << set.n << endl;

    for(int i = 0; i < set.n; ++i)
    {
        for(auto& m : set.machines)
        {
            out << m.distr() << " ";
        }
        out << endl;
    }
}
