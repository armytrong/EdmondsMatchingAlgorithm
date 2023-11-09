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
    auto optional_graph = DIMACSFileParser::create_graph(filename);
    if(not optional_graph){
        return 1;
    }
    auto graph = optional_graph.value();

    graph.greedy_matching();
    graph.outer_vertex_scan();
    std::cout << std::endl;

    DIMACSFileParser::output_matching(graph);
    std::cout << "Size of Node: " << sizeof(Graph::Node) << std::endl;
}
