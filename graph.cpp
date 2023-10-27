//
// Created by jgier on 24.10.2023.
//

#include "graph.h"
#include <algorithm>
#include <cassert>

void Graph::outer_vertex_scan() {
    while (auto const node_id = find_unscanned_outer_vertex()) {
        neighbor_search(node_id.value());
        nodes[node_id.value()].scanned = true;
    }
}

std::optional<Graph::NodeId> Graph::find_unscanned_outer_vertex() {
    for (Node const &node: nodes) {
        if (node.mu == node.id or phi(mu(node.id)) != mu(node.id)) {
            if (not node.scanned) {
                return node.id;
            }
        }
    }
    return std::nullopt;
}

void Graph::neighbor_search(Graph::NodeId x_id) {
    for (auto y_id: nodes[x_id].neighbors) {
        if (not is_node_out_of_forest(y_id) and not (is_node_outer(y_id) and rho(y_id) != rho(x_id))) {
            continue;
        }
        if (is_node_out_of_forest(y_id)) {
            // grow forest
            phi(y_id) = x_id;
        } else if (find_path_root(x_id) != find_path_root(y_id)) {
            augment_matching(x_id, y_id);
            break;
        } else {
            shrink_blossom(x_id, y_id);
        }
    }
}

Graph::NodeId &Graph::mu(Graph::NodeId id) { return nodes[id].mu; }
Graph::NodeId Graph::mu(Graph::NodeId id) const { return nodes[id].mu; }
Graph::NodeId &Graph::phi(Graph::NodeId id) { return nodes[id].phi; }
Graph::NodeId Graph::phi(Graph::NodeId id) const { return nodes[id].phi; }
Graph::NodeId &Graph::rho(Graph::NodeId id) { return nodes[id].rho; }
Graph::NodeId Graph::rho(Graph::NodeId id) const { return nodes[id].rho; }

bool Graph::is_node_outer(Graph::NodeId x) const {
    return (mu(x) == x) or (phi(mu(x)) != mu(x));
}

bool Graph::is_node_inner(Graph::NodeId x) const {
    return (phi(mu(x)) == mu(x) and (phi(x) != x));
}

bool Graph::is_node_out_of_forest(Graph::NodeId x) const {
    return (mu(x) != x) and (phi(x) == x) and (phi(mu(x)) == mu(x));
}

Graph::NodeId Graph::find_path_root(Graph::NodeId node) {
    return get_path(node).back();
    auto previous_node = node;
    node = mu(node);
    while (node != previous_node) {
        previous_node = node;
        node = mu(phi(node));
    }
    return node;
}

void Graph::augment_matching(Graph::NodeId x_id, Graph::NodeId y_id) {
    auto const x_path = get_path(x_id);
    for (size_t i = 1; i < x_path.size(); i += 2) {
        auto v_id = x_path[i];
        mu(phi(v_id)) = v_id;
        mu(v_id) = phi(v_id);
    }
    auto const y_path = get_path(y_id);
    for (size_t i = 1; i < y_path.size(); i += 2) {
        auto v_id = y_path[i];
        mu(phi(v_id)) = v_id;
        mu(v_id) = phi(v_id);
    }
    mu(x_id) = y_id;
    mu(y_id) = x_id;
    for (auto &node: nodes) {
        node.phi = node.id;
        node.rho = node.id;
        node.scanned = false;
    }
}

std::vector<Graph::NodeId> Graph::get_path(Graph::NodeId node) {
    std::vector<NodeId> result{node};
    while (true) {
        auto const mu_id = mu(result.back());
        if (mu_id == result.back()) {
            return result;
        }
        result.push_back(mu_id);

        auto const phi_id = phi(mu_id);
        if (phi_id == mu_id) {
            return result;
        }
        result.push_back(phi_id);
    }
}

std::optional<Graph::NodeId>
Graph::get_first_path_intersection(const std::vector<NodeId> &path_1, const std::vector<NodeId> &path_2) {
    for (auto const node_id: path_1) {
        if (rho(node_id) != node_id) { continue; }
        if (std::ranges::count(path_2, node_id)) {
            return node_id;
        }
    }
    return std::nullopt;
}

void Graph::shrink_blossom(Graph::NodeId x_id, Graph::NodeId y_id) {
    auto const x_path = get_path(x_id);
    auto const y_path = get_path(y_id);
    auto const optional_r_id = get_first_path_intersection(x_path, y_path);
    assert(optional_r_id);
    auto const r_id = optional_r_id.value();

    for (size_t i = 1; i < x_path.size(); i += 2) {
        auto const v_id = x_path[i];
        if(rho(phi(v_id)) != r_id) {
            phi(phi(v_id)) = v_id;
        }
        if(v_id == r_id){ break; }
        if(i+1 < x_path.size() and x_path[i+1] == r_id){ break; }
    }
    for (size_t i = 1; i < y_path.size(); i += 2) {
        auto const v_id = y_path[i];
        if(rho(phi(v_id)) != r_id) {
            phi(phi(v_id)) = v_id;
        }
        if(v_id == r_id){ break; }
        if(i+1 < y_path.size() and y_path[i+1] == r_id){ break; }
    }
    if(rho(x_id) != r_id) { phi(x_id) = y_id; }
    if(rho(y_id) != r_id) { phi(y_id) = x_id; }
    for (auto const node_id : x_path){
        rho(node_id) = r_id;
        if(node_id == r_id){ break; }
    }
    for (auto const node_id : y_path){
        rho(node_id) = r_id;
        if(node_id == r_id){ break; }
    }
}

void Graph::add_node(std::vector<NodeId> neighbors) {
    Node node{nodes.size()};
    node.neighbors = std::move(neighbors);
    for(auto const neighbor: node.neighbors){
        nodes[neighbor].neighbors.push_back(node.id);
    }
    nodes.push_back(node);
}

void Graph::add_edge(Graph::NodeId a, Graph::NodeId b) {
    if (not std::ranges::count(nodes[a].neighbors, b)){
        nodes[a].neighbors.push_back(b);
    }
    if (not std::ranges::count(nodes[b].neighbors, a)){
        nodes[b].neighbors.push_back(a);
    }
}

void Graph::add_nodes(Graph::NodeId num_nodes) {
    for(NodeId i = 0; i < num_nodes; i++){
        nodes.emplace_back(nodes.size());
    }
}
