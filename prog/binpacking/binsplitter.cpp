#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "../local_bin_search/local_bin_search.hpp" //for jobgroup and bin

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

    //read group sizes
    vector<jobgroup> groups(n);
    int sum = 0;
    for(int i = 1; i < n+1; i++)
    {
        int size;
        stringstream(vfile[i]) >> size;
        groups[i-1].id = i-1;
        groups[i-1].length = size;
        sum += size;
    }
    int binsize = sum / numbins;
    if(!silent)
    {
        cout << "sum of group sizes is " << sum << " so each bin has size " << binsize << endl;
    }

    //read resources
    for(int i = n+1; i < 2*n+1; i++)
    {
        int res;
        stringstream(vfile[i]) >> res;
        groups[i-n-1].res = res;
    }
    if(!silent)
    {
        cout << "resources read" << endl;
    }

    //read process times
    for(int i = 2*n+1; i < 3*n+1; i++)
    {
        double t1, t2;
        stringstream(vfile[i]) >> t1 >> t2;
        groups[i-2*n-1].times = {t1, t2};
    }
    if(!silent)
    {
        cout << "process times read" << endl;
    }

    //assign the groups to the bin binfile tells they belong to
    //and get the sizes of the bins
    vector<bin> bins(numbins);
    vector<int> binsizes(numbins);
    for(auto line : vbinfile)
    {
        int group, bin;
        stringstream(line) >> group >> bin;
        bins[bin-1].id = bin-1;
        bins[bin-1].groups.push_back(groups[group-1]);
        binsizes[bin-1] += groups[group-1].length;
    }
    if(!silent)
    {
        cout << "bins read" << endl;
    }

    int first_empty;
    for(first_empty = 0; binsizes[first_empty] > 0; ++first_empty);


    for(int bin = 0; bin < numbins; bin++)
    {
        //split overfull bins
        if(binsizes[bin] > binsize)
        {
            //comparator for jobgroups
            auto comparator = [binsize] (const jobgroup& g1, const jobgroup& g2) 
            {
                //compare by available resources or by id if equal
                if(g1.res - g1.length / binsize == g2.res - g2.length / binsize)
                    return g1.id < g2.id;
                return g1.res - g1.length / binsize == g2.res - g2.length / binsize;
            };

            //sort groups by comparator
            //so groups with just a few resources will be assigned to bin first 
            sort(bins[bin].groups.begin(), bins[bin].groups.end(), comparator);

            //search for group to split
            while(true)
            {
                int fill_level = 0;//of current bin
                int g = 0;
                while(fill_level < binsize)
                {
                    fill_level += bins[bin].groups[g].length;
                    g++;
                } //g+1 is the index of the group to be split
                --g;

                //is split possible?
                if(bins[bin].groups[g].res - bins[bin].groups[g].length / binsize > 1)
                {
                    int new_gsize = fill_level - binsize;
                    bins[bin].groups[g].length -= new_gsize;
                    groups[bins[bin].groups[g].id].length -= new_gsize;

                    //add new group to empty bin and configurate it
                    bins[first_empty].groups.push_back(bins[bin].groups[g]);
                    bins[first_empty].groups.back().length = new_gsize;
                    binsizes[first_empty] = new_gsize;
                    bins[first_empty].groups.back().id = n++;
                    groups.push_back(bins[first_empty].groups.back());

                    //give them resources
                    bins[first_empty].groups.back().res = bins[bin].groups[g].res - 1;
                    bins[bin].groups[g].res = 1;

                    if(!silent)
                    {
                        cout << "split bin " << bin+1 << ", it's " << new_gsize << " too full:" << endl;
                        cout << "now group " << bins[bin].groups[g].id+1 << " has size " << bins[bin].groups[g].length << endl;
                        cout << "new group " << n << " created with size " << new_gsize << " in bin " << first_empty+1 << endl;
                    }

                    //transfer remaining groups to that bin
                    while(bins[bin].groups.size() > (unsigned)g+1)
                    {
                        bins[first_empty].groups.push_back(move(bins[bin].groups.back()));
                        binsizes[first_empty] += bins[bin].groups.back().length;
                        bins[bin].groups.pop_back();
                        if(!silent)
                        {
                            cout << "group " << bins[first_empty].groups.back().id+1 << " moved to bin " << first_empty+1 << endl;
                        }
                    }

                    binsizes[bin] = binsize;
                    ++first_empty;

                    break;
                }
                else //this group cannot be split
                {
                    //try next lexicographical permutation
                    bool next = next_permutation(bins[bin].groups.begin(), bins[bin].groups.end(), comparator);

                    if(!next) //tried all possibilities to split
                    {
                        throw "impossible to split";
                    }
                } 
            }
        }
    }

    //write results back to files
    ofstream file(argv[2]);
    file << n << endl << endl;

    for(auto& g : groups)
    {
        file << g.length << endl;
    }
    file << endl;

    for(auto& g : groups)
    {
        file << g.res << endl;
    }
    file << endl;

    for(auto& g : groups)
    {
        file << g.times[0] << g.times[1] << endl;
    }

    ofstream binfile(argv[3]);
    for(auto& bin : bins)
    {
        for(auto& g : bin.groups)
        {
            binfile << g.id+1 << " " << bin.id+1 << endl;
        }
    }

    if(!silent)
    {
        cout << "done" << endl;
    }
}
