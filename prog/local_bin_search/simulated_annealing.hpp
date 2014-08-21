#ifndef _SIMULATED_ANNEALING_HPP_
#define _SIMULATED_ANNEALING_HPP_

#include "local_bin_search.hpp"

#include <vector>
#include <random>
#include <iostream>

using namespace std;



vector<bin> simulated_annealing(vector<bin> pi, double& best_cost)
{
    const int m = pi.size();
    uniform_real_distribution<double> rand(0, 1);
    uniform_int_distribution<int> rand_bin_pos(0, pi.size()-1);
    default_random_engine generator(time(0));

    local_bin_search lbs(pi);
    auto cost = lbs.get_cost();
    cerr << "initial cost = " << cost << endl;

    vector<bin> best_pi = pi;
    best_cost = cost;
    auto t = [] (int i) -> double {return 5000 * abs(cos(i/33.0)) / i;};
    //auto t = [] (int i) -> double {return pow(0.99, i);};

    int no_improvements = 0;
    int i = 1;
    do
    {
        //cerr << i << endl;

        int bin1 = rand_bin_pos(generator);
        if(rand(generator) < 1.0/m) //exchange bins
        {
            int bin2;
            do
            {
                bin2 = rand_bin_pos(generator);
            } while(bin2 == bin1);

            auto gain = lbs.calculate_bin_exchange(bin1, bin2);
            if(rand(generator) < min(1.0, exp(gain / t(i++))))
            {
                lbs.exchange_bins(bin1, bin2);

                cost -= gain;
                if(cost < best_cost)
                {
                    best_pi = pi;
                    best_cost = cost;
                    //cerr << "improved cost to " << cost <<  " by exchanging bins" << endl;
                    no_improvements = 0;
                }
                else
                {
                    no_improvements++;
                    //cout << "rand not improved" << endl;
                }
            }
            else
            {
                //cout << "rand discarded" << endl;
                no_improvements++;
            }
        }
        else //exchange jobgroups
        {
            if(pi[bin1].groups.size() > 1)
            {
                uniform_int_distribution<int> rand_group_pos(0, pi[bin1].groups.size()-1);

                int pos1 = rand_group_pos(generator);
                int pos2;
                do
                {
                    pos2 = rand_group_pos(generator);
                } while(pos2 == pos1);

                auto gain = lbs.calculate_group_exchange(bin1, pos1, pos2);
                if(rand(generator) < min(1.0, exp(gain / t(i++))))
                {
                    lbs.exchange_groups(bin1, pos1, pos2);

                    cost -= gain;
                    if(cost < best_cost)
                    {
                        best_pi = pi;
                        best_cost = cost;
                        //cerr << "improved cost to " << cost <<  " by exchanging groups" << endl;
                        no_improvements = 0;
                    }
                    else
                    {
                        no_improvements++;
                        //cout << "rand not improved" << endl;
                    }
                }
                else
                {
                    //cout << "rand discarded" << endl;
                    no_improvements++;
                }
            }
        }
    } while(no_improvements < 1000);

    return best_pi;
}


#endif
