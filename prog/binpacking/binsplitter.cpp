#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

vector<string> readFile(string name)
{
    ifstream file(name);
    vector<string> vfile;
    while(file)
    {
        string line;
        getline(file, line);
        if(!line.empty() && line.at(0) != '#')
            vfile.push_back(line);
    }
    return vfile;
}

int main(int argc, char** argv)
{
    if(argc != 5 && argc != 4)
    {
        cerr << "usage: " << argv[0] << " <numbins> <file> <binfile> [-silent]" << endl;
        exit(EXIT_FAILURE);
    }

    int numbins;
    stringstream(argv[1]) >> numbins;
    vector<string> vfile = readFile(argv[2]);
    vector<string> vbinfile = readFile(argv[3]);
    bool silent = argc == 5 && argv[4] == string("-silent");

    int n;
    stringstream(vfile.front()) >> n;

    //map groups to the bin binfile tells they belong to
    //and get the biggest bin number
    vector<int> group_to_bin(n);
    int maxbin = 0;
    for(auto line : vbinfile)
    {
        int group, bin;
        stringstream(line) >> group >> bin;
        group_to_bin[group-1] = bin-1;
        maxbin = max(bin-1, maxbin);
    }

    //read group sizes
    vector<int> groupsizes(n);
    int sum = 0;
    for(int i = 1; i < n+1; i++)
    {
        int size;
        stringstream(vfile[i]) >> size;
        groupsizes[i-1] = size;
        sum += size;
    }
    int binsize = sum / numbins;
    if(!silent)
    {
        cout << "sum of group sizes is " << sum << " so each bin has size " << binsize << endl;
    }

    //read resources
    vector<int> resources(n);
    for(int i = n+1; i < 2*n+1; i++)
    {
        int res;
        stringstream(vfile[i]) >> res;
        resources[i-n-1] = res;
    }

    //read process times
    vector<pair<string,string>> times(n);
    for(int i = 2*n+1; i < 3*n+1; i++)
    {
        string t1, t2;
        stringstream(vfile[i]) >> t1 >> t2;
        times[i-2*n-1] = {t1, t2};
    }

    for(int bin = 0; bin < numbins; bin++)
    {
        int asize = 0;//size of actual bin
        int group = 0;

        //fill bin until it's full
        while(group < n && asize < binsize)
        {
            if(group_to_bin[group] == bin)
            {
                asize += groupsizes[group];
                if(!silent)
                {
                    cout << "group " << group+1 << " stays in bin " << bin+1 << endl;
                    cout << "size of bin " << bin+1 << ": " << asize << endl;
                }
            }
            group++;
        }
        if(asize > binsize)//split group
        {
            int diff = asize - binsize;
            groupsizes[group-1] -= diff;

            //install new group
            groupsizes.push_back(diff);
            resources.push_back(resources[group-1]);//TODO resources may not be available
            times.push_back(times[group-1]);
            group_to_bin.push_back(bin);

            if(!silent)
            {
                cout << "split bin " << bin+1 << ", it's " << diff << " too full:" << endl;
                cout << "group " << group << " now has size " << groupsizes[group-1] << endl;
                cout << "new group " << n+1 << " created with size " << diff << " in bin " << bin+1 << endl;
            }

            n++;
            maxbin++;

            //move other groups from this bin to the next free one
            while(group < n)
            {
                if(group_to_bin[group] == bin)
                {
                    if(!silent)
                    {
                        cout << "group " << group+1 << " moved to bin " << maxbin+1 << endl;
                    }
                    group_to_bin[group] = maxbin;
                }
                group++;
            }

        }
    }

    stringstream f;
    f << argv[2];
    ofstream file(f.str());
    file << n << endl << endl;
    
    for(auto s : groupsizes)
    {
        file << s << endl;
    }
    file << endl;
    for(auto r : resources)
    {
        file << r << endl;
    }
    file << endl;
    for(auto p : times)
    {
        file << p.first << " " << p.second << endl;
    }

    f.str("");
    f << argv[3];
    ofstream binfile(f.str());
    for(int i = 0; i < n ; i++)
    {
        binfile << i+1 << " " << group_to_bin[i]+1 << endl;
    }
}
