//
// Created by jgier on 25.10.2023.
//

#include <iostream>
#include "graph.h"
#include "dimacs_file_parser.h"

int main(int argc, char** argv){

    if(argc < 2){
        std::cerr << "Not enough arguments. Usage: " << argv[0] << " " << "<graph_file>" << std::endl;
        return 1;
    }
    std::string filename = argv[1];
    std::ifstream file(filename);
    if(not file){
        std::cerr << "Could not open " << filename << std::endl;
        return -1;
    }
    auto g = DIMACSFileParser::create_graph(file);

    g.greedy_matching();
    g.outer_vertex_scan();
    std::cout << std::endl;

    std::cout << "p edge " << g.num_nodes() << " " << g.matching_size << std::endl;
    for(auto const& node: g.nodes){
        if(node.matching_neighbor != node.id){
            if (g.nodes[node.matching_neighbor].matching_neighbor != node.id){
                std::cerr << "node " << node.id + 1 << " has matching neighbor " << node.matching_neighbor + 1<< ", "
                    << "but " << node.matching_neighbor + 1 << " has matching neighbor " << g.nodes[node
                    .matching_neighbor].matching_neighbor + 1<< std::endl;
            }
        }
        if(node.matching_neighbor > node.id){
            std::cout << "e " << node.id + 1 << " " << node.matching_neighbor + 1<< "\n";
        }
    }
    std::cout << "c matching size: " << g.matching_size << std::endl;
}
