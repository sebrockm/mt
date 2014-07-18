#ifndef _NONFULL_SCHEDULE_HPP_
#define _NONFULL_SCHEDULE_HPP_

#include <array>
#include <vector>
#include <algorithm>
#include <iostream>


using namespace std;



template <unsigned m, class time_type>
class nonfull_schedule
{
public:
    typedef vector<time_type> job;

    vector<job> jobs;

    nonfull_schedule(unsigned n = 0)
        :jobs() 
    {
        jobs.reserve(n);
    }

    time_type get_last_cycle_time(unsigned i) const
    {
        time_type t = 0;
        for(unsigned j = max(0, (int)(i+1-jobs.size())); j <= i; ++j)
        {
            t = max(t, jobs[jobs.size()-1-i+j][m-1-j]);
        }

        return t;
    }

    void add_job(const job& j)
    {
        jobs.push_back(j);
    }

    int evaluate_job(const job& j)
    {
        int diff = 0;
        for(int i = 0; i < m-1; --i)
        {
            diff += abs(j[m-2-i] - get_last_cycle_time(i));
        }

        return diff;
    }
};


template <unsigned m, class time_type>
vector<typename nonfull_schedule<m, time_type>::job> create_schedule(vector<typename nonfull_schedule<m, time_type>::job>& unscheduled)
{
    typedef typename nonfull_schedule<m, time_type>::job job;

    nonfull_schedule<m, time_type> schedule(unscheduled.size());

    while(!unscheduled.empty())
    {
        auto mini = min_element(unscheduled.begin(), unscheduled.end(), [&schedule](const job& j1, const job& j2)
                {
                    return schedule.evaluate_job(j1) < schedule.evaluate_job(j2);
                });

        schedule.add_job(*mini);

        swap(*mini, unscheduled.back());
        unscheduled.pop_back();
    }

    return schedule.jobs;
}

#endif
