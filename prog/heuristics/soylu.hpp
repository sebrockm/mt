#ifndef _SOYLU_HPP_
#define _SOYLU_HPP_

#include "nonfull_schedule.hpp" //for job


#include <algorithm>
#include <iostream>
#include <vector>


using namespace std;

typedef typename nonfull_schedule<int>::job job;



//uses Soylu's H1 to calculate Cmax
int h1(vector<job> jobs)
{
    int n = jobs.size();
    int m = jobs[0].size();

    vector<int> S(n + m - 1);
    for(int i = 0; i < n+m-1; ++i)
    {
        if(i < m)
            S[i] = i + 1;
        else if(i < n)
            S[i] = m;
        else
            S[i] = n + m - i - 1;
    }

    int best_Cmax = numeric_limits<int>::max();
    for(int i = 0; i < m; ++i)
    {
        //sort with respect to process times on machine i
        sort(jobs.begin(), jobs.end(), [i](const job& j1, const job& j2)
                {
                    return j1[i] < j2[i];
                });

        //copy all relevant of S and their indexes
        vector<pair<int, int>> Spos(n);
        for(int j = 0; j < n; ++j)
        {
            Spos[j] = {S[i+j], j};
        }
        sort(Spos.begin(), Spos.end(), [](const pair<int, int>& p1, const pair<int, int>& p2)
                {
                    return p1.first < p2.first;
                });

        //rearrange jobs, Spos[j].second is the index jobs[i] belongs to
        nonfull_schedule<int> sch(m, n); //use nonfull_schdule to calculate Cmax
        for(int j = 0; j < n; ++j)
        {
            sch.add_job(jobs[Spos[j].second]);
        }

        int cmax = sch.get_cost();
        
        best_Cmax = min(best_Cmax, cmax);
    }

    return best_Cmax;
}


//uses Soylu's H2 to calculate Cmax
int h2(vector<job> jobs)
{
    int n = jobs.size();
    int m = jobs[0].size();

    //sort jobs by their greatest process time
    sort(jobs.begin(), jobs.end(), [](const job& j1, const job& j2)
            {
                return *max_element(j1.begin(), j1.end()) < *max_element(j2.begin(), j2.end());
            });

    //use nonfull_schedule to calculate Cmax and cycle times
    nonfull_schedule<int> sch(m, n); 
    sch.jobs.resize(n);

    //assign job with greates process time in such a way that that process time is in cycle m
    int big_machine = distance(max_element(jobs.back().begin(), jobs.back().end()), jobs.back().begin());
    sch.jobs[m-1-big_machine] = move(jobs.back()); //start with biggest process time at positon m
    jobs.pop_back(); //no longer needed

    //index helper for sorting
    vector<int> cycles(n+m-1);
    for(int i = 0; i < n+m-1; ++i)
    {
        cycles[i] = i;
    }

    while(!jobs.empty())
    {
        //sort cycles by cycle times
        sort(cycles.begin(), cycles.end(), [&sch](int a, int b) {return sch.get_first_cycle_time(a) > sch.get_first_cycle_time(b);});

        //index of the next job's machine with greatest process time 
        big_machine = distance(max_element(jobs.back().begin(), jobs.back().end()), jobs.back().begin());
        
        //search first of the sorted cycles where next job can still be added
        auto it = find_if(cycles.begin(), cycles.end(), [&sch, big_machine, n](int id)
                {
                    //job must be inserted at a-big_machine
                    return id-big_machine >= 0 && id-big_machine < n && sch.jobs[id-big_machine].empty();
                });

        if(it == cycles.end()) // may occur if only jobs at the very front or back of sch are still empty
        {
            //find any empty job
            *find_if(sch.jobs.begin(), sch.jobs.end(), [](const job& j) {return j.empty();}) = move(jobs.back());
        }
        else
        {
            sch.jobs[*it-big_machine] = move(jobs.back());
        }
        jobs.pop_back();
    }

    return sch.get_cost();
}


//uses Soylu's H3 to calculate Cmax
int h3(vector<job> jobs)
{
    int m = jobs[0].size();

    //this heuristic works similar to the non-full-schedule heuristic 
    //it differs in the evaluation of the possible jobs to be added
    nonfull_schedule<int> sch(m);
    
    while(!jobs.empty())
    {
        auto best = min_element(jobs.begin(), jobs.end(), [&sch](const job& j1, const job& j2)
                {
                    //take the last process time into account, too
                    return sch.evaluate_job(j1).first + j1.back() < sch.evaluate_job(j2).first + j2.back();
                });

        sch.add_job(*best);
        swap(*best, jobs.back());
        jobs.pop_back();
    }

    return sch.get_cost();
}


//H4 and H5 are not practicable for our problem




#endif
