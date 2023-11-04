#include "Graph.h"

Graph::Graph(const size_t n, const size_t m, std::istream &in) :
        m_n(n),
        m_m(m),
        m_neighbours(n, std::vector<size_t>()){

    for (int edgeIndex = 0; edgeIndex < m_m; ++edgeIndex) {
        vertex_t a, b;
        in >> a >> b;
        addEdge(--a, --b); // zero-index
    }
}

Graph::Graph() {
    std::cin >> m_n >> m_m;
    *this = Graph(m_n, m_m);
}

std::vector<vertex_t> const &Graph::getNeighbours(const vertex_t u) const {
    return m_neighbours[u];
}

bool Graph::areConnected(const vertex_t u, const vertex_t v) const {
    bool uHasMoreNeighbours = m_neighbours[u].size() > m_neighbours[v].size();
    auto &neighbours = m_neighbours[uHasMoreNeighbours ? v : u];
    auto value = uHasMoreNeighbours ? u : v;
    return std::find(neighbours.begin(), neighbours.end(), value) != neighbours.end();
}

void Graph::addEdge(const vertex_t u, const vertex_t v) {
    m_neighbours[u].push_back(v);
    m_neighbours[v].push_back(u);
}

std::ostream &operator<<(std::ostream &out, const Graph &g) {
    out << g.m_n << ' ' << g.m_m << '\n';
    for (vertex_t src = 0; src < g.m_n; ++src) {
        auto const &neighbours = g.m_neighbours[src];
        for(auto neighbour: neighbours) {
            if(src < neighbour)
                out << src << ' ' << neighbour << '\n';
        }
    }
    return out << std::endl;
}