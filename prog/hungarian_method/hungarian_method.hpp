#ifndef _HUNGARIAN_METHOD_HPP_
#define _HUNGARIAN_METHOD_HPP_


#include <boost/graph/dijkstra_shortest_paths_no_color_map.hpp>
#include <boost/graph/breadth_first_search.hpp>
//#include <boost/graph/bipartite.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/graph_traits.hpp>
#include <vector>


namespace boost {
/*
template<
    class Graph,
    class WeightMap,
    class MateMap,
    class VertexIndexMap
>
void hungarian_method(const Graph& g, 
        WeightMap weight_map, 
        MateMap mate_map,
        VertexIndexMap vertex_index_map = get(vertex_index, g))
{
    std::vector<default_color_type> partition_map(num_vertices(g));

    bool bipartite = is_bipartite(g, vertex_index_map, partition_map);

    if(!bipartite)
        throw "not bipartite";

    hungarian_method(g, weight_map, mate_map, partition_map, vertex_index_map);
}
*/

template<
    class Graph,
    class WeightMap,
    class MateMap,
    class PartitionMap,
    class VertexIndexMap
>
void hungarian_method(const Graph& g, 
        WeightMap weight_map, 
        MateMap mate_map,
        PartitionMap partition_map,
        VertexIndexMap vertex_index_map)
{
    typedef typename graph_traits<Graph>::vertex_iterator vit;
    typedef typename graph_traits<Graph>::out_edge_iterator eit;
    typedef typename property_traits<WeightMap>::type weight_t;

    std::vector<bool> done(num_vertices(g));
    std::vector<weight_t> pi(num_vertices(g));
    
    vit v, vend;
    for(tie(v, vend) = vertices(g); v != vend; ++v)
    {
        if(partition_map[*v] == white)
        {
            done[vertex_index_map[*v]] = true;
            pi[vertex_index_map[*v]] = weight_t();
        }
    }

    for(tie(v, vend) = vertices(g); v != vend; ++v)
    {
        //main algo
        if(partition_map[*v] == black)
        {
            auto id = vertex_index_map[*v];
            done[id] = true;
            pi[id] = weight_t();

            eit e, eend;
            for(tie(e, eend) = out_edges(*v, g); e != eend; ++e)
            {
                pi[id] = std::max(pi[id], weight_map[*e] - pi[vertex_index_map[target(*e, g)]]);
            }

            //dijkstra
        }
    }
}


} // end namespace boost


#endif
