/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "AdjacencyMatrixSchemaHashedTableManager.hpp"

bool AdjacencyMatrixSchemaHashedTableManager::loadGraph(std::string verticesDirectory, std::string edgesDirectory) {
    if (loadVertices(verticesDirectory) && loadEdges(edgesDirectory)) {
        this->adjacencyMatrix.shrinkToFit();
        return true;
    } else {
        return false;
    }

}

bool AdjacencyMatrixSchemaHashedTableManager::loadVertices(std::string verticesDirectory) {

    uint64_t loadCounter = 0;
    uint64_t rowCount = 0;

    std::vector< std::pair <std::string, std::unordered_map<std::string, char*> > > vertexSchemaHashedMap;

    DIR *pdir = NULL;
    struct dirent *pent = NULL;
    pdir = opendir(verticesDirectory.data());

    while ((pent = readdir(pdir)) != NULL) {
        rowCount = 0;

        std::cout << verticesDirectory + "/" + pent->d_name << std::endl;
        std::ifstream vertexFile(verticesDirectory + "/" + pent->d_name);

        std::string vertexLine, headerLine;
        std::getline(vertexFile, headerLine);

        std::map<int, std::string> propertiesPositions = this->getVertexProperties(headerLine);
        std::unordered_map<std::string, char*> properties;

        std::istringstream iss(pent->d_name);
        std::string vertexType;
        getline(iss, vertexType, '_');


        while (std::getline(vertexFile, vertexLine)) {
            rowCount++;
            std::istringstream iss(vertexLine);
            std::string property, vertexOriginalId;

            int propertyCounter = 0;

            while (getline(iss, property, '|')) {
                if (propertiesPositions[propertyCounter] == "id") {
                    vertexOriginalId = property;
                    propertyCounter++;
                    continue;
                }
                char * propertyVal = new char [property.length() + 1];
                strcpy(propertyVal, property.c_str());
                properties[propertiesPositions[propertyCounter++]] = propertyVal;
            }

            vertexSchemaHashedMap.emplace_back(vertexType + "_" + vertexOriginalId, properties);

            if (++loadCounter % batchSize == 0) {

                this->schemaHashedTable.upsertVertex(vertexSchemaHashedMap);
                vertexSchemaHashedMap.clear();

            }
        }

        this->schemaHashedTable.upsertVertex(vertexSchemaHashedMap);
        vertexSchemaHashedMap.clear();

        std::cout << "file: " << pent->d_name << std::endl;
        std::cout << "Adjacency Matrix Size: " << this->adjacencyMatrix.getAdjacencyMatrixSize() << std::endl;
        std::cout << "Vertex Schema Hashed Table Size: " << this->schemaHashedTable.getVertexSchemaHashedTableSize() << std::endl;
        std::cout << "Edge Schema Hashed Table Size: " << this->schemaHashedTable.getEdgeSchemaHashedTableSize() << std::endl;
        std::cout << "--------------------------------------------------------------------------" << std::endl;
    }

    closedir(pdir);
    return true;
}

bool AdjacencyMatrixSchemaHashedTableManager::loadEdges(std::string edgesDirectory) {

    uint64_t loadCounter = 0;
    uint64_t rowCount = 0;

    std::vector< std::pair <std::string, std::unordered_map<std::string, char*> > > vertexSchemaHashedMap;
    std::unordered_map<std::string, char*> vertexProperties;

    std::vector< std::pair <std::string, std::unordered_map<std::string, char*> > > edgeSchemaHashedMap;
    std::vector<std::tuple<std::string, std::string, std::string> > edges;

    DIR *pdir = NULL;
    struct dirent *pent = NULL;
    pdir = opendir(edgesDirectory.data());

    while ((pent = readdir(pdir)) != NULL) {
        rowCount = 0;

        std::cout << edgesDirectory + "/" + pent->d_name << std::endl;
        std::ifstream edgeFile(edgesDirectory + "/" + pent->d_name);

        std::string edgeLine, headerLine;
        std::getline(edgeFile, headerLine);

        std::map<int, std::string> propertiesPositions = this->getEdgeProperties(headerLine);
        std::unordered_map<std::string, char*> properties;

        std::istringstream iss(pent->d_name);
        std::string sourceVertex, edgeLabel, targetVertex;
        getline(iss, sourceVertex, '_');
        getline(iss, edgeLabel, '_');
        getline(iss, targetVertex, '_');


        while (std::getline(edgeFile, edgeLine)) {
            rowCount++;

            std::istringstream iss(edgeLine);
            std::string property, sourceVertexId, targetVertexId;

            uint64_t propertyCounter = 0;
            while (getline(iss, property, '|')) {
                if (propertyCounter == 0) {
                    sourceVertexId = property;
                    propertyCounter++;
                } else if (propertyCounter == 1) {
                    targetVertexId = property;
                    propertyCounter++;
                } else {
                    char * propertyVal = new char [property.length() + 1];
                    strcpy(propertyVal, property.c_str());
                    properties[propertiesPositions[propertyCounter++]] = propertyVal;
                }
            }

            vertexSchemaHashedMap.emplace_back(sourceVertex + "_" + sourceVertexId, vertexProperties);
            vertexSchemaHashedMap.emplace_back(targetVertex + "_" + targetVertexId, vertexProperties);

            edges.emplace_back(std::make_tuple(sourceVertex + "_" + sourceVertexId, edgeLabel, targetVertex + "_" + targetVertexId));
            edgeSchemaHashedMap.emplace_back(sourceVertex + "_" + sourceVertexId + "$" + edgeLabel + "$" + targetVertex + "_" + targetVertexId, properties);

            if (++loadCounter % batchSize == 0) {

                this->schemaHashedTable.upsertVertex(vertexSchemaHashedMap);
                vertexSchemaHashedMap.clear();

                this->schemaHashedTable.upsertEdge(edgeSchemaHashedMap);
                edgeSchemaHashedMap.clear();

                this->adjacencyMatrix.addNeighbourVertex(edges);
                edges.clear();

            }
        }

        this->schemaHashedTable.upsertVertex(vertexSchemaHashedMap);
        vertexSchemaHashedMap.clear();

        this->schemaHashedTable.upsertEdge(edgeSchemaHashedMap);
        edgeSchemaHashedMap.clear();

        this->adjacencyMatrix.addNeighbourVertex(edges);
        edges.clear();

        std::cout << "file: " << pent->d_name << std::endl;
        std::cout << "Adjacency Matrix Size: " << this->adjacencyMatrix.getAdjacencyMatrixSize() << std::endl;
        std::cout << "Vertex Schema Hashed Table Size: " << this->schemaHashedTable.getVertexSchemaHashedTableSize() << std::endl;
        std::cout << "Edge Schema Hashed Table Size: " << this->schemaHashedTable.getEdgeSchemaHashedTableSize() << std::endl;
        std::cout << "--------------------------------------------------------------------------" << std::endl;
    }

    closedir(pdir);
    return true;
}

std::map<int, std::string> AdjacencyMatrixSchemaHashedTableManager::getVertexProperties(std::string vertexHeaderLine) {
    std::map<int, std::string> propertiesPositions;
    std::istringstream iss(vertexHeaderLine);
    std::string propertyName;
    while (getline(iss, propertyName, '|')) {
        propertiesPositions[propertiesPositions.size()] = propertyName;
    }
    return propertiesPositions;
}

std::map<int, std::string> AdjacencyMatrixSchemaHashedTableManager::getEdgeProperties(std::string edgeHeaderLine) {
    std::map<int, std::string> propertiesPositions;
    std::istringstream iss(edgeHeaderLine);
    std::string propertyName;
    while (getline(iss, propertyName, '|')) {
        propertiesPositions[propertiesPositions.size()] = propertyName;
    }
    return propertiesPositions;
}
