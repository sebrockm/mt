#ifndef GG_H
#define GG_H

#include <vector>
#include <algorithm> //std::sort, std::max, std::min
#include <limits>
#include "DisjointSet.h"

using namespace std;

bool sort2dVector (const vector<int>& i,const vector<int>& j) { return (i[1]<j[1]); }

bool sortPair (pair<int,int> l, pair<int,int> r) { return l.first < r.first; }

DisjointSet::DisjointSet(int n)
  :jobs(vector<int>(n, 0)) 
{  
  count = n;
  for (int i = 1; i < n; i++) {
    jobs[i] = i;
  }
}

bool DisjointSet::sameComponent(unsigned a, unsigned b) {
  if (a >= jobs.size() || b >= jobs.size())
    return false;
  return find(a) == find(b);  
}

int DisjointSet::find(int a) {
  return jobs[a];
}

void DisjointSet::merge(int a, int b) {
  int a_index = find(a);
  int b_index = find(b);
  if (a_index != b_index) {
    for (unsigned i = 0; i < jobs.size(); i++) {
      if (jobs[i] == b_index) {
        jobs[i] = a_index;
      }
    }
    count--;
  }  
}

bool DisjointSet::stronglyConnected() {
  return count == 1;
}

int popSmallestValue(vector<int>& cost_matrix) {
  int minValue = cost_matrix[0];  
  int minValue_index = 0;
  for (unsigned i = 0; i < cost_matrix.size(); i++) {        
      if (cost_matrix[i] < minValue) {  
        minValue = cost_matrix[i];
        minValue_index = i;
      }      
  }   
  cost_matrix[minValue_index] = numeric_limits<int>::max();   
  return minValue_index;  
}

int alpha(int p, int q, int j) {
  if (p == j)
    return q;
  if (q == j)
    return p;
  return j;
}

int permutation(vector<int> groups, int num_arcs, int j) {
  int result = j;
  for (int i = num_arcs - 1; i >= 0; i--) {
    result = alpha(groups[i], groups[i] + 1, result);
  }
  return result;
}

vector<int> tsp_gg(vector<vector<int> > const &job_list, int dominant1, int dominant2) {  
  int num_jobs = job_list.size() + 1;
  int num_machines = 2;  
  vector<vector<int> > matrix(num_jobs, vector<int>(3, 0));
  matrix[0][0] = 0;
  matrix[0][1] = 0;
  DisjointSet ds(num_jobs);
  for (int i = 1; i < num_jobs; i++) {    
    matrix[i][0] = job_list[i - 1][dominant1];
    matrix[i][1] = job_list[i - 1][dominant2];    
    matrix[i][2] = i;
  }  
  // Schritt P1
  sort(matrix.begin(), matrix.end(), sort2dVector);   
  // Schritt P2 + P3
  vector<pair<int,int> > jobs_first_machine(num_jobs);
  for (int i = 0; i < num_jobs; i++) {
    jobs_first_machine[i] = make_pair(matrix[i][0], i);
  }
  sort(jobs_first_machine.begin(), jobs_first_machine.end(), sortPair);
  vector<vector<int> > sorted_matrix(num_jobs, vector<int>(num_machines + 2, 0));
  for (int i = 0; i < num_jobs; i++) {
    sorted_matrix[i][0] = matrix[i][1];
    sorted_matrix[i][1] = jobs_first_machine[i].first;
    sorted_matrix[i][2] = jobs_first_machine[i].second;      
  }  
  // Schritt P4
  for (int i = 1; i < num_jobs - 1; i++) {
    sorted_matrix[i][3] = max(0, min(sorted_matrix[i+1][0], sorted_matrix[i+1][1]) - max(sorted_matrix[i][0], sorted_matrix[i][1]));
  }   
  // Schritt S1
  sorted_matrix[num_jobs - 1][3] = numeric_limits<int>::max();  
  vector<vector<int> > adjacency_matrix(num_jobs, vector<int>(num_jobs, 0));  
  for (int i = 1; i < num_jobs; i++) {
    if (i != sorted_matrix[i][2]) {
      ds.merge(i, sorted_matrix[i][2]);      
    }
  }  
  // Schritt S2  
  vector<int> cost_matrix(num_jobs, 0);
  vector<int> added_arcs(num_jobs, 0); // Array fuer alle in Schritt S3 hinzugefuegten Kanten
  for (int i = 0; i < num_jobs; i++) {    
    cost_matrix[i] = sorted_matrix[i][3];
  }  
  int num_arcs = 0;
  while(!ds.stronglyConnected()) {     
    int smallestIndex;
    do {
      smallestIndex = popSmallestValue(cost_matrix);      
    } while (ds.sameComponent(smallestIndex, smallestIndex + 1));    
    //Schritt S3
    ds.merge(smallestIndex, smallestIndex + 1);    
    //Schritt T1
    added_arcs[smallestIndex] = sorted_matrix[smallestIndex][0] <= sorted_matrix[smallestIndex][1] ? 1 : 2; // Gruppe 1 oder 2    
    num_arcs++;
  }  
  // Schritt T2
  vector<int> groups(num_arcs, 0); 
  int j = 0;
  for (int i = num_jobs - 1; i >= 0; i--) { // hinten stehen die groessten Werte
    if (added_arcs[i] != 0 && added_arcs[i] == 1) {      
      groups[j] = i;        
      j++;
    }     
  }
  // Schritt T3
  for (int i = 0; i < num_jobs; i++) {
    if (added_arcs[i] != 0 && added_arcs[i] == 2) {      
      groups[j] = i;        
      j++;
    }
  }  
  // Schritt T4
  int next_job = 0;  
  vector<int> optimal_tour;  
  for (int i = 0; i < num_jobs - 1; i++) {   
    next_job = sorted_matrix[permutation(groups, num_arcs, next_job)][2];
    optimal_tour.push_back(matrix[next_job][2]);   
  } 
  return optimal_tour;
}

#endif
