//
// Created by jgier on 25.10.2023.
//

#include <iostream>
#include "graph.h"

int main(){
    Graph g;
    g.add_nodes(10);
    g.add_edge(0,1);
    g.add_edge(1,2);
    g.add_edge(2,3);
    g.add_edge(3,4);
    g.add_edge(4,0);
    g.add_edge(5,6);
    g.add_edge(6,7);
    g.add_edge(7,8);
    g.add_edge(8,9);
    g.add_edge(9,5);
    g.add_edge(0,5);
    g.add_edge(1,6);
    g.add_edge(2,7);
    g.add_edge(3,8);
    g.add_edge(4,9);
    for(auto const& node: g.nodes){
        std::cout << "neighbors of " << node.id << ": ";
        for(auto const neighbor : node.neighbors){
            std::cout << " " << neighbor;
        }
        std::cout << std::endl;
    }
    g.outer_vertex_scan();
    std::cout << std::endl;
    for(auto const& node: g.nodes){
        std::cout << "mu(" << node.id << ") = " << node.mu << std::endl;
    }
}
