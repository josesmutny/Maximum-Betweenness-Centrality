#pragma once

#include <cstdio>
#include <vector>
#include <queue>
#include <list>
#include <map>
#include "Graph.h"


/**
 * Calculate the number of shortest paths between pairs of vertices that are not forbidden.
 * Input graph should be connected.
 * A vertex v can be disregarded by setting isForbidden[v] to true.
 * If the size of isForbidden is not equal to the number of vertices, it is assumed
 * that no vertices are forbidden.
 *
 *
 * After execution, distances contains the distance for each pair of vertices, excluiding those pairs
 * containing a forbidden vertex.
 */
std::vector<std::vector<size_t>> calculatePathCounts(const Graph &g, std::vector<std::vector<size_t>> &distances,
                                                     const std::vector<bool> &isForbidden = {}) {
    auto &mask = isForbidden.size() == g.vertexCount() ? isForbidden : std::vector<bool>(g.vertexCount(), false);

    // modified BFS ran once from each vertex.
    std::vector<std::vector<size_t>> pathCounts(g.vertexCount(), std::vector<size_t>(g.vertexCount(), 0));
    distances = std::vector<std::vector<size_t>>(g.vertexCount(), std::vector<size_t>(g.vertexCount()));

    for (int src = 0; src < g.vertexCount(); ++src) {
        if (mask[src])
            continue;

        std::vector<bool> isVisited(g.vertexCount(), false);
        auto &pathCount = pathCounts[src];
        auto &distance = distances[src];

        std::queue<vertex_t> toBeProcessed;

        toBeProcessed.push(src);
        isVisited[src] = true;
        pathCount[src] = 1;
        distance[src] = 0;

        // TODO: reimplement so that only A -> B paths and not B -> A paths need to be considered
        while (!toBeProcessed.empty()) {
            vertex_t currVertex = toBeProcessed.front();
            toBeProcessed.pop();

            for (auto &neighbour: g.getNeighbours(currVertex)) {
                if (mask[neighbour])
                    continue;

                if (!isVisited[neighbour]) {
                    isVisited[neighbour] = true;
                    toBeProcessed.push(neighbour);
                    distance[neighbour] = distance[currVertex] + 1;
                    pathCount[neighbour] = pathCount[currVertex];
                } else if (distance[neighbour] == distance[currVertex] + 1) {
                    pathCount[neighbour] += pathCount[currVertex];
                }
            }
        }
    }
    return pathCounts;
}

double groupBetweennessCentrality(const Graph &g, const std::vector<bool> &isInGroup) {
    std::vector<std::vector<size_t>> initialDistances;
    auto initialPathCounts = calculatePathCounts(g, initialDistances);

    // Repeat, but avoid vertices from group
    std::vector<std::vector<size_t>> distances;
    auto pathCounts = calculatePathCounts(g, distances, isInGroup);

    double GBC = 0;

    // TODO check g size edge cases
    for (int i = 0; i < g.vertexCount() - 1; ++i) {
        if (isInGroup[i]) {
            GBC += g.vertexCount() - 1;
            continue;
        }

        for (int j = i + 1; j < g.vertexCount(); ++j) {
            if (isInGroup[j]) {
                GBC += 1;
                continue;
            }

            if (initialDistances[i][j] == distances[i][j])
                GBC += (double) (initialPathCounts[i][j] - pathCounts[i][j]) / (double) initialPathCounts[i][j];
        }
    }
    return GBC * 2;
}

void mbcAux(
        const size_t intervalStart,
        const size_t n,
        const size_t k,
        const Graph &g,
        const size_t depth,
        std::vector<std::vector<size_t>> &pathCounts,
        const std::vector<std::vector<size_t>> &initialPathCounts,
        const std::vector<std::vector<size_t>> &initialDistances,
        std::vector<bool> &isInGroup,
        double GBC,
        std::multimap<double, std::vector<bool>> &optimalGroups
) {
    if (depth == k) {
        auto currLeastGoodGroup = optimalGroups.begin();
        if(currLeastGoodGroup == optimalGroups.end())
            optimalGroups.emplace(GBC, isInGroup);
        else if(GBC > currLeastGoodGroup->first) {
            if(optimalGroups.size() > 5)
                optimalGroups.erase(currLeastGoodGroup);
            optimalGroups.emplace(GBC, isInGroup);
        }
        return;
    }

    for (vertex_t newVertex = intervalStart; newVertex <= n - k + depth; ++newVertex) {
        std::vector<std::vector<size_t>> coveredPathCount(n, std::vector<size_t>(n, 0));

        // TODO: Edge cases for n
        for (int i = 0; i < n - 1; ++i) {
            if(isInGroup[i]) continue;
            for (int j = i + 1; j < n; ++j) {
                if(isInGroup[j]) continue;
                // check if vertices appear ordered as (i, newVertex, j), if so, then consider said paths as covered by newVertex
                // additionally, check if newVertex == i || newVertex == j
                if(initialDistances[i][j] == initialDistances[i][newVertex] + initialDistances[newVertex][j]){
                    coveredPathCount[i][j] = pathCounts[i][newVertex] * pathCounts[newVertex][j];
                    coveredPathCount[j][i] = coveredPathCount[i][j];
                } else if(i == newVertex || j == newVertex) {
                    coveredPathCount[i][j] = pathCounts[i][j];
                    coveredPathCount[j][i] = coveredPathCount[i][j];
                }
            }
        }

        double contributionToGBC = 0;
        for (int i = 0; i < n - 1; ++i) {
            if(isInGroup[i]) continue;
            for (int j = i + 1; j < n; ++j) {
                if(isInGroup[j]) continue;
                if(pathCounts[i][j])
                    contributionToGBC += (double)coveredPathCount[i][j] / (double) initialPathCounts[i][j];
            }
        }
        contributionToGBC *= 2; // for forward and backwards paths

        for (int i = 0; i < n - 1; ++i) {
            for (int j = i + 1; j < n; ++j) {
                // if (i == newVertex || j == newVertex) continue;
                pathCounts[i][j] -= coveredPathCount[i][j];
                pathCounts[j][i] -= coveredPathCount[j][i];
            }
        }
        // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        isInGroup[newVertex] = true;
        mbcAux(newVertex + 1, n, k, g, depth + 1, pathCounts, initialPathCounts, initialDistances, isInGroup,
                GBC + contributionToGBC, optimalGroups);
        isInGroup[newVertex] = false;
        for (int i = 0; i < n - 1; ++i) {
            for (int j = i + 1; j < n; ++j) {
                // if (i == newVertex || j == newVertex) continue;
                pathCounts[i][j] += coveredPathCount[i][j];
                pathCounts[j][i] += coveredPathCount[j][i];
            }
        }
    }
}

void maximumBetweennessCentrality(const Graph &g, const size_t budget) {
    size_t n = g.vertexCount();
    size_t k = budget;

    std::vector<std::vector<size_t>> initialDistances;
    auto initialPathCounts = calculatePathCounts(g, initialDistances);
    auto pathCounts = initialPathCounts;

    // TODO: keep best x groups instead of only the optimal one
    std::multimap<double, std::vector<bool>> optimalGroups;
    optimalGroups.emplace(0, std::vector<bool>(n, false));
    std::vector<bool> isInGroup(n, false);
    mbcAux(0, n, k, g, 0, pathCounts, initialPathCounts, initialDistances, isInGroup, 0, optimalGroups);



    for(auto [gbc, group]: optimalGroups) {
        std::cout << gbc << ":";
        for (int i = 0; i < n; ++i) {
            if(group[i])
                std::cout << " " << i + 1;
        }
        std::cout << '\n';
    }
    std::cout.flush();
}
