

#ifndef CSRSCHEMAHASHEDTABLEMANAGER_HPP
#define CSRSCHEMAHASHEDTABLEMANAGER_HPP

#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include <tuple>
#include <dirent.h>

#include "CSR.hpp"
#include "SchemaHashedTable.hpp"

class CSRSchemaHashedTableManager {
private:
    CSR csr;
    SchemaHashedTable schemaHashedTable;
    int batchSize;
public:

    bool loadGraph(std::string verticesDirectory, std::string edgesDirectory);
    bool loadVertices(std::string verticesDirectory);
    bool loadEdges(std::string edgesDirectory);
    std::map<int, std::string> getVertexProperties(std::string vertexHeaderLine);
    std::map<int, std::string> getEdgeProperties(std::string edgeHeaderLine);
    
    CSRSchemaHashedTableManager(int bSize):batchSize(bSize){};
};

#endif /* CSRSCHEMAHASHEDTABLEMANAGER_HPP */

