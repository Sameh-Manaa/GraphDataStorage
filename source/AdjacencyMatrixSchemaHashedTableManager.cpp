/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "AdjacencyMatrixSchemaHashedTableManager.hpp"

bool AdjacencyMatrixSchemaHashedTableManager::loadGraph(std::string verticesDirectory, std::string edgesDirectory) {
    if (loadVertices(verticesDirectory) &&
            loadEdges(edgesDirectory)
            ) {
        return true;
    } else {
        return false;
    }

}

bool AdjacencyMatrixSchemaHashedTableManager::loadVertices(std::string verticesDirectory) {

    uint64_t loadCounter = 0;
    uint64_t rowCount = 0;

    std::map<std::string, std::map<std::string, std::string> > vertexSchemaHashedMap;
    std::set<std::string> vertexIds;

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
        std::map<std::string, std::string> properties;

        std::istringstream iss(pent->d_name);
        std::string vertexType;
        getline(iss, vertexType, '_');


        while (std::getline(vertexFile, vertexLine)) {
            rowCount++;
            std::istringstream iss(vertexLine);
            std::string property, vertexOriginalId;

            int propertyCounter = 0;
            properties["VertexType"] = vertexType;

            while (getline(iss, property, '|')) {
                if (propertiesPositions[propertyCounter] == "id") {
                    vertexOriginalId = property;
                    propertyCounter++;
                    continue;
                }
                properties[propertiesPositions[propertyCounter++]] = property;
            }
            vertexIds.emplace(vertexType + "_" + vertexOriginalId);
            vertexSchemaHashedMap.emplace(vertexType + "_" + vertexOriginalId, properties);

            if (++loadCounter % batchSize == 0) {

                this->adjacencyMatrix.insertVertex(vertexIds);
                vertexIds.clear();
                this->schemaHashedTable.upsertVertex(vertexSchemaHashedMap);
                vertexSchemaHashedMap.clear();

            }
        }

        this->adjacencyMatrix.insertVertex(vertexIds);
        vertexIds.clear();
        this->schemaHashedTable.upsertVertex(vertexSchemaHashedMap);
        vertexSchemaHashedMap.clear();

        std::cout << "file: " << pent->d_name << std::endl;
        std::cout << "Adjacency Matrix Size: " << this->adjacencyMatrix.getAdjacencyMatrixSize() << std::endl;
        std::cout << "Schema Hashed Table Size: " << this->schemaHashedTable.getVertexSchemaHashedTableSize() << std::endl;
        std::cout << "--------------------------------------------------------------------------" << std::endl;
    }

    return true;
}

bool AdjacencyMatrixSchemaHashedTableManager::loadEdges(std::string edgesDirectory) {

    uint64_t loadCounter = 0;
    uint64_t rowCount = 0;

    std::map<std::pair<std::string, std::string>, std::map<std::string, std::map<std::string, std::string> > > edgeSchemaHashedMap;
    std::vector<std::pair<std::string, std::string> > edges;

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
        std::map<std::string, std::string> properties;

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
                    properties[propertiesPositions[propertyCounter++]] = property;
                }
            }

            edges.emplace_back(std::make_pair(sourceVertex + "_" + sourceVertexId, targetVertex + "_" + targetVertexId));
            edgeSchemaHashedMap[std::make_pair(sourceVertex + "_" + sourceVertexId, targetVertex + "_" + targetVertexId)][edgeLabel] = properties;

            if (++loadCounter % batchSize == 0) {

                this->adjacencyMatrix.addNeighbourVertex(edges);
                edges.clear();
                this->schemaHashedTable.upsertEdge(edgeSchemaHashedMap);
                edgeSchemaHashedMap.clear();

            }
        }

        this->adjacencyMatrix.addNeighbourVertex(edges);
        edges.clear();
        this->schemaHashedTable.upsertEdge(edgeSchemaHashedMap);
        edgeSchemaHashedMap.clear();

        std::cout << "file: " << pent->d_name << std::endl;
        std::cout << "Adjacency Matrix Size: " << this->adjacencyMatrix.getAdjacencyMatrixSize() << std::endl;
        std::cout << "Vertex Schema Hashed Table Size: " << this->schemaHashedTable.getVertexSchemaHashedTableSize() << std::endl;
        std::cout << "Edge Schema Hashed Table Size: " << this->schemaHashedTable.getEdgeSchemaHashedTableSize() << std::endl;
        std::cout << "--------------------------------------------------------------------------" << std::endl;
    }

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
