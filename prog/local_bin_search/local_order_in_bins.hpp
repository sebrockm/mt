#ifndef _LOCAL_ORDER_IN_BINS_HPP_
#define _LOCAL_ORDER_IN_BINS_HPP_


#include "local_2opt.hpp"

#include <vector>


using namespace std;


//rearranges the jobgroups in the bins while the bins' positions are fix
double local_order_in_bins(vector<bin>& bins)
{
    int m = bins.size();

    double cost = calculate_cost(bins);

    bool improvement;
    do
    {
        improvement = false;

        for(int i = 0; i < m; ++i)
        {
            //save the bins
            bin a = bins[i];
            bin b = bins[(i+1)%m];

            calculate_opt_rearrange(bins[i], bins[(i+1)%m]);
            double new_cost = calculate_cost(bins);

            //if no improvement
            if(new_cost >= cost)
            {
                //recreate former state
                bins[i] = a;
                bins[(i+1)%m] = b;
            }
            else
            {
                improvement = true;
                cost = new_cost;
            }
        }
    } while(improvement);

    return cost;
}


#endif
