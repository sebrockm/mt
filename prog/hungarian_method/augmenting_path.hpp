#ifndef _AUGMENTING_PATH_HPP_
#define _AUGMENTING_PATH_HPP_


#include "matching.hpp"

#include <vector>
#include <stack>


using namespace std;


vector<int> find_augmenting_path(const unordered_map<int, unordered_map<int, float>>& adjacency_list, matching& m, int start, int target,
        const unordered_map<int, float>& potential)
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
    //cout << "push " << start << endl;
    while(!next.empty())
    {
        int node = next.top();
        next.pop();
        //cout << "pop " << node << endl;
        for(auto& p : adjacency_list.at(node))
        {
            //cout << "try " << p.first << endl;
            if(!visited[p.first] && //not visited and 
                    m.edges[make_pair(node, p.first)] == next_node_match[node] && //next edge is matched iff required and
                    (p.first != target || m.vertices[p.first] == next_node_match[node])) //next node is saturated iff required
            {
                if(potential.at(node) + potential.at(p.first) - adjacency_list.at(node).at(p.first) != 0)
                {
                    //if(node == start)
                        //cout << "first "; 
                    //if(p.first == target)
                        //cout << "last ";
                    //cout << "edge " << node << "," << p.first << " in aug tree not tight" << endl;
                    if(p.first == target || node != start)
                    {
                        //cout << "skipped" << endl;
                        continue;
                    }
                }

                predecessors[p.first] = node;
                visited[p.first] = true;
                if(p.first == target)
                {
                    //cout << "found " << target << endl;
                    break;
                }

                next.push(p.first);
                //cout << "push " << p.first << endl;
                next_node_match[p.first] = !next_node_match[node];
            }
            else
            {
                //cout << "don't push " << p.first << " because " << !visited[p.first] << 
                    //(m.edges[make_pair(node, p.first)] == next_node_match[node]) <<
                    //(p.first != target || m.vertices[p.first] == next_node_match[node]) << endl;
            }
        }
    }

    if(visited[target])
    {
        vector<int> path;
        int v = target;
        while(v != start)
        {
            if(potential.at(v) + potential.at(predecessors[v]) - adjacency_list.at(v).at(predecessors[v]) != 0)
            {
                if(predecessors[v] == start)
                    cout << "first "; 
                if(v == target)
                    cout << "last ";
                cout << "edge " << predecessors[v] << "," << v << " in aug PATH not tight" << endl;
            }
            path.push_back(v);
            v = predecessors[v];
        }
        path.push_back(start);

        return path;
    }    

    return {};
}

#endif
