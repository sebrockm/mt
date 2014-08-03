#ifndef _NEIGHBORHOOD_HPP_
#define _NEIGHBORHOOD_HPP_

#include <vector>
#include <algorithm>


using namespace std;



class neighborhood
{
public:
    typedef vector<int> job;

protected:
    vector<job>& _pi;

public:
    neighborhood(vector<job>& pi)
        :_pi(pi) {}

    virtual int gain(int i, int j) = 0;

    virtual void make(int i, int j) = 0;
};


class xch_neighborhood : public neighborhood
{
public:
    xch_neighborhood(vector<job>& pi)
        :neighborhood(pi) {}

    virtual int gain(int i, int j)
    {
        if(j < i)
            swap(i, j);

        int m = _pi[0].size();
        int n = _pi.size();

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
        for(int k = max(j, i+m); k < j+m && k < n+m-1; ++k)
        {
            int maxt = 0;
            for(int l = max(0, k-n+1); l < m && k-l >= 0; ++l)
            {
                maxt = max(maxt, _pi[k-l][l]);
            }

            before += maxt;
        }

        this->make(i, j);

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

        this->make(i, j);

        return before - after;
    }

    virtual void make(int i, int j)
    {
        swap(_pi[i], _pi[j]);
    }
};



class shift_neighborhood : public neighborhood
{
public:
    shift_neighborhood(vector<job>& pi)
        :neighborhood(pi) {}

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
