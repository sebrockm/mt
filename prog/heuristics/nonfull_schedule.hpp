#ifndef _NONFULL_SCHEDULE_HPP_
#define _NONFULL_SCHEDULE_HPP_

#include <array>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ostream>
#include <iomanip>

using namespace std;



template <class time_type>
class nonfull_schedule
{
public:
    typedef vector<time_type> job;

    vector<job> jobs;
    const unsigned m;

    nonfull_schedule(unsigned m, unsigned n = 0)
        :jobs(), m(m)
    {
        jobs.reserve(n);
    }

    time_type get_last_cycle_time(unsigned i) const
    {
        time_type t = 0;
        for(unsigned j = max(0, (int)(i+1-jobs.size())); j <= i && j < m; ++j)
        {
            t = max(t, jobs[jobs.size()-1-i+j][m-1-j]);
        }

        return t;
    }

    void add_job(const job& j)
    {
        jobs.push_back(j);
    }

    pair<time_type, time_type> evaluate_job(const job& j) const 
    {
        /*if(jobs.empty())
        {
            return j.front();
        }*/

        pair<time_type, time_type> diff = {0, 0};
        for(int i = 0; i < m-1; ++i)
        {
            diff.first += max(j[m-2-i] - get_last_cycle_time(i), (time_type)0);
            diff.second += max(get_last_cycle_time(i) - j[m-2-i], (time_type)0);
        }

        return diff;
    }

    time_type get_cost() const
    {
        time_type sum = 0;
        for(unsigned i = 0; i < jobs.size() + m - 1; ++i)
        {
            sum += get_last_cycle_time(i);
        }

        return sum;
    }
};


template <class time_type>
nonfull_schedule<time_type> create_schedule(unsigned m, vector<vector<time_type>>& unscheduled)
{
    typedef typename nonfull_schedule<time_type>::job job;

    nonfull_schedule<time_type> schedule(m, unscheduled.size());

    while(!unscheduled.empty())
    {
        auto mini = min_element(unscheduled.begin(), unscheduled.end(), [&schedule](const job& j1, const job& j2)
                {
                    auto p1 = schedule.evaluate_job(j1); 
                    auto p2 = schedule.evaluate_job(j2);
                    if(p1.first == p2.first)
                        return p1.second < p2.second;
                    return p1.first < p2.first;
                });

        schedule.add_job(*mini);

        swap(*mini, unscheduled.back());
        unscheduled.pop_back();
    }

    return schedule;
}


template <class time_type>
ostream& operator << (ostream& s, const nonfull_schedule<time_type>& nfs)
{
    for(int i = 0; i < nfs.m; ++i)
    {
        for(int j = 0; j < i; ++j)
        {
            s << "  ";
        }
        for(auto& j : nfs.jobs)
        {
            s << setw(2) << j[i];
        }
        s << endl;
    }
    return s << setw(1);
}


#endif
