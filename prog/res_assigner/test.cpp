#include "res_assigner.hpp"
#include <iostream>

using namespace std;

int main()
{
    vector<job> pi(4);
    pi[0].group_id = 0;
    pi[1].group_id = 1;
    pi[2].group_id = 1;
    pi[3].group_id = 1;

    pi[0].times = {1,2,3};
    pi[1].times = {2,3,1};
    pi[2].times = {3,1,2};
    pi[3].times = {1,2,3};

    vector<int> res = {1,2};

    cout << assign_resources(pi, res) << endl;

}
