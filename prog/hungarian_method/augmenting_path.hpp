#ifndef _AUGMENTING_PATH_HPP_
#define _AUGMENTING_PATH_HPP_


#include "matching.hpp"

#include <vector>
#include <stack>


using namespace std;


vector<int> find_augmenting_path(unordered_map<int, unordered_map<int, float>>& adjacency_list, matching& m, int start, int target)
{
    if(start == target)
    {
        return {};
    }

    unordered_map<int, int> predecessors;
    unordered_map<int, bool> visited(adjacency_list.size()*3/2);
    unordered_map<int, bool> next_node_match;
    stack<int> next;
    visited[start] = true;
    
    next_node_match[start] = m.vertices[start];

    next.push(start);
    while(!next.empty())
    {
        int node = next.top();
        next.pop();
        for(auto& p : adjacency_list[node])
        {
            if(!visited[p.first] && //not visited and 
                    m.edges[make_pair(node, p.first)] == next_node_match[node] && //next edge is matched iff required and
                    m.vertices[p.first] == next_node_match[node]) //next node is saturated iff required
            {
                predecessors[p.first] = node;
                visited[p.first] = true;
                if(p.first == target)
                {
                    break;
                }

                next.push(p.first);
                next_node_match[p.first] = !next_node_match[node];
            }
        }
    }

    if(visited[target])
    {
        vector<int> path;
        int v = target;
        while(v != start)
        {
            path.push_back(v);
            v = predecessors[v];
        }
        path.push_back(start);

        return path;
    }    

    return {};
}

#endif
