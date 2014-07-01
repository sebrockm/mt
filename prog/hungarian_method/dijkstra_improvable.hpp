#ifndef _DIJKSTRA_IMPROVABLE_HPP_
#define _DIJKSTRA_IMPROVABLE_HPP_


#include <unordered_map>
#include <vector>
#include <memory>
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

    vector<int> nodes_a;
    const vector<int>& nodes_b;
    unordered_map<int, unordered_map<int, float>> adjacency_list;

public:
    dijkstra_improvable(const vector<int>& nodes_b)
        :nodes_a(), nodes_b(nodes_b), adjacency_list(3*nodes_b.size())
    {
        nodes_a.reserve(nodes_b.size()); 
    }

    //calculate shortest paths from <node> to all others
    const unordered_map<int, float>& add(int node, unordered_map<int, float>&& neighbors)
    {
        unordered_map<int, float> nodes_distances(3*(nodes_a.size()+nodes_b.size())/2);

        //initialize new distances
        for(int n : nodes_b)
        {
            nodes_distances[n] = numeric_limits<float>::infinity();
        }
        for(int n : nodes_a)
        {
            nodes_distances[n] = numeric_limits<float>::infinity();
        }
        nodes_distances[node] = 0;

        adjacency_list[node] = move(neighbors);
        for(auto& p : adjacency_list[node])
        {
            adjacency_list[p.first][node] = p.second;
        }
        nodes_a.push_back(node);

        
        auto heap = fibonacci_heap<int, compare<node_cmp>>(node_cmp(nodes_distances));
        typedef decltype(heap)::handle_type handle_t;
        unordered_map<int, handle_t> handles((nodes_a.size() + nodes_b.size())*3/2);

        handles[node] = heap.push(node);

        while(!heap.empty())
        {
            int n = heap.top();
            heap.pop();

            float n_d = nodes_distances[n];

            for(auto& p : adjacency_list[n]) //all <neighbor,distance> pairs of n
            {
                if(n_d + p.second < nodes_distances[p.first])
                {
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

        //add new shortest distances as edges
        adjacency_list[node] = move(nodes_distances);
        for(auto& p : adjacency_list[node])
        {
            adjacency_list[p.first][node] = p.second;
        }

        return adjacency_list[node];
    }
};


#endif
