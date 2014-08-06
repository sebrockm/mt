#ifndef _GG_HEURISTIK_HPP_
#define _GG_HEURISTIK_HPP_

#include "gilmore_gomory/gg.h"
#include "nonfull_schedule.hpp"

#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

typedef typename nonfull_schedule<int>::job job;


pair<int, int> find_best_dom(const vector<job>& jobs)
{
    vector<int> mins(jobs[0].size(), numeric_limits<int>::max());
    vector<int> maxs(jobs[0].size(), 0);

    for(unsigned i = 0; i < jobs.size(); ++i)
    {
        for(unsigned j = 0; j < jobs[i].size(); ++j)
        {
            mins[j] = min(mins[j], jobs[i][j]);
            maxs[j] = max(maxs[j], jobs[i][j]);
        }
    }
    
    auto k = numeric_limits<float>::max();

    int pos = 0;
    for(unsigned i = 0; i+1 < mins.size(); i++)
    {
        int mini = min(mins[i], mins[i+1]);
        int maxi = 0;
        for(unsigned j = 0; j < mins.size(); ++j)
        {
            if(j == i || j == i+1)
                continue;
            maxi = max(maxi, maxs[i]);
        }

        if(k > ((float)mini) / maxi)
        {
            k = ((float)mini) / maxi;
            pos = i;
        }
    }

    return {pos, pos+1};
}


vector<job> gg_heuristik(const vector<job>& jobs, int dom1, int dom2)
{
    dom1 = 3;
    dom2 = 4;
    auto pi = tsp_gg(jobs, dom1, dom2);
    //cerr << "dom1 " << dom1 << " dom2 " << dom2 << endl;

    vector<job> sch(jobs.size());
    for(unsigned i = 0; i < sch.size(); ++i)
    {
        //cerr << "pi[" << i << "]=" << pi[i] << endl;
        //cerr << "sch[" << i << "] = jobs[" << pi[i]-1 << "]" << endl;
        sch[i] = jobs[pi[i]-1];
    }

    return sch;
}


#endif
