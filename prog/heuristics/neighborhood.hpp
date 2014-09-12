#ifndef _NEIGHBORHOOD_HPP_
#define _NEIGHBORHOOD_HPP_

#include <vector>
#include <algorithm>

#include "nonfull_schedule.hpp"

using namespace std;



//neighborhood interface
class neighborhood
{
public:
    typedef nonfull_schedule<int>::job job;

protected:
    vector<job>& _pi;

public:
    neighborhood(vector<job>& pi)
        :_pi(pi) {}

    //calculate gain of "doing something" with the i-th and j-th job
    virtual int gain(int i, int j) = 0;

    //"do something" with the i-th and j-th job
    virtual void make(int i, int j) = 0;
};


//exchange neighborhood
class xch_neighborhood : public neighborhood
{
public:
    xch_neighborhood(vector<job>& pi)
        :neighborhood(pi) {}

    //calculate the gain of an exchange of the i-th and j-th job
    virtual int gain(int i, int j)
    {
        //W.l.o.g i < j
        if(j < i)
            swap(i, j);

        int m = _pi[0].size();
        int n = _pi.size();

        //calculate sum of all cycle times i and j are involved in
        int before = 0;
        for(int k = i; k < i+m && k < n+m-1; ++k)
        {
            int maxt = 0;
            for(int l = max(0, k-n+1); l < m && k-l >= 0; ++l)
            {
                maxt = max(maxt, _pi[k-l][l]);
            }

            before += maxt;
        }
        for(int k = max(j, i+m); k < j+m && k < n+m-1; ++k)//make sure to count no cycle time twice
        {
            int maxt = 0;
            for(int l = max(0, k-n+1); l < m && k-l >= 0; ++l)
            {
                maxt = max(maxt, _pi[k-l][l]);
            }

            before += maxt;
        }

        //really make an exchange
        this->make(i, j);

        //calculate cycle times again
        int after = 0;
        for(int k = i; k < i+m && k < n+m-1; ++k)
        {
            int maxt = 0;
            for(int l = max(0, k-n+1); l < m && k-l >= 0; ++l)
            {
                maxt = max(maxt, _pi[k-l][l]);
            }

            after += maxt;
        }
        for(int k = max(j, i+m); k < j+m && k < n+m-1; ++k)
        {
            int maxt = 0;
            for(int l = max(0, k-n+1); l < m && k-l >= 0; ++l)
            {
                maxt = max(maxt, _pi[k-l][l]);
            }

            after += maxt;
        }

        //undo the exchange
        this->make(i, j);

        return before - after;
    }

    //exchange i and j
    virtual void make(int i, int j)
    {
        swap(_pi[i], _pi[j]);
    }
};



//shift neighborhood, actually not used
class shift_neighborhood : public neighborhood
{
public:
    shift_neighborhood(vector<job>& pi)
        :neighborhood(pi) {}

    //TODO
    virtual int gain(int i, int j)
    {
        return 0;
    }

    virtual void make(int i, int j)
    {
        if(i == j)
            return;

        job tmp = move(_pi[i]);

        if(i < j)
        {
            //move one to the left
            for(int k = i; k < j; ++k)
            {
                _pi[k] = move(_pi[k+1]);
            }
        }
        else
        {
            //move one to the right
            for(int k = i; k > j; --k)
            {
                _pi[k] = move(_pi[k-1]);
            }
        }

        _pi[j] = move(tmp);
    }
};

#endif
