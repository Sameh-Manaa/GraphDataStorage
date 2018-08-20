

#ifndef CSRSCHEMAHASHEDTABLEMANAGER_HPP
#define CSRSCHEMAHASHEDTABLEMANAGER_HPP

#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include <tuple>
#include <dirent.h>
#include "UtilityFunctions.hpp"

#include "CSR.hpp"
#include "SchemaHashedTable.hpp"

class CSRSchemaHashedTableManager {
private:
    CSR csr;
    SchemaHashedTable schemaHashedTable;
    int batchSize;
public:

    bool loadGraph(std::string verticesDirectory, std::string edgesDirectory, uint8_t filesToLoad = 0);
    bool loadVertices(std::string verticesDirectory, uint8_t filesToLoad);
    bool loadEdges(std::string edgesDirectory, uint8_t filesToLoad);
    std::map<int, std::string> getVertexProperties(std::string vertexHeaderLine);
    std::map<int, std::string> getEdgeProperties(std::string edgeHeaderLine);
    
    void executeQueryBI18(tm messageCreationDate, uint16_t messageLength, std::vector<std::string> messageLanguages, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);

    uint64_t getCSRSizeInBytes(){return this->csr.getCSRSizeInBytes();};
    uint64_t getSchemaHashedTableSizeInBytes(){return this->schemaHashedTable.getSchemaHashedTableSizeInBytes();};
    
    CSRSchemaHashedTableManager(int bSize):batchSize(bSize){};
};

#endif /* CSRSCHEMAHASHEDTABLEMANAGER_HPP */

