//
// Created by jgier on 24.10.2023.
//

#ifndef EDMONDSMATCHINGALGORITHM_GRAPH_H
#define EDMONDSMATCHINGALGORITHM_GRAPH_H

#include<vector>
#include<optional>

class Graph {
public:
    using NodeId = unsigned long;
    struct Node {
        explicit Node(NodeId id) : id(id), forest_neighbor(id), matching_neighbor(id), blossom_root(id), scanned(false){}
        NodeId const id;
        NodeId forest_neighbor;
        NodeId matching_neighbor;
        NodeId blossom_root;
        bool scanned;
        std::vector<NodeId> neighbors;
    };
    std::size_t num_nodes() {return nodes.size();}

    void add_node(std::vector<NodeId> neighbors);
    void add_nodes(NodeId num_nodes);
    void add_edge(NodeId a, NodeId b);

    void greedy_matching();
    void outer_vertex_scan();
private:

    NodeId& matching_neighbor(NodeId id);
    NodeId& forest_neighbor(NodeId id);
    NodeId& blossom_root(NodeId id);
    [[nodiscard]] NodeId matching_neighbor(NodeId id) const;
    [[nodiscard]] NodeId forest_neighbor(NodeId id) const;
    [[nodiscard]] NodeId find_blossom_root(NodeId node) ;

    void neighbor_search(NodeId x_id);
    void augment_matching(NodeId x_id, NodeId y_id);
    void shrink_blossom(NodeId x_id, NodeId y_id);

    [[nodiscard]] bool is_node_outer(NodeId x) const;
    [[nodiscard]] bool is_node_inner(NodeId x) const;
    [[nodiscard]] bool is_node_out_of_forest(NodeId x) const;

    NodeId find_path_root(NodeId node);
    std::vector<NodeId> get_path(NodeId node);

    std::optional<NodeId>
    get_first_path_intersection(std::vector<NodeId> const& path_1, std::vector<NodeId> const& path_2);

public:
    unsigned matching_size = 0;
    std::vector<Node> nodes;
};


#endif //EDMONDSMATCHINGALGORITHM_GRAPH_H
