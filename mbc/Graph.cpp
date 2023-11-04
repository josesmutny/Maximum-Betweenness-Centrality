#include "Graph.h"

Graph::Graph(const size_t n, const size_t m, std::istream &in) :
        m_n(n),
        m_m(m),
        m_neighbours(n, std::vector<size_t>()){

    for (int edgeIndex = 0; edgeIndex < m_m; ++edgeIndex) {
        size_t a, b;
        in >> a >> b;
        addEdge(a, b);
    }
}

Graph::Graph(std::istream &in) {
    size_t n, m;
    in >> n >> m;
    *this = Graph(n, m, in);
}

std::vector<vertex_t> const &Graph::getNeighbours(const vertex_t u) const {
    return m_neighbours[u];
}

bool Graph::hasEdge(vertex_t u, vertex_t v) const {
    auto [s, t] = zeroIndex(u, v);
    bool sHasMoreNeighbours = m_neighbours[s].size() > m_neighbours[t].size();
    auto &neighbours = m_neighbours[sHasMoreNeighbours ? t : s];
    auto value = sHasMoreNeighbours ? s : t;
    return std::find(neighbours.begin(), neighbours.end(), value) != neighbours.end();
}

Graph & Graph::addEdge(vertex_t u, vertex_t v) {
    m_neighbours[zeroIndex(u)].push_back(zeroIndex(v));
    m_neighbours[zeroIndex(v)].push_back(zeroIndex(u));
    return *this;
}

std::ostream &operator<<(std::ostream &out, const Graph &g) {
    out << g.m_n << ' ' << g.m_m << '\n';
    for (vertex_t src = 0; src < g.m_n; ++src) {
        auto const &neighbours = g.m_neighbours[src];
        for(auto neighbour: neighbours) {
            if(src < neighbour)
                out << src + 1 << ' ' << neighbour + 1 << '\n'; // zero-index to one-index
        }
    }
    return out << std::flush;
}