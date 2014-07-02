#ifndef _MATCHING_HPP_
#define _MATCHING_HPP_


#include <unordered_map>
#include <boost/functional/hash.hpp>


using namespace std;


class matching
{
public:
    typedef pair<int, int> edge;

    unordered_map<edge, bool, boost::hash<edge>> edges;
    unordered_map<int, bool> vertices;

    matching(int num_vertices = 0)
        :edges(num_vertices*num_vertices*3/4), vertices(3*num_vertices) {}

    template <class EdgeIterator>
    void add_path(EdgeIterator first, EdgeIterator last)
    {
        //first and last vertex will switch their freeness
        if(first != last)
        {
            vertices[first->first] ^= true;

            if(first != last - 1)
            {
                vertices[(last-1)->second] ^= true;
            }
        }

        //every edge will switch its matchedness
        while(first != last)
        {
            edges[*first] ^= true;
            ++first;
        }
    }
};


#endif
