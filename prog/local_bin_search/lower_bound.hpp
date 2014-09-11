#ifndef _LOWER_BOUND_HPP_
#define _LOWER_BOUND_HPP_


#include "local_2opt.hpp"

#include <vector>

using namespace std;



//calculates a lower bound of cycle times
double lower_bound(const vector<bin>& bins)
{
    int m = bins.size();

    double bound = 0;

    for(int i = 0; i < m; ++i)
    {
        //left and right min
        double lmin = numeric_limits<double>::max();
        double rmin = numeric_limits<double>::max();
        for(int j = 0; j < m; ++j)
        {
            if(i != j)
            {
                lmin = min(lmin, calculate_opt(bins[j], bins[i]));
                rmin = min(rmin, calculate_opt(bins[i], bins[j]));
            }
        }

        bound += lmin + rmin;
    } 

    return bound / 2;
}




#endif
