#ifndef _DE_NONFULL_SCHEDULE_HPP_
#define _DE_NONFULL_SCHEDULE_HPP_


#include <array>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ostream>
#include <iomanip>

#include <boost/range/adaptor/reversed.hpp>//to access a std::vector in reverse, only needed for output

#include "nonfull_schedule.hpp"

using namespace std;



//This class represents a double ended non-full schedule.
//It provides some helper methods to decide which job is best do add next.
template <class time_type>
class de_nonfull_schedule
{
public:
    typedef typename nonfull_schedule<time_type>::job job;

    //two vectors of jobs, one growing from left to right and one from right to left
    vector<job> jobs_front, jobs_back;
    const unsigned m;

    //constructor
    de_nonfull_schedule(unsigned m, unsigned n = 0)
        :jobs_front(), jobs_back(), m(m)
    {
        jobs_front.reserve(n);
        jobs_back.reserve(n);
    }

    //calculate the i-th last cycle time at the front
    time_type get_last_cycle_time_front(unsigned i) const
    {
        time_type t = 0;
        for(unsigned j = max(0, (int)(i+1-jobs_front.size())); j <= i && j < m; ++j)
        {
            t = max(t, jobs_front[jobs_front.size()-1-i+j][m-1-j]);
        }

        return t;
    }

    //calculate the i-th last cycle time at the end
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

    //evaluate how well a job would fit at the front (compare non_full_schedule)
    pair<time_type, time_type> evaluate_job_front(const job& j) const 
    {
        pair<time_type, time_type> diff = {0, 0};
        for(int i = 0; i < m-1; ++i)
        {
            diff.first += max(j[m-2-i] - get_last_cycle_time_front(i), (time_type)0);
            diff.second += max(get_last_cycle_time_front(i) - j[m-2-i], (time_type)0);
        }

        return diff;
    }

    //evaluate how well a job would fit at the end (compare non_full_schedule)
    pair<time_type, time_type> evaluate_job_back(const job& j) const 
    {
        pair<time_type, time_type> diff = {0, 0};
        for(int i = 0; i < m-1; ++i)
        {
            diff.first += max(j[i+1] - get_last_cycle_time_back(i), (time_type)0);
            diff.second += max(get_last_cycle_time_back(i) - j[i+1], (time_type)0);
        }

        return diff;
    }

    //calculate Cmax of the non-full schedule (both ends together)
    time_type get_cost() const
    {
        time_type sum = 0;

        //those cycles only jobs of the front take into account
        for(unsigned i = 0; i < jobs_front.size(); ++i)
        {
            sum += get_last_cycle_time_front(jobs_front.size()+m-2-i);
        }

        //those cycles both ends take into account
        for(int i = 0; i < m-1; ++i)
        {
            sum += max(get_last_cycle_time_front(m-2-i), get_last_cycle_time_back(i));
        }

        //and those onlye jobs of the back take into account
        for(unsigned i = 0; i < jobs_back.size(); ++i)
        {
            sum += get_last_cycle_time_back(i+m-1);
        }

        return sum;
    }

    //returns the schedule as one vector
    vector<job> get_as_one() const
    {
        vector<job> res = jobs_front;
        res.insert(res.end(), jobs_back.rbegin(), jobs_back.rend());

        return res;
    }
};



//Creates a schedule using the doble ended non-full-schedule-heuristic.
//template <class time_type>
de_nonfull_schedule<int> create_de_schedule(unsigned m, vector<typename de_nonfull_schedule<int>::job> unscheduled)
{
    typedef int time_type;
    typedef typename de_nonfull_schedule<time_type>::job job;

    de_nonfull_schedule<time_type> schedule(m, unscheduled.size());

    while(!unscheduled.empty())
    {
        //get the unscheduled job that would fit best to the front
        auto mini_front = min_element(unscheduled.begin(), unscheduled.end(), [&schedule](const job& j1, const job& j2)
                {
                    auto p1 = schedule.evaluate_job_front(j1);
                    auto p2 = schedule.evaluate_job_front(j2);
                    if(p1.first == p2.first)
                        return p1.second < p2.second;
                    return p1.first < p2.first;
                });

        //get the unscheduled job that would fit best to the end
        auto mini_back = min_element(unscheduled.begin(), unscheduled.end(), [&schedule](const job& j1, const job& j2)
                {
                    auto p1 = schedule.evaluate_job_back(j1);
                    auto p2 = schedule.evaluate_job_back(j2);
                    if(p1.first == p2.first)
                        return p1.second < p2.second;
                    return p1.first < p2.first;
                });

        //add the better one to the front or back respectively
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
