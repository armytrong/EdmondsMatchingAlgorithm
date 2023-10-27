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

    for(auto const& node: g.nodes){
        std::cout << "matching_neighbor(" << node.id << ") = " << node.matching_neighbor << std::endl;
    }
}
