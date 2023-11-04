#include <iostream>
#include <sstream>
#include "../mbc/graph.h"

void test(const bool assertion, const char *message) {
    if(!assertion)
        throw std::runtime_error(message);
}

void testDefaultConstructor() {
    std::string edges = "1 2 1 3 1 4 1 5 1 6";
    std::istringstream iss(edges);

    Graph graph(6, 5, iss);

    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            if(i == 0 xor j == 0)
                test(graph.areConnected(i + 1, j + 1), "Missing edge during construction with given sizes");
            else
                test(!graph.areConnected(i + 1, j + 1), "Unexpected edge during construction with given sizes");

            // check edge symmetry
            if(i != j)
                test(graph.areConnected(i + 1, j + 1) == graph.areConnected(j + 1, i + 1), "Edges are not symmetrical");
        }
    }

    // check empty graph
    iss = std::istringstream("");
    graph = Graph(0, 0, iss);

    // check graph of isolated vertices;
    graph = Graph(100, 0, iss);
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            // TODO rename to hasEdge(...)
            test(!graph.areConnected(i + 1, j + 1), "Unexpected edge in empty graph");
        }
    }
}

void testDefaultConstructorWithoutSize() {
    std::string graphString = "6 5 1 2 1 3 1 4 1 5 1 6";
    std::istringstream iss(graphString);

    Graph graph(iss);
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            if(i == 0 xor j == 0)
                test(graph.areConnected(i + 1, j + 1), "Missing edge during construction without given sizes");
            else
                test(!graph.areConnected(i + 1, j + 1), "Unexpected edge during construction without given sizes");

            // check edge symmetry
            if(i != j)
                test(graph.areConnected(i + 1, j + 1) == graph.areConnected(j + 1, i + 1), "Edges are not symmetrical");
        }
    }

    // check empty graph
    graphString = "0 0";
    iss = std::istringstream(graphString);
    graph = Graph(iss);

    // check graph of isolated vertices;
    graphString = "100 0";
    iss = std::istringstream(graphString);
    graph = Graph(iss);
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            test(!graph.areConnected(i + 1, j + 1), "Unexpected edge in empty graph");
        }
    }
}

void testAddEdge() {
    // add edge
    std::string edges = "1 2 2 3 3 4 4 5";
    std::istringstream iss(edges);
    Graph referenceGraph(5, 4, iss);

    Graph testGraph(5, 0);

    testGraph.addEdge(1, 2)
            .addEdge(3, 4)
            .addEdge(2, 3)
            .addEdge(5, 4);

    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            test(referenceGraph.areConnected(i + 1, j + 1) == testGraph.areConnected(i + 1, j + 1), "Differing edge between reference and added ones");

    // add duplicate edge
    testGraph.addEdge(2, 4);
    testGraph.addEdge(4, 2);
    test(testGraph.areConnected(2, 4), "Missing edge after double add");
}

void testPrint() {
    std::string edges = "1 2 1 3 1 4 1 5 1 6 2 3 3 4 4 5 5 6 6 2";
    std::istringstream iss(edges);
    Graph graph(6, 10,iss);
    std::ostringstream oss;
    oss << graph;
    auto graphPrintOutput = oss.str();
    test(graphPrintOutput == "6 10\n1 2\n1 3\n1 4\n1 5\n1 6\n2 3\n2 6\n3 4\n4 5\n5 6\n", "Unexpected graph output");

    oss = std::ostringstream();
    graph = Graph(10, 0);
    oss << graph;
    graphPrintOutput = oss.str();
    test(graphPrintOutput == "10 0\n", "Unexpected edgeless graph output");
}

int main() {
    try {
        testDefaultConstructor();
        testDefaultConstructorWithoutSize();
        testAddEdge();
        testPrint();
    } catch(const std::runtime_error &error) {
        std::cout << error.what() << std::endl;
        return 1;
    }
    std::cout << "Passed all tests" << std::endl;
}
