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
    unordered_map<int, bool> vertices; //false means free

    matching(int num_vertices = 0)
        :edges(num_vertices*num_vertices*3/2), vertices(3*num_vertices) {}

    template <class VertexIterator>
    void add_path(VertexIterator first, VertexIterator last)
    {
        //first and last vertex will switch their freeness
        if(first + 1 < last)
        {
            vertices[*first] ^= true;
            vertices[*(last-1)] ^= true;

            //every edge will switch its matchedness
            while(first + 1 != last)
            {
                edges[make_pair(*first, *(first+1))] = edges[make_pair(*(first+1), *first)] ^= true;
                ++first;
            }
        }
    }
};


#endif
