#ifndef CSREMERGINGSCHEMAMANAGER_HPP
#define CSREMERGINGSCHEMAMANAGER_HPP

#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include <tuple>
#include <dirent.h>
#include "UtilityFunctions.hpp"

#include "CSR.hpp"
#include "UniversalTable.hpp"
#include "EmergingSchema.hpp"



class CSREmergingSchemaManager {
private:
    CSR csr;
    UniversalTable universalTable;
    EmergingSchema emergingSchema;
    int batchSize;
public:

    bool loadGraph(std::string verticesDirectory, std::string edgesDirectory);
    bool loadVertices(std::string verticesDirectory);
    bool loadEdges(std::string edgesDirectory);
    std::vector<int16_t> addVertexProperties(std::string vertexHeaderLine);
    std::vector<int16_t> addEdgeProperties(std::string edgeHeaderLine);
    
    void executeQueryBI18(tm messageCreationDate, uint16_t messageLength, std::vector<std::string> messageLanguages, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);
    
    uint64_t getCSRSizeInBytes(){return this->csr.getCSRSizeInBytes();};
    uint64_t getEdgeEmergingSchemaSizeInBytes(){return this->emergingSchema.getEdgeEmergingSchemaSizeInBytes();};
    
    CSREmergingSchemaManager(int bSize):batchSize(bSize){};
    
};

#endif /* CSREMERGINGSCHEMAMANAGER_HPP */

