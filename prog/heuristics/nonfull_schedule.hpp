#ifndef _NONFULL_SCHEDULE_HPP_
#define _NONFULL_SCHEDULE_HPP_

#include <array>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ostream>
#include <iomanip>

using namespace std;



//This class represents a non-full schedule.
//It provides some helper methods to decide which job is best do add next.
template <class time_type>
class nonfull_schedule
{
public:
    //this typedef is used by many other heuristics, too
    class job : public vector<time_type> 
    {
    public:
        unsigned group_id;
        using vector<time_type>::vector; //inheriting constructors
    };

    //the list of jobs
    vector<job> jobs;
    const unsigned m;

    //constructor
    nonfull_schedule(unsigned m, unsigned n = 0)
        :jobs(), m(m)
    {
        jobs.reserve(n);
    }

    //calculates the last cycle time in the non-full schedule for i==0,
    //the second last cycle time for i==1, ...
    time_type get_last_cycle_time(unsigned i) const
    {
        time_type t = 0;
        for(unsigned j = max(0, (int)(i+1-jobs.size())); j <= i && j < m; ++j)
        {
            t = max(t, jobs[jobs.size()-1-i+j][m-1-j]);
        }

        return t;
    }

    //calculates the first cycle time in the non-full schedule for i==0,
    //the second cycle time for i==1, ...
    time_type get_first_cycle_time(unsigned i) const
    {
        time_type t = 0;
        for(unsigned j = 0; j < m && j <= i; ++j)
        {
            if(jobs[i-j].size() == m)
                t = max(t, jobs[i-j][j]);
        }

        return t;
    }

    //adds a job at the back of the schedule
    void add_job(const job& j)
    {
        jobs.push_back(j);
    }

    //Evaluates, how well a job would fit at the end of the schedule.
    //The result is a pair of numbers: 
    //The first one is the amount of time the already existing cycle times would be increased by
    //because the job's process times are larger than them.
    //The second one is the amount of time that would be "wasted" 
    //because the job's process times are smaller than the cycle times
    pair<time_type, time_type> evaluate_job(const job& j) const 
    {
        pair<time_type, time_type> diff = {0, 0};
        for(int i = 0; i < m-1; ++i)
        {
            diff.first += max(j[m-2-i] - get_last_cycle_time(i), (time_type)0);
            diff.second += max(get_last_cycle_time(i) - j[m-2-i], (time_type)0);
        }

        return diff;
    }

    //Calculates Cmax of the non-full schedule
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


//Creates a schedule using the non-full-schedule-heuristic.
template <class time_type>
nonfull_schedule<time_type> create_schedule(unsigned m, vector<typename nonfull_schedule<time_type>::job>& unscheduled)
{
    typedef typename nonfull_schedule<time_type>::job job;

    nonfull_schedule<time_type> schedule(m, unscheduled.size());

    while(!unscheduled.empty())
    {
        //find that unscheduled job with the best evaluation
        auto mini = min_element(unscheduled.begin(), unscheduled.end(), [&schedule](const job& j1, const job& j2)
                {
                    auto p1 = schedule.evaluate_job(j1); 
                    auto p2 = schedule.evaluate_job(j2);
                    if(p1.first == p2.first)
                        return p1.second < p2.second;
                    return p1.first < p2.first;
                });

        //add it to non-full schedule
        schedule.add_job(*mini);

        //and remove from unscheduled
        swap(*mini, unscheduled.back());
        unscheduled.pop_back();
    }

    return schedule;
}


//simple way of displaying a nonfull_schedule
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
