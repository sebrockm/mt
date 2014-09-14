#include "local_bin_search.hpp"
#include "simulated_annealing.hpp"
#include "local_order_in_bins.hpp"
#include "lower_bound.hpp"
#include "greedy_bin_sequence.hpp"

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>


using namespace std;


vector<bin> read_bins(const char* group_file, const char* bin_file)
{
    ifstream group_stream(group_file);
    ifstream bin_stream(bin_file);

    if(!group_stream)
    {
        cerr << "cannot read groups from " << group_file << endl;
        throw 1;
    }

    if(!bin_stream)
    {
        cerr << "cannot read bins from " << bin_file << endl;
        throw 1;
    }

    int n;
    group_stream >> n;
    cerr << "read n:" << n << endl;

    vector<jobgroup> groups(n);
    for(int i = 0; i < n; ++i)
    {
        group_stream >> groups[i].length;
        cerr << "read length " << i+1 << ":" << groups[i].length << endl;
        groups[i].id = i+1;
    }

    int dummy;
    for(int i = 0; i < n; ++i)
    {
        group_stream >> dummy;
        cerr << "skipped " << dummy << endl;
    }

    for(int i = 0; i < n; ++i)
    {
        groups[i].times.resize(2);
        for(int j = 0; j < 2; ++j)
        {
            group_stream >> groups[i].times[j];
            cerr << "read time " << i+1 << " " << j+1 << ":" << groups[i].times[j] << " ";
        }
        cerr << endl;
    }


    int id, b;
    vector<bin> bins(8);
    for(int i = 0; i < n; ++i)
    {
        bin_stream >> id >> b;
        /*if(id != i+1)
        {
            cerr << "id of group " << i+1 << " equals " << id << endl;
            throw 1;
        }*/
        cerr << "read group " << id << " is in bin " << b << endl;
        bins[b-1].groups.push_back(move(groups[id-1]));
    }

    for(int i = 0; i < 8; ++i)
    {
        bins[i].id = i;
    }

    return bins;
}

int main(int argc, char** argv)
{
    vector<bin> bins(8);

    /*bins[0].groups = vector<jobgroup>(2);
    bins[0].groups[1].times = {4,4};
    bins[0].groups[1].length = 4;
    bins[0].groups[0].times = {6,6};
    bins[0].groups[0].length = 1;

    bins[1].groups = vector<jobgroup>(2);
    bins[1].groups[1].times = {3,3};
    bins[1].groups[1].length = 3;
    bins[1].groups[0].times = {7,7};
    bins[1].groups[0].length = 2;

    bins[2].groups = vector<jobgroup>(2);
    bins[2].groups[0].times = {2,2};
    bins[2].groups[0].length = 2;
    bins[2].groups[1].times = {8,8};
    bins[2].groups[1].length = 3;

    bins[3].groups = vector<jobgroup>(2);
    bins[3].groups[0].times = {1,1};
    bins[3].groups[0].length = 1;
    bins[3].groups[1].times = {9,9};
    bins[3].groups[1].length = 4;

    bins[4].groups = vector<jobgroup>(1);
    bins[4].groups[0].times = {5,5};
    bins[4].groups[0].length = 5;

    bins[5].groups = vector<jobgroup>(1);
    bins[5].groups[0].times = {6,6};
    bins[5].groups[0].length = 5;

    bins[6].groups = vector<jobgroup>(1);
    bins[6].groups[0].times = {7,7};
    bins[6].groups[0].length = 5;

    bins[7].groups = vector<jobgroup>(1);
    bins[7].groups[0].times = {8,8};
    bins[7].groups[0].length = 5;*/

    if(argc < 2 || argc > 3)
    {
        cerr << "usage: " << argv[0] << " group_file [bin_file]" << endl;
        return 1;
    }

    string bin_file = argc == 3 ? argv[2] : string(argv[1])+".bins.dat";
    cerr << "read groups from " << argv[1] << " and bins from " << bin_file << endl;

    bins = read_bins(argv[1], bin_file.c_str());


    //double cost;
    /*cerr << "begin SA" << endl;
    bins = simulated_annealing(bins, cost);
    cout << cost << "\t";

*/
    cerr << "begin LB" << endl;
    cout << lower_bound(bins) << endl;

    //read bin order from input
    /*vector<bin> bins2(bins.size());
    for(unsigned i = 0; i < bins.size(); ++i)
    {
        int id, pos;
        double skip;
        cin >> id >> pos >> skip;
        bins2[id-1] = bins[pos-1];
        cerr << id << " " << pos << endl;
    }

    cerr << "begin local opt" << endl;
    cout << local_order_in_bins(bins2) << "\t";
*/
    /*cerr << "begin rearranging bins" << endl;
    auto order = greedy_bin_sequence(bins);
    for(unsigned i = 0; i < order.size(); ++i)
        cout << i+1 << " " << order[i]+1 << " 1.0000000" << endl;
*/
    /*vector<bin> bins2(bins.size());
    unsigned char c = 0;
    for(unsigned i = 0; i < bins.size(); ++i)
    {
        bins2[i] = bins[order[i]-1];
        c |= 1 << (order[i]-1);
    }
    cerr << hex << (unsigned)c << dec << endl;
    cerr << "begin local opt again" << endl;
    cout << local_order_in_bins(bins2) << "\t";


    cerr << "begin SA" << endl;
    bins = simulated_annealing(bins, cost);

    cout << cost << endl;
    */
}
