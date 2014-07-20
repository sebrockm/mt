#ifndef _DE_NONFULL_SCHEDULE_HPP_
#define _DE_NONFULL_SCHEDULE_HPP_


#include <array>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ostream>
#include <iomanip>

#include <boost/range/adaptor/reversed.hpp>

using namespace std;



template <class time_type>
class de_nonfull_schedule
{
public:
    typedef vector<time_type> job;

    vector<job> jobs_front, jobs_back;
    const unsigned m;

    de_nonfull_schedule(unsigned m, unsigned n = 0)
        :jobs_front(), jobs_back(), m(m)
    {
        jobs_front.reserve(n);
        jobs_back.reserve(n);
    }

    time_type get_last_cycle_time_front(unsigned i) const
    {
        time_type t = 0;
        for(unsigned j = max(0, (int)(i+1-jobs_front.size())); j <= i && j < m; ++j)
        {
            t = max(t, jobs_front[jobs_front.size()-1-i+j][m-1-j]);
        }

        return t;
    }

    time_type get_last_cycle_time_back(unsigned i) const
    {
        time_type t = 0;
        for(unsigned j = max(0, (int)(i+1-jobs_back.size())); j <= i && j < m; ++j)
        {
            t = max(t, jobs_back[jobs_back.size()-1-i+j][j]);
        }

        return t;
    }

    void add_job_front(const job& j)
    {
        jobs_front.push_back(j);
    }

    void add_job_back(const job& j)
    {
        jobs_back.push_back(j);
    }

    time_type evaluate_job_front(const job& j) const 
    {
        /*if(jobs_front.empty())
        {
            return j.front();
        }*/

        time_type diff = 0;
        for(int i = 0; i < m-1; ++i)
        {
            diff += abs(j[m-2-i] - get_last_cycle_time_front(i));
        }

        return diff;
    }

    time_type evaluate_job_back(const job& j) const 
    {
        /*if(jobs_back.empty())
        {
            return j.back();
        }*/

        time_type diff = 0;
        for(int i = 0; i < m-1; ++i)
        {
            diff += abs(j[i+1] - get_last_cycle_time_back(i));
        }

        return diff;
    }

    time_type get_cost() const
    {
        time_type sum = 0;
        for(unsigned i = 0; i < jobs_front.size(); ++i)
        {
            sum += get_last_cycle_time_front(jobs_front.size()+m-2-i);
        }
        for(int i = 0; i < m-1; ++i)
        {
            sum += max(get_last_cycle_time_front(m-2-i), get_last_cycle_time_back(i));
        }
        for(unsigned i = 0; i < jobs_back.size(); ++i)
        {
            sum += get_last_cycle_time_back(i+m-1);
        }

        return sum;
    }
};


template <class time_type>
de_nonfull_schedule<time_type> create_de_schedule(unsigned m, vector<vector<time_type>>& unscheduled)
{
    typedef typename de_nonfull_schedule<time_type>::job job;

    de_nonfull_schedule<time_type> schedule(m, unscheduled.size());

    while(!unscheduled.empty())
    {
        auto mini_front = min_element(unscheduled.begin(), unscheduled.end(), [&schedule](const job& j1, const job& j2)
                {
                    return schedule.evaluate_job_front(j1) < schedule.evaluate_job_front(j2);
                });

        auto mini_back = min_element(unscheduled.begin(), unscheduled.end(), [&schedule](const job& j1, const job& j2)
                {
                    return schedule.evaluate_job_back(j1) < schedule.evaluate_job_back(j2);
                });

        if(schedule.evaluate_job_front(*mini_front) < schedule.evaluate_job_back(*mini_back))
        {
            schedule.add_job_front(*mini_front);
            swap(*mini_front, unscheduled.back());
        }
        else
        {
            schedule.add_job_back(*mini_back);
            swap(*mini_back, unscheduled.back());
        }
        unscheduled.pop_back();
    }

    return schedule;
}


template <class time_type>
ostream& operator << (ostream& s, const de_nonfull_schedule<time_type>& nfs)
{
    for(int i = 0; i < nfs.m; ++i)
    {
        for(int j = 0; j < i; ++j)
        {
            s << "  ";
        }
        for(auto& j : nfs.jobs_front)
        {
            s << setw(2) << j[i];
        }
        for(int i = 0; i < nfs.jobs_front.capacity()-nfs.jobs_front.size()-nfs.jobs_back.size(); ++i)
        {
            s << "  ";
        }
        for(auto& j : boost::adaptors::reverse(nfs.jobs_back))
        {
            s << setw(2) << j[i];
        }
        s << endl;
    }
    return s << setw(1);
}


#endif