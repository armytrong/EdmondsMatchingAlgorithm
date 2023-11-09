//
// Created by jgier on 24.10.2023.
//

#include "graph.h"
#include <algorithm>
#include <cassert>
#include <iostream>

void Graph::add_edge(Graph::NodeId a, Graph::NodeId b) {
    if (not std::ranges::count(nodes[a].neighbors, b)) {
        nodes[a].neighbors.push_back(b);
    }
    if (not std::ranges::count(nodes[b].neighbors, a)) {
        nodes[b].neighbors.push_back(a);
    }
}

void Graph::add_nodes(Graph::NodeId num_nodes) {
    for (NodeId i = 0; i < num_nodes; i++) {
        nodes.emplace_back(nodes.size());
    }
}

Graph::NodeId &Graph::matching_neighbor(NodeId id) {
    return nodes[id].matching_neighbor;
}

Graph::NodeId Graph::matching_neighbor(Graph::NodeId id) const {
    return nodes[id].matching_neighbor;
}

Graph::NodeId &Graph::forest_neighbor(NodeId id) {
    return nodes[id].forest_neighbor;
}

Graph::NodeId Graph::forest_neighbor(Graph::NodeId id) const {
    return nodes[id].forest_neighbor;
}

Graph::NodeId &Graph::blossom_root(NodeId id) {
    return nodes[id].blossom_root;
}

Graph::NodeId Graph::find_blossom_root(NodeId node)  {
    std::vector<NodeId> found_nodes;
    while(blossom_root(node) != node){
        found_nodes.push_back(node);
        node = blossom_root(node);
    }
    for(auto other_node : found_nodes){
        blossom_root(other_node) = node;
    }
    return node;
}

void Graph::outer_vertex_scan() {
    std::vector<NodeId> node_ids;
    for(NodeId i = 0; i < num_nodes(); i++){
        node_ids.push_back(i);
    }
    bool loop = true;
    while(loop){
        loop = false;
        for(auto node_id : node_ids){
            if(not is_node_outer(node_id) or nodes[node_id].scanned == matching_size + 1){continue;}
            loop = true;
            neighbor_search(node_id);
            nodes[node_id].scanned = matching_size + 1;
        }
    }
    std::cout << std::endl;
}

void Graph::neighbor_search(Graph::NodeId x_id) {
    for (auto y_id: nodes[x_id].neighbors) {
        if (not(
            is_node_out_of_forest(y_id) or
            (is_node_outer(y_id) and find_blossom_root(y_id) != find_blossom_root(x_id))
        )) {
            continue;
        }
        if (is_node_out_of_forest(y_id)) {
            grow_forest(x_id, y_id);
        } else if (get_path(x_id).back() != get_path(y_id).back()) {
            augment_matching(x_id, y_id);
            break;
        } else {
            shrink_blossom(x_id, y_id);
        }
    }
}

void Graph::grow_forest(NodeId x_id, NodeId y_id) {
    forest_neighbor(y_id) = x_id;
    tree_root(y_id) = tree_root(x_id);
    tree_root(matching_neighbor(y_id)) = tree_root(x_id);
    tree_nodes(tree_root(x_id)).push_back(y_id);
    tree_nodes(tree_root(x_id)).push_back(matching_neighbor(y_id));
}
bool Graph::is_node_outer(Graph::NodeId x) const {
    return (matching_neighbor(x) == x) or
           (forest_neighbor(matching_neighbor(x)) != matching_neighbor(x));
}

bool Graph::is_node_inner(Graph::NodeId x) const {
    return (forest_neighbor(matching_neighbor(x)) == matching_neighbor(x) and
            (forest_neighbor(x) != x));
}

bool Graph::is_node_out_of_forest(Graph::NodeId x) const {
    return (matching_neighbor(x) != x) and
           (forest_neighbor(x) == x) and
           (forest_neighbor(matching_neighbor(x)) == matching_neighbor(x));
}

void Graph::augment_matching(Graph::NodeId x_id, Graph::NodeId y_id) {
    auto const x_path = get_path(x_id);
    auto const y_path = get_path(y_id);
    for (size_t i = 1; i < x_path.size(); i += 2) {
        auto v_id = x_path[i];
        matching_neighbor(forest_neighbor(v_id)) = v_id;
        matching_neighbor(v_id) = forest_neighbor(v_id);
    }
    for (size_t i = 1; i < y_path.size(); i += 2) {
        auto v_id = y_path[i];
        matching_neighbor(forest_neighbor(v_id)) = v_id;
        matching_neighbor(v_id) = forest_neighbor(v_id);
    }
    matching_neighbor(x_id) = y_id;
    matching_neighbor(y_id) = x_id;
    reset_tree(x_id);
    reset_tree(y_id);
    matching_size += 1;
    std::cout << "\rMatching Size = " << matching_size << std::flush;
}

void Graph::shrink_blossom(Graph::NodeId x_id, Graph::NodeId y_id) {
    auto const x_path = get_path(x_id);
    auto const y_path = get_path(y_id);
    auto const optional_r_id = get_first_path_intersection(x_path, y_path);
    assert(optional_r_id);
    auto const r_id = optional_r_id.value();

    for (auto v_it = x_path.begin(); v_it < x_path.end(); v_it++) {
        if(*v_it == r_id){ break; }
        v_it++;
        if (find_blossom_root(forest_neighbor(*v_it)) != r_id) {
            forest_neighbor(forest_neighbor(*v_it)) = *v_it;
        }
        if (*v_it == r_id) { break; }
    }
    for (auto v_it = y_path.begin(); v_it < y_path.end(); v_it++) {
        if(*v_it == r_id){ break; }
        v_it++;
        if (find_blossom_root(forest_neighbor(*v_it)) != r_id){
            forest_neighbor(forest_neighbor(*v_it)) = *v_it;
        }
        if (*v_it == r_id) { break; }
    }
    if (find_blossom_root(x_id) != r_id) { forest_neighbor(x_id) = y_id; }
    if (find_blossom_root(y_id) != r_id) { forest_neighbor(y_id) = x_id; }
    for (auto const node : x_path) {
        blossom_root(node) = r_id;
        if (node == r_id) { break; }
    }
    for (auto const node : y_path) {
        blossom_root(node) = r_id;
        if (node == r_id) { break; }
    }
}

void Graph::reset_tree(NodeId node_id) {
    auto const root = tree_root(node_id);
    for (auto tree_node: tree_nodes(root)){
        forest_neighbor(tree_node) = tree_node;
        blossom_root(tree_node) = tree_node;
        nodes[tree_node].scanned = 0;
        tree_root(tree_node) = tree_node;
    }
    tree_nodes(root) = {root};
}

std::vector<Graph::NodeId> Graph::get_path(Graph::NodeId node) {
    std::vector<NodeId> result{node};
    while (true) {
        auto const mu_id = matching_neighbor(result.back());
        if (mu_id == result.back()) {
            return result;
        }
        result.push_back(mu_id);

        auto const phi_id = forest_neighbor(mu_id);
        if (phi_id == mu_id) {
            return result;
        }
        result.push_back(phi_id);
    }
}

std::optional<Graph::NodeId>
Graph::get_first_path_intersection(const std::vector<NodeId> &path_1, const std::vector<NodeId> &path_2) {
    auto path_1_it = path_1.rbegin();
    auto path_2_it = path_2.rbegin();
    std::optional<NodeId> last_candidate;
    while(*path_1_it == *path_2_it and path_1_it < path_1.rend() and path_2_it < path_2.rend()){
        if(find_blossom_root(*path_1_it) == *path_1_it){
            last_candidate = *path_1_it;
        }
        path_1_it++;
        path_2_it++;
    }
    return last_candidate;
}



void Graph::greedy_matching() {
    std::vector<NodeId> node_ids;
    for(NodeId i = 0; i < num_nodes(); i++){
        node_ids.push_back(i);
    }
    std::ranges::sort(node_ids, [&](NodeId a, NodeId b){return nodes[a].neighbors.size() < nodes[b].neighbors.size();});
    for (auto node_id: node_ids) {
        auto& node = nodes[node_id];
        std::ranges::sort(node.neighbors, [&](NodeId a, NodeId b){return nodes[a].neighbors.size() < nodes[b].neighbors.size();});

        assert(node_id == node.id);
        if (node.matching_neighbor != node.id) { continue; }
        for (auto neighbor_id: node.neighbors) {
            if (matching_neighbor(neighbor_id) == neighbor_id) {
                matching_neighbor(neighbor_id) = node.id;
                node.matching_neighbor = neighbor_id;
                matching_size += 1;
                break;
            }
        }
    }
}

bool Graph::is_matching_legal() const {
    for(auto const& node: nodes){
        if(nodes[node.matching_neighbor].matching_neighbor != node.id){
            std::cerr << "node " << node.id
                      << " has matching neighbor " << node.matching_neighbor << ", "
                      << "but " << node.matching_neighbor << " has matching neighbor "
                      << nodes[node.matching_neighbor].matching_neighbor << std::endl;
            return false;
        }
    }
    return true;
}

std::size_t Graph::num_nodes() const {return nodes.size();}

Graph::Node::Node(Graph::NodeId id) :
    id(id),
    forest_neighbor(id),
    matching_neighbor(id),
    blossom_root(id),
    scanned(0),
    nodes_in_tree{id},
    tree_root(id)
{}
