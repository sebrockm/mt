#ifndef _GREEDY_BIN_SEQUENCE_HPP_
#define _GREEDY_BIN_SEQUENCE_HPP_


#include "local_2opt.hpp"

#include <vector>
#include <algorithm>


using namespace std;


//greedyly calculates a bin sequence using locally optimal pairs of bins
//returns a sequence of the bins' ids
vector<unsigned> greedy_bin_sequence(const vector<bin>& bins)
{
    int n = bins.size();
    vector<vector<double>> matrix(n, vector<double>(n));

    double best = numeric_limits<double>::infinity();
    int best_i = -1;
    int best_j = -1;

    //calculate all possibilities and save the best of them
    for(int i = 0; i < n; ++i)
    {
        for(int j = 0; j < n; ++j)
        {
            if(i == j)
            {
                //store a big value to ease the search afterwards
                matrix[i][j] = numeric_limits<double>::max();
            }
            else
            {
                matrix[i][j] = calculate_opt(bins[i], bins[j]);
                if(matrix[i][j] < best)
                {
                    best = matrix[i][j];
                    best_i = i;
                    best_j = j;
                }
            }
        }
    }

    vector<unsigned> result(n);
    result[0] = bins[best_i].id;
    result[1] = bins[best_j].id;

    for(int i = 2; i < n; ++i)
    {
        int id = -1;
        double best = numeric_limits<double>::max();
        for(int j = 0; j < n; ++j)
        {
            //make sure, j is not already in the result vector
            bool j_valid = true;
            for(int k = 0; k < i; ++k)
            {
                if(bins[j].id == result[k])
                {
                    j_valid = false;
                    break;
                }
            }

            if(j_valid)
            {
                //find the i-th bin, neighbor of i-1
                if(matrix[i-1][j] < best)
                {
                    best = matrix[i-1][j];
                    id = j;
                }
            }
        }

        result[i] = bins[id].id;
    }

    return result;
}



#endif
