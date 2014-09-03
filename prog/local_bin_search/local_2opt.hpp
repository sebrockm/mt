#ifndef _LOCAL_2OPT_HPP_
#define _LOCAL_2OPT_HPP_


#include "local_bin_search.hpp"
#include <algorithm>

using namespace std;


//calculates the optimal cost that could occur between two bins
double calculate_opt(bin b1, bin b2)
{
    //sort groups in bin1 descending by times on second dominating machine
    sort(b1.groups.begin(), b1.groups.end(), 
            [](const jobgroup& g1, const jobgroup& g2) {return g1.times[1] > g2.times[1];});

    //sort groups in bin2 descending by times on first dominating machine
    sort(b2.groups.begin(), b2.groups.end(), 
            [](const jobgroup& g1, const jobgroup& g2) {return g1.times[0] > g2.times[0];});

    return calculate_cost(b1, b2);
}


//calculates the optimal cost that could occur between two bins
//and rearranges the job groups in those bins that way
double calculate_opt_rearrange(bin& b1, bin& b2)
{
    //sort groups in bin1 descending by times on second dominating machine
    sort(b1.groups.begin(), b1.groups.end(), 
            [](const jobgroup& g1, const jobgroup& g2) {return g1.times[1] > g2.times[1];});

    //sort groups in bin2 descending by times on first dominating machine
    sort(b2.groups.begin(), b2.groups.end(), 
            [](const jobgroup& g1, const jobgroup& g2) {return g1.times[0] > g2.times[0];});

    return calculate_cost(b1, b2);
}

#endif
