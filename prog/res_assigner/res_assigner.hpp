#ifndef _RES_ASSIGNER_HPP_
#define _RES_ASSIGNER_HPP_


#include <vector>
#include <limits>
#include <iostream>

using namespace std;


struct job
{
    int group_id;
    vector<int> times;
};

int calculate_exchange_cost(const vector<job>& cpi, int i, int j)
{
    if(i == j)
        return 0;

    //pi stays constant but will be changed intermediately
    vector<job>& pi = const_cast<vector<job>&>(cpi);

    if(i > j)
        swap(i, j);

    int n = pi.size();
    int m = pi[0].times.size();

    int cost_before = 0;
    int cost_after = 0;
        
    if(j - i >= m) //ranges do not cross
    {
        for(int t = i; i < min(i+m, n+m-1); ++t)
        {
            int maxc = 0;
            for(int c = max(t-n+1, 0); c < min(m, t+1); ++c)
            {
                maxc = max(maxc, pi[t-c].times[c]);
            }
            cost_before += maxc;
        }
        for(int t = j; j < min(j+m, n+m-1); ++t)
        {
            int maxc = 0;
            for(int c = max(t-n+1, 0); c < min(m, t+1); ++c)
            {
                maxc = max(maxc, pi[t-c].times[c]);
            }
            cost_before += maxc;
        }

        swap(pi[i], pi[j]);

        for(int t = i; i < min(i+m, n+m-1); ++t)
        {
            int maxc = 0;
            for(int c = max(t-n+1, 0); c < min(m, t+1); ++c)
            {
                maxc = max(maxc, pi[t-c].times[c]);
            }
            cost_after += maxc;
        }
        for(int t = j; j < min(j+m, n+m-1); ++t)
        {
            int maxc = 0;
            for(int c = max(t-n+1, 0); c < min(m, t+1); ++c)
            {
                maxc = max(maxc, pi[t-c].times[c]);
            }
            cost_after += maxc;
        }

        swap(pi[i], pi[j]);
    }
    else //ranges do cross
    {
        for(int t = i; i < min(j+m, n+m-1); ++t)
        {
            int maxc = 0;
            for(int c = max(t-n+1, 0); c < min(m, t+1); ++c)
            {
                maxc = max(maxc, pi[t-c].times[c]);
            }
            cost_before += maxc;
        }

        swap(pi[i], pi[j]);

        for(int t = j; j < min(i+m, n+m-1); ++t)
        {
            int maxc = 0;
            for(int c = max(t-n+1, 0); c < min(m, t+1); ++c)
            {
                maxc = max(maxc, pi[t-c].times[c]);
            }
            cost_after += maxc;
        }

        swap(pi[i], pi[j]);
    }

    return cost_after - cost_before;
}


int search_for_swap_pos(const vector<job>& pi, const vector<vector<int>> avail, int pos)
{
    int group_id = pi[pos].group_id;

    int swap_pos = -1; //found osition to swap with
    int cost = numeric_limits<int>::max(); //cost that this swap would cause
    int n = pi.size();
    int m = pi[0].times.size();

    //search all possible positions
    for(int i = 0; i < (int)pi.size(); ++i)
    {
        bool possible = true;
        for(int j = max(0, i-m+1); possible && j < min(n, i+m); ++j)
        {
            //check whether i could take pos's position
            if(avail[j][group_id] <= 0)
                possible = false;
        }
        for(int j = max(0, pos-m+1); possible && j < min(n, pos+m); ++j)
        {
            //check whether pos could take i's position
            if(avail[j][pi[i].group_id] <= 0)
                possible = false;
        }

        if(possible) //calculate cost
        {
            int tmp_cost = calculate_exchange_cost(pi, pos, i);            
            if(tmp_cost < cost) //choose the lowest cost
            {
                swap_pos = i;
                cost = tmp_cost;
            }
        }
    }

    return swap_pos;
}


//returns wether a feasible assignement of resources could be found
//maybe pi is changed 
bool assign_resources(vector<job>& pi, const vector<int>& resource_map)
{
    int n = pi.size();
    int m = pi[0].times.size();

    vector<vector<int>> available_res_matrix(n, resource_map);
    
    for(int i = 0; i < (int)pi.size(); ++i)
    {
        //decrease available resources of jobs surrounding i
        for(int j = max(0, i-m+1); j < min(n, i+m); ++j)
        {
            if(--available_res_matrix[j][pi[i].group_id] < 0)
            {
                //undo
                for(int k = max(0,i-m+1); k <= j; ++k)
                {
                    ++available_res_matrix[j][pi[j].group_id];
                }

                //search a job in pi to be swapped with i
                int swap_pos = search_for_swap_pos(pi, available_res_matrix, i);
                if(swap_pos < 0) //no position found
                {
                    return false;
                }

                swap(pi[i], pi[swap_pos]);

                // start from the beginning because the matrix may have changed completely
                i = 0; 
                available_res_matrix = vector<vector<int>>(n, resource_map);
                break;
            }
        }
    }

    return true;
}




#endif
