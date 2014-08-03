#ifndef _SIMULATED_ANNEALING_HPP_
#define _SIMULATED_ANNEALING_HPP_

#include "neighborhood.hpp"
#include "nonfull_schedule.hpp"

#include <vector>
#include <random>

using namespace std;



vector<typename neighborhood::job> simulated_annealing(vector<neighborhood::job> pi, int cost)
{
    uniform_real_distribution<double> rand(0, 1);
    uniform_int_distribution<int> rand_pos(0, pi.size()-1);
    default_random_engine generator(time(0));

    vector<neighborhood::job> best_pi = pi;
    int best_cost = cost;
    auto t = [cost] (int i) -> double {return 50 * abs(cos(i/33.0)) / i;};

    int no_improvements = 0;
    int i = 1;
    do
    {
        int k1 = rand_pos(generator);
        int k2;
        do
        {
           k2 = rand_pos(generator);
        } while(k2 == k1);

        xch_neighborhood nbh(pi);
        int gain = nbh.gain(k1, k2);
        //cout << "gain=" << gain << " t(i)=" << t(i) << endl;
        
        if(rand(generator) < min(1.0, exp(gain / t(i++))))
        {
            nbh.make(k1, k2);

            cost -= gain;
            if(cost < best_cost)
            {
                best_pi = pi;
                best_cost = cost;
                //cout << "rand improved, " << no_improvements << endl;
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
    } while(no_improvements < 1000);

    return best_pi;
}


#endif
