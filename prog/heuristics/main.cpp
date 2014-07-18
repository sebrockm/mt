#include "nonfull_schedule.hpp"

#include <vector>
#include <iostream>


using namespace std;


int main()
{
    typedef typename nonfull_schedule<3, int>::job job;

    vector<job> jobs = 
    {
        {3,5,1},
        {5,6,8},
        {4,9,1},
        {4,3,3},
        {6,8,1},
        {6,5,9},
        {3,5,1},
        {1,5,8}
    };

    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < i; ++j)
        {
            cout << " ";
        }
        for(auto& j : jobs)
        {
            cout << j[i];
        }
        cout << endl;
    }

    jobs = create_schedule<3, int>(jobs);

    
    cout << endl;
    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < i; ++j)
        {
            cout << " ";
        }
        for(auto& j : jobs)
        {
            cout << j[i];
        }
        cout << endl;
    }
}
