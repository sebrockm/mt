#include <string>
#include <iostream>
#include <vector>


using namespace std;

int main(int argc, char** argv)
{
    srand(time(0));

    int n = stoi(argv[1]);
    int r = stoi(argv[2]);
    int m = stoi(argv[3]);

    cout << m << " " << n << endl;

    vector<int> t(m);
    for(int i = 0; i < m; ++i)
    {
        t[i] = rand()%9 + 1;
    }

    cerr << "group of size " << n/2 << endl;
    for(int i = 0; i < n/2; ++i)
    {
        for(int j : t)
        {
            cout << j << " ";
        }
        cout << endl;
    }

    int groupsize = (n-n/2) / (r-1);
    int gplus1 = (n-n/2) % (r-1);

    for(int i = 0; i < r-1; ++i)
    {
        for(int j = 0; j < m; ++j)
        {
            t[j] = rand()%9 + 1;
        }

        cerr << "group of size " << groupsize + (i<gplus1) << endl;
        for(int j = 0; j < groupsize + (i<gplus1); ++j)
        {
            for(int k : t)
            {
                cout << k << " ";
            }
            cout << endl;
        }
    }
}
