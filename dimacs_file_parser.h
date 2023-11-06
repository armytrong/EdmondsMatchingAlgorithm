//
// Created by jgier on 25.10.2023.
//

#ifndef EDMONDSMATCHINGALGORITHM_DIMACS_FILE_PARSER_H
#define EDMONDSMATCHINGALGORITHM_DIMACS_FILE_PARSER_H

#include<string>
#include<fstream>
#include "graph.h"
class DIMACSFileParser {
public:
    static Graph create_graph(std::string const& filename);
    static Graph create_graph(std::istream & file);

    static void output_matching(Graph const& graph);
};


#endif //EDMONDSMATCHINGALGORITHM_DIMACS_FILE_PARSER_H
