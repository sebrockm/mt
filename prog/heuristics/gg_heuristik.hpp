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


//Find those neighboring dominating machines that qualify best for gg.
pair<int, int> find_best_dom(const vector<job>& jobs)
{
    //find the minimum and maximum process time on each machine
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

    //calculate k for all pairs of neighboring dominating machines 
    //and choose smallest
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
    //transform non_full_schedule::job to vector<int> as needed by gg
    vector<vector<int>> gg_jobs(jobs.size());
    for(unsigned i = 0; i < jobs.size(); ++i)
    {
        gg_jobs[i] = jobs[i];
    }

    auto pi = tsp_gg(gg_jobs, dom1, dom2); // gg call

    //pi is a vector of indexes, so transform it into a vector of jobs
    vector<job> sch(jobs.size());
    for(unsigned i = 0; i < sch.size(); ++i)
    {
        sch[i] = jobs[pi[i]-1];
    }

    return sch;
}


#endif
