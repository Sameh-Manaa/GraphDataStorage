#ifndef CSRUNIVERSALTABLEMANAGER_HPP
#define CSRUNIVERSALTABLEMANAGER_HPP

#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include <tuple>
#include <dirent.h>

#include "CSR.hpp"
#include "UniversalTable.hpp"



class CSRUniversalTableManager {
private:
    CSR csr;
    UniversalTable universalTable;
    int batchSize;
public:

    bool loadGraph(std::string verticesDirectory, std::string edgesDirectory);
    bool loadVertices(std::string verticesDirectory);
    bool loadEdges(std::string edgesDirectory);
    std::vector<int64_t> addVertexProperties(std::string vertexHeaderLine);
    std::vector<int64_t> addEdgeProperties(std::string edgeHeaderLine);
    
    CSRUniversalTableManager(int bSize):batchSize(bSize){};
    
};

#endif /* CSRUNIVERSALTABLEMANAGER_HPP */

