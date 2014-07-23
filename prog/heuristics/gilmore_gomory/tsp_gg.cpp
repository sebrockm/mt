#include "gg.h"
#include <time.h>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

void print_1dv(vector<int> & matrix) {
  for( std::vector<int>::const_iterator i = matrix.begin(); i != matrix.end(); ++i)
    cout << *i << ' ';
  cout << endl;
}


vector<vector<int> > readFile(string const &filename) {
  ifstream file(filename.c_str());
  string line;
  int i, j, n, num_jobs, num_machines;
  vector<vector<int> > matrix;
  i = 0;  
  while(getline(file, line)) {     
    if (!line.empty() && line[0] != '#') {
      istringstream is(line);
      j = 0;
      if (i == 0) {
        is >> num_machines >> num_jobs;      
        matrix = vector<vector<int> >(num_jobs, vector<int>(num_machines, 0)); //num_jobs Zeilen, num_machines Spalten
      } else {
        while( is >> n ) {
          matrix[i - 1][j] = n;
          j++;
        }
      }
      if(i == num_jobs)
          break;
      i++;
    }
  }
  file.close();
  //cout << "read" << endl;
  return matrix;
}

int calcTourCost(vector<int>::const_iterator first, vector<int>::const_iterator last, vector<vector<int> > const &job_list, int dominant1, int dominant2) {
  int next_job, cost1;
  int cost2 = 0;
  int total_cost = 0;   
  for (vector<int>::const_iterator cur = first; cur != last; ++cur) {
    next_job = *cur - 1;
    cost1 = job_list[next_job][dominant1];
    total_cost += max(cost1, cost2);     
    cost2 = job_list[next_job][dominant2];    
  }
  total_cost += cost2;
  return total_cost;
}



void solve_tsp(vector<vector<int> > const &job_list, int dominant1, int dominant2) {  
  vector<int> result = tsp_gg(job_list, dominant1, dominant2);   
  //print_1dv(result);    
  cout << "gg Cmax " << calcTourCost(result.begin(), result.end(), job_list, dominant1, dominant2) << endl;
}

int main(int argc, char** argv) {  
  /*int num_jobs = 80;
  vector<vector<int> > job_list(num_jobs, vector<int>(2, 0));
  srand ( time(NULL) );
  for (int i = 0; i < num_jobs; i++) {
    for (int j = 0; j < 2; j++) {      
        job_list[i][j] = rand() % 100 + 1;      
    }
  }
  solve_tsp(job_list, 0, 1, true);*/
    if(argc != 2)
    {
        cerr << "usage: " << argv[0] << " file" << endl;
        return 1;
    }
  vector<vector<int> > job_list = readFile(argv[1]);
  solve_tsp(job_list, 0, 1);
}
