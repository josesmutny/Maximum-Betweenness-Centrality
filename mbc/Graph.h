#pragma once

#include <iostream>

typedef size_t vertex_t;

/**
 * Simple Graph data structure.
 *
 * Implemented for O(1) access to neighbour lists.
 * Checking whether an edge exists is linear in the size of
 * the neighbourhood of the vertices in the edge (worst case O(n))
 */
class Graph {
    size_t m_n{};
    size_t m_m{};
    std::vector<std::vector<vertex_t>> m_neighbours;

public:
    Graph(size_t n, size_t m, std::istream &in = std::cin);

    Graph();

    std::vector<vertex_t> const & getNeighbours(vertex_t u) const;

    bool areConnected(vertex_t u, vertex_t v) const;

    void addEdge(vertex_t u, vertex_t v);

    /**
     * Prints graph to ostream with the following format:
     * n m
     * edgeSrc edgeDst
     * ...
     */
    friend std::ostream &operator<<(std::ostream &out, const Graph &g);
};
