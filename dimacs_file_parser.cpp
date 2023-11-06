//
// Created by jgier on 25.10.2023.
//

#include <cassert>
#include <sstream>
#include <iostream>
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

void DIMACSFileParser::output_matching(const Graph &graph) {

    std::cout << "p edge " << graph.num_nodes() << " " << graph.matching_size << std::endl;
    for(auto const& node: graph.nodes){
        if(node.matching_neighbor != node.id){
            if (graph.nodes[node.matching_neighbor].matching_neighbor != node.id){
                std::cerr << "node " << node.id + 1 << " has matching neighbor " << node.matching_neighbor + 1<< ", "
                          << "but " << node.matching_neighbor + 1 << " has matching neighbor " << graph.nodes[node
                        .matching_neighbor].matching_neighbor + 1<< std::endl;
            }
        }
        if(node.matching_neighbor > node.id){
            std::cout << "e " << node.id + 1 << " " << node.matching_neighbor + 1<< "\n";
        }
    }
    std::cout << "c matching size: " << graph.matching_size << std::endl;
}
