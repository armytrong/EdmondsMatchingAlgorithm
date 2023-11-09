//
// Created by jgier on 25.10.2023.
//

#include <cassert>
#include <sstream>
#include <iostream>
#include "dimacs_file_parser.h"

std::optional<Graph> DIMACSFileParser::create_graph(std::string const& filename) {
    std::ifstream file(filename);
    if(not file){
        std::cerr << "Could not open " << filename << std::endl;
        return std::nullopt;
    }
    return create_graph(file);
}



std::optional<Graph> DIMACSFileParser::create_graph(std::istream &file) {
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
            case 'c': // Comments
                break;
            case 'p': // Graph header
                ss >> edge_string;
                if(edge_string != "edge"){
                    std::cerr << "WARNING: input file does not follow sepcifications, "
                              << edge_string << " != 'edge'!." << std::endl;
                }
                ss >> num_nodes >> num_edges;
                result.add_nodes(num_nodes);
                break;
            case 'e': // Edge specification
                if(found_edges >= num_edges){
                    std::cerr << "WARNING: Too many edges. Continuing anyway." << std::endl;
                }
                unsigned node_1, node_2;
                ss >> node_1 >> node_2; // Nodes in DIMACS are indexed starting at one. This program is 0 indexed.
                if(node_1 >= result.num_nodes()){
                    std::cerr << "ERROR: Node " << node_1 << " is not in the graph. Aborting." << std::endl;
                    return std::nullopt;
                }
                if(node_2 >= result.num_nodes()){
                    std::cerr << "ERROR: Node " << node_2 << " is not in the graph. Aborting." << std::endl;
                }
                result.add_edge(node_1 - 1, node_2 - 1);
                found_edges++;
                break;
            default:
                std::cerr << "WARNING incorrect file syntax: '" << line_type[0] << "' is not a recognized line header. "
                          << "Continuing anyway." << std::endl;
        }
        if(found_edges < num_edges){
            std::cerr << "WARNING: Less edges found than specified. Continuing anyway." << std::endl;
        }
    }
    return result;
}

void DIMACSFileParser::output_matching(const Graph &graph) {
    if (not graph.is_matching_legal()){
        std::cerr << "An error occured during matching generation. The result is not a matching. "
                  << "No result will be returned." << std::endl;
        return;
    }
    std::cout << "p edge " << graph.num_nodes() << " " << graph.matching_size << std::endl;
    for(auto const& node: graph.nodes){
        if(node.matching_neighbor > node.id){
            std::cout << "e " << node.id + 1 << " " << node.matching_neighbor + 1<< "\n";
        }
    }
    std::cout << "c matching size: " << graph.matching_size << std::endl;
}
