#ifndef _LOCAL_BIN_SEARCH_HPP_
#define _LOCAL_BIN_SEARCH_HPP_

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>

using namespace std;

struct jobgroup
{
    vector<double> times;
    unsigned length;
    unsigned id;
    unsigned res; //available resources
};

struct bin
{
    vector<jobgroup> groups;
    unsigned id;
};

//Calculates the cost between two bins.
//Normally the station of b1 is before b2's station.
//Otherwise n must be set to true
//which means that the first job of b2 and the last job of b1 must not be taken into account.
double calculate_cost(const bin& b1, const bin& b2, bool n = false)
{
    double sum = 0;

    auto j1 = b1.groups.begin();
    auto j2 = b2.groups.begin();

    int rem1 = j1->length;
    int rem2 = j2->length;

    if(n)
    {
        if(j2->length == 1)
        {
            ++j2;
            rem2 = j2->length;
        }
        else
        {
            rem2--;
        }
    }


    while(j1 != b1.groups.end() && j2 != b2.groups.end())
    {
        if(rem1 < rem2)
        {
            sum += rem1 * max(j1->times[1], j2->times[0]);
            ++j1;
            rem2 -= rem1;
            rem1 = (j1 == b1.groups.end()) ? 0 : j1->length;
        }
        else if(rem2 < rem1)
        {
            sum += rem2 * max(j1->times[1], j2->times[0]);
            ++j2;
            rem1 -= rem2;
            rem2 = (j2 == b2.groups.end()) ? 0 : j2->length;
        }
        else
        {
            sum += rem2 * max(j1->times[1], j2->times[0]);
            ++j1;
            ++j2;
            rem1 = (j1 == b1.groups.end()) ? 0 : j1->length;
            rem2 = (j2 == b2.groups.end()) ? 0 : j2->length;
        }

        if(n && j2 == b2.groups.end() && rem1 == 1)
        {
            break;
        }
    }

    return sum;
}


class local_bin_search
{
private:
    vector<bin>& _bins;

    vector<double> _costs;
    double _cost;

    vector<double> _tmpCosts;
    double _tmpCost;

public:
    local_bin_search(vector<bin>& bins)
        :_bins(bins), _costs(bins.size()), _cost(0), _tmpCosts(bins.size()), _tmpCost(0)
    {
        for(unsigned i = 0; i < _bins.size(); ++i)
        {
            _costs[i] = _tmpCosts[i] = calculate_cost(_bins[i], _bins[(i+1)%_bins.size()], i+1 == _bins.size());
            _cost += _costs[i];
        }
        _tmpCost = _cost;
    }

    double get_cost() const
    {
        return _cost;
    }

    double calculate_bin_exchange(unsigned i, unsigned j)
    {
        if(i == j)
            return 0;
        if(j < i && (j != 0 || i+1 != _bins.size()))
            swap(i, j);//now i is definitely on j's left


        unsigned bi = (i+_bins.size()-1)%_bins.size();
        unsigned bj = (j+_bins.size()-1)%_bins.size();
        unsigned ni = (i+1)%_bins.size();
        unsigned nj = (j+1)%_bins.size();

        if(ni == j)
        {
            _tmpCosts[bi] = calculate_cost(_bins[bi], _bins[j], i == 0);
            _tmpCosts[i] = calculate_cost(_bins[j], _bins[i], j == 0);
            _tmpCosts[j] = calculate_cost(_bins[i], _bins[nj], nj == 0);
        }
        else
        {
            _tmpCosts[bi] = calculate_cost(_bins[bi], _bins[j], i == 0);
            _tmpCosts[i] = calculate_cost(_bins[j], _bins[ni], ni == 0);
            _tmpCosts[bj] = calculate_cost(_bins[bj], _bins[i], j == 0);
            _tmpCosts[j] = calculate_cost(_bins[i], _bins[nj], nj == 0);
        }



        _tmpCost = 0;
        for(unsigned k = 0; k < _costs.size(); ++k)
        {
            if(k != bi && k != i && k != bj && k != j)
            {
                _tmpCosts[k] = _costs[k];
            }
            _tmpCost += _tmpCosts[k];
        }

        return _tmpCost - _cost;
    }

    void exchange_bins(unsigned i, unsigned j)
    {
        swap(_bins[i], _bins[j]);
        swap(_tmpCosts, _costs);
        _cost = _tmpCost;
    }

    double calculate_group_exchange(unsigned bin, unsigned i, unsigned j) 
    {
        if(i == j)
            return 0;

        swap(_bins[bin].groups[i], _bins[bin].groups[j]);

        unsigned bbin = (bin+_bins.size()-1)%_bins.size();
        _tmpCosts[bbin] = calculate_cost(_bins[bbin], _bins[bin], bin == 0);
        _tmpCosts[bin] = calculate_cost(_bins[bin], _bins[(bin+1)%_bins.size()], bin+1 == _bins.size());

        swap(_bins[bin].groups[i], _bins[bin].groups[j]);


        _tmpCost = 0;
        for(unsigned k = 0; k < _costs.size(); ++k)
        {
            if(k != bbin && k != bin)
            {
                _tmpCosts[k] = _costs[k];
            }
            _tmpCost += _tmpCosts[k];
        }

        return _tmpCost - _cost;
    }

    void exchange_groups(unsigned bin, unsigned i, unsigned j)
    {
        swap(_bins[bin].groups[i], _bins[bin].groups[j]);
        swap(_tmpCosts, _costs);
        _cost = _tmpCost;
    }
};


#endif
