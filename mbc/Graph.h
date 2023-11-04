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
    static vertex_t zeroIndex(const size_t v) { return v - 1; }
    static std::pair<vertex_t, vertex_t> zeroIndex(const size_t u, const size_t v) { return {zeroIndex(u), zeroIndex((v))}; }

    Graph(size_t n, size_t m, std::istream &in = std::cin);

    explicit Graph(std::istream &in = std::cin);

    std::vector<vertex_t> const & getNeighbours(vertex_t u) const;

    bool hasEdge(vertex_t u, vertex_t v) const;

    Graph & addEdge(vertex_t u, vertex_t v);

    /**
     * Prints graph to ostream with the following format:
     * n m
     * edgeSrc edgeDst
     * ...
     */
    friend std::ostream &operator<<(std::ostream &out, const Graph &g);
};
