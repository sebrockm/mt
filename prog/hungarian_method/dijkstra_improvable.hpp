#ifndef _DIJKSTRA_IMPROVABLE_HPP_
#define _DIJKSTRA_IMPROVABLE_HPP_

#include "matching.hpp"

#include <unordered_map>
#include <vector>
#include <memory>
#include <algorithm>
#include <boost/heap/fibonacci_heap.hpp>
#include <iostream>

using namespace std;
using namespace boost::heap;


class dijkstra_improvable
{
private:
    struct node_cmp
    {
        unordered_map<int, float>& distances;

        node_cmp(unordered_map<int, float>& d)
            :distances(d) {}

        bool operator () (int n1, int n2) const
        {
            return distances[n1] > distances[n2];
        }
    };

    unordered_map<int, bool> nodes_in_a;//true->a, false->b
    unordered_map<int, unordered_map<int, float>> adjacency_list;

public:
    template<class VectorIterator>
    dijkstra_improvable(VectorIterator b_first, VectorIterator b_last)
        :nodes_in_a(3*(b_last-b_first)), adjacency_list(3*(b_last-b_first))
    {
        while(b_first != b_last)
        {
            nodes_in_a[*b_first++] = false;
        }
    }

    //calculate shortest paths from <node> to all others
    const unordered_map<int, float>& add(int node, const unordered_map<int, float>& neighbors, matching& match, const unordered_map<int, float>& potential, int& znode)
    //const unordered_map<int, float> add(int node, const unordered_map<int, unordered_map<int, float>>& neighbors, matching& match, const unordered_map<int, float>& potential, int& znode)
    {
        unordered_map<int, float> nodes_distances(3*nodes_in_a.size()/2);

        //initialize new distances
        for(auto& p : nodes_in_a)
        {
            nodes_distances[p.first] = numeric_limits<float>::infinity();
        }
        nodes_distances[node] = 0;

        //arc costs are potentials of nodes minus normal arc costs
        adjacency_list[node] = neighbors;
        //adjacency_list = neighbors;
        for(auto& p : adjacency_list[node])
        {
            p.second = potential.at(node) + potential.at(p.first) - p.second;
            adjacency_list[p.first][node] = p.second;
        }
        nodes_in_a[node] = true;

        
        auto heap = fibonacci_heap<int, compare<node_cmp>>(node_cmp(nodes_distances));
        typedef decltype(heap)::handle_type handle_t;
        unordered_map<int, handle_t> handles(nodes_in_a.size()*3/2);

        handles[node] = heap.push(node);

        vector<int> finished_a;

        bool foundz = false;

        while(!heap.empty())
        {
            int n = heap.top();
            heap.pop();

            if(nodes_in_a[n])
            {
                finished_a.push_back(n);
            }

            float n_d = nodes_distances[n];
            
            //shortest path to n is calculated
            if(nodes_in_a[n]) 
            {
                auto found = find_if(finished_a.begin(), finished_a.end(),
                        [&] (int na) -> bool
                        {
                            return nodes_distances[na] + potential.at(na) <= n_d;
                        }
                );

                if(found != finished_a.end()) //there is a finished node in A with little distance+potential
                {
                    znode = *found;
                    foundz = true;
                    break;
                }
            }
            else //n is in B
            {
                if(!match.vertices[n]) //n is free
                {
                    znode = n;
                    foundz = true;
                    break;
                }
            }

            for(auto& p : adjacency_list[n]) //all <neighbor,distance> pairs of n
            {
                if(nodes_in_a.find(p.first) == nodes_in_a.end())
                    continue;
                //float cost = potential.at(n) + potential.at(p.first) - p.second;
                if(n_d + p.second < nodes_distances[p.first])
                //if(n_d + cost < nodes_distances[p.first])
                {
                    //nodes_distances[p.first] = n_d + cost;
                    nodes_distances[p.first] = n_d + p.second;
                    if(handles.find(p.first) == handles.end()) //not in handles, so not in heap
                    {
                        handles[p.first] = heap.push(p.first);
                    }
                    else //already in heap, so update it
                    {
                        heap.increase(handles[p.first]);
                    }
                }
            }
        }

        if(!foundz)
            cout << "not found z" << endl;

        //add new shortest distances as edges
        adjacency_list[node] = move(nodes_distances);
        for(auto& p : adjacency_list[node])
        {
            adjacency_list[p.first][node] = p.second;
        }

        return adjacency_list[node];
        //return nodes_distances;
    }
};


#endif
