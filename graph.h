//
// Created by jgier on 24.10.2023.
//

#ifndef EDMONDSMATCHINGALGORITHM_GRAPH_H
#define EDMONDSMATCHINGALGORITHM_GRAPH_H

#include<vector>
#include<optional>

class Graph {
public:
    using NodeId = unsigned;
    struct Node {
        explicit Node(NodeId id);
        NodeId const id;
        NodeId forest_neighbor;
        NodeId matching_neighbor;
        NodeId blossom_root;
        unsigned scanned;
        std::vector<NodeId> neighbors;
        std::vector<NodeId> nodes_in_tree;
        NodeId tree_root;
    };

    // Default graph methods
    /// add a number of nodes to the graph, automatically generating indices
    void add_nodes(NodeId num_nodes);
    /// add an edge between the nodes with id a and b
    void add_edge(NodeId a, NodeId b);

    [[nodiscard]] std::size_t num_nodes() const;

    // Matching algorithms
    /// Greedily find a maximal matching on the graph, stored in the matching neighbors of each node.
    void greedy_matching();
    /// Use Edmonds maximum matching algorithm to find a maximum matching on the graph.
    void outer_vertex_scan();
    /// Sanity check; check weather matching_neighbor(a) == b implies matching neighbor(b) == a for all nodes.
    [[nodiscard]] bool is_matching_legal() const;
private:
    // Methods for querying nodes
    NodeId& tree_root(NodeId id) { return nodes[id].tree_root;}
    std::vector<NodeId>& tree_nodes(NodeId id) { return nodes[id].nodes_in_tree; }

    NodeId& matching_neighbor(NodeId id);
    NodeId& forest_neighbor(NodeId id);
    NodeId& blossom_root(NodeId id);
    [[nodiscard]] NodeId matching_neighbor(NodeId id) const;
    [[nodiscard]] NodeId forest_neighbor(NodeId id) const;
    /// Find the blossom root of a certain node and also point all nodes on the path down to the actual root (see UnionFind)
    [[nodiscard]] NodeId find_blossom_root(NodeId node);

    // check node states
    [[nodiscard]] bool is_node_outer(NodeId x) const;
    [[nodiscard]] bool is_node_inner(NodeId x) const;
    [[nodiscard]] bool is_node_out_of_forest(NodeId x) const;

    // subroutines of Edmonds matching algorithms
    void neighbor_search(NodeId x_id);
    void grow_forest(NodeId x_id, NodeId y_id);
    void augment_matching(NodeId x_id, NodeId y_id);
    void shrink_blossom(NodeId x_id, NodeId y_id);
    void reset_tree(NodeId node_id);

    std::vector<NodeId> get_path(NodeId node);

    /// Find the first intersection between two paths that is its own blossom root
    std::optional<NodeId> get_first_path_intersection(
        std::vector<NodeId> const& path_1,
        std::vector<NodeId> const& path_2
    );
public:
    unsigned matching_size = 0;
    std::vector<Node> nodes;
};


#endif //EDMONDSMATCHINGALGORITHM_GRAPH_H
