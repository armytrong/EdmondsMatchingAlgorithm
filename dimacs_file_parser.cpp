//
// Created by jgier on 25.10.2023.
//

#include <cassert>
#include <sstream>
#include "dimacs_file_parser.h"

Graph DIMACSFileParser::create_graph(const std::string &filename) {
    std::ifstream file(filename);
    assert(file);
    return create_graph(file);
}



Graph DIMACSFileParser::create_graph(std::istream &file) {
    Graph result;

    std::string line;
    std::string line_type;
    unsigned num_nodes, num_edges;
    std::string edge_string;
    unsigned found_edges = 0;

    while(std::getline(file, line)){
        std::stringstream ss(line);
        ss >> line_type;
        switch (line_type[0]){
            case 'c':
                break;
            case 'p':
                ss >> edge_string;
                assert(edge_string == "edge");
                ss >> num_nodes >> num_edges;
                result.add_nodes(num_nodes);
                break;
            case 'e':
                if(found_edges >= num_edges){
                    throw std::runtime_error("To many edges.");
                }
                unsigned node_1, node_2;
                ss >> node_1 >> node_2;
                result.add_edge(node_1 - 1, node_2 - 1);
                found_edges++;
                break;
            default:
                throw; // TODO
        } 
    }
    return result;
}
