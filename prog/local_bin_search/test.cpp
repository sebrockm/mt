#include "local_bin_search.hpp"

#include <vector>
#include <iostream>


using namespace std;



int main()
{
    vector<bin> bins(8);

    bins[0].groups = vector<jobgroup>(2);
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
    bins[7].groups[0].length = 5;

    local_bin_search lbs(bins);




    bool groupchange = false;
    bool binchange = false;
    do
    {
        do
        {
            groupchange = false;
            for(unsigned bin = 0; bin < bins.size(); ++bin)
            {
                for(unsigned i = 0; i < bins[bin].groups.size(); ++i)
                {
                    for(unsigned j = i+1; j < bins[bin].groups.size(); ++j)
                    {
                        if(lbs.calculate_group_exchange(bin, i, j) < lbs.get_cost())
                        {
                            lbs.exchange_groups(bin, i, j);
                            groupchange = true;
                            cout << "improved to " << lbs.get_cost() << " by exchanging " << i << " and " << j << " in bin " << bin << endl;
                        }
                    }
                }
            }
        }while(groupchange);

        do
        {
            binchange = false;
            for(unsigned i = 0; i < bins.size(); ++i)
            {
                for(unsigned j = i+1; j < bins.size(); ++j)
                {
                    if(lbs.calculate_bin_exchange(i, j) < lbs.get_cost())
                    {
                        lbs.exchange_bins(i, j);
                        binchange = true;
                        cout << "improved to " << lbs.get_cost() << " by exchanging bins " << i << " and " << j << endl;
                    }
                }
            }
        }while(binchange);
    }while(groupchange || binchange);

    

    cout << "cost " << lbs.get_cost() << endl;
    for(auto& bin : bins)
    {
        for(auto& g : bin.groups)
        {
            cout << g.times[0] << ",";
        }
        cout << endl;
    }
}
