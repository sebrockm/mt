#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/property_map/property_map.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>


using namespace std;
using namespace boost;

void unique_maker(vector<vector<double>>& v)
{
    constexpr double e = 0.0001;
    std::unordered_map<double, int> m;
    for(auto& vv : v)
    {
        for(double& d : vv)
        {
            double offset = d*m[d]*e;
            m[d]++;
            d += offset;
        }
    }
}

int main()
{
    srand(time(0));
    //test instance
    //vector<int> left_sizes = {3, 2, 3, 4, 9, 2, 1};
    //vector<int> right_sizes = {4, 1, 2, 4, 3, 10};
    vector<int> left_sizes = {5,5,7,2};
    vector<int> right_sizes = {5,2,4,4,2,2};
    assert(accumulate(left_sizes.begin(),left_sizes.end(),0)==accumulate(right_sizes.begin(),right_sizes.end(),0));
    vector<vector<double>> costs(left_sizes.size(), vector<double>(right_sizes.size()));
    for(auto& v : costs)
    {
        generate(v.begin(), v.end(), [](){return rand()%9+1;});
    }
    unique_maker(costs);

    for(auto& v : costs)
    {
        for(double d : v)
            cout << d << " ";
        cout << endl;
    }


    struct edge_properties;
    struct vertex_properties;

    typedef adjacency_list<vecS, vecS, bidirectionalS, vertex_properties, edge_properties> graph;
    typedef typename graph_traits<graph>::edge_descriptor edge;
    typedef typename graph_traits<graph>::vertex_descriptor vertex;
    struct edge_properties
    {
        int capacity;
        int residual_capacity;
        edge reverse;
        double weight;
        edge_properties()
            :capacity(0), residual_capacity(0), reverse(), weight(0) {}
    };
    struct vertex_properties
    {
        string name;
    };

    graph g(2+left_sizes.size()+right_sizes.size());

    //function to add edge with capacity, weight and reverse edge at once
    auto add_edge = [&g] (vertex s, vertex t, int cap, double w)
    {
        auto e = boost::add_edge(s, t, g).first;
        auto er = boost::add_edge(t, s, g).first;
        g[e].capacity = cap;
        g[e].weight = w;
        g[er].weight = -w;
        g[e].reverse = er;
        g[er].reverse = e;
    };

    //add stuff
    auto source = add_vertex(g);
    auto target = add_vertex(g);
    g[source].name = "source";
    g[target].name = "target";
    
    vector<vertex> left_jobs(left_sizes.size());
    vector<vertex> right_jobs(right_sizes.size());

    vector<string> names(right_sizes.size()+left_sizes.size());
    int i = 0;
    generate(names.begin(), names.end(), [&i](){return string({(char)('A'+i++)});});
    
    //add job groups in left bin and edges to and from source
    for(unsigned i = 0; i < left_jobs.size(); ++i)
    {
        left_jobs[i] = add_vertex(g);
        add_edge(source, left_jobs[i], left_sizes[i], 1);
        g[left_jobs[i]].name = names[i];
    }
    
    //add job groups in right bin and edges to and from target
    for(unsigned i = 0; i < right_jobs.size(); ++i)
    {
        right_jobs[i] = add_vertex(g);
        add_edge(right_jobs[i], target, right_sizes[i], 1);
        g[right_jobs[i]].name = names[left_jobs.size()+i];
    }

    //add edges between job groups
    for(unsigned i = 0; i < left_jobs.size(); ++i)
    {
        for(unsigned j = 0; j < right_jobs.size(); ++j)
        {
            add_edge(left_jobs[i], right_jobs[j], min(left_sizes[i], right_sizes[j]), costs[i][j]);
        }
    }

    successive_shortest_path_nonnegative_weights(g, source, target,
            weight_map(get(&edge_properties::weight, g)).
            capacity_map(get(&edge_properties::capacity, g)).
            residual_capacity_map(get(&edge_properties::residual_capacity, g)).
            reverse_edge_map(get(&edge_properties::reverse, g)));

    for(auto p = edges(g); p.first != p.second; ++p.first)
    {
        if(g[*p.first].weight >= 0)
        {
            auto from = boost::source(*p.first, g);
            auto to = boost::target(*p.first, g);
            if(from != target && from != source && to != target && to != source)
            {
                cout << g[from].name << "->" << g[to].name << ": " << g[*p.first].capacity - g[*p.first].residual_capacity << " w=" << g[*p.first].weight << endl;
            }
        }
    }
}
