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
        explicit Node(NodeId id) : id(id), phi(id), mu(id), rho(id), scanned(false){}
        NodeId const id;
        NodeId phi;
        NodeId mu;
        NodeId rho;
        bool scanned;
        std::vector<NodeId> neighbors;
    };
    std::size_t num_nodes() {return nodes.size();}

    void add_node(std::vector<NodeId> neighbors);
    void add_nodes(NodeId num_nodes);
    void add_edge(NodeId a, NodeId b);

    void outer_vertex_scan();
private:

    NodeId& mu(NodeId id);
    NodeId& phi(NodeId id);
    NodeId& rho(NodeId id);
    [[nodiscard]] NodeId mu(NodeId id) const;
    [[nodiscard]] NodeId phi(NodeId id) const;
    [[nodiscard]] NodeId rho(NodeId id) const;

    void neighbor_search(NodeId x_id);
    void augment_matching(NodeId x_id, NodeId y_id);
    void shrink_blossom(NodeId x_id, NodeId y_id);

    [[nodiscard]] bool is_node_outer(NodeId x) const;
    [[nodiscard]] bool is_node_inner(NodeId x) const;
    [[nodiscard]] bool is_node_out_of_forest(NodeId x) const;

    std::optional<NodeId> find_unscanned_outer_vertex();

    NodeId find_path_root(NodeId node);
    std::vector<NodeId> get_path(NodeId node);

    std::optional<NodeId>
    get_first_path_intersection(std::vector<NodeId> const& path_1, std::vector<NodeId> const& path_2);

public:

    std::vector<Node> nodes;
};


#endif //EDMONDSMATCHINGALGORITHM_GRAPH_H
