/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "AdjacencyListSchemaHashedTableManager.hpp"

namespace fs = std::experimental::filesystem;

bool AdjacencyListSchemaHashedTableManager::loadGraph(std::string verticesDirectory, std::string edgesDirectory) {
    if (loadVertices(verticesDirectory) &&
            loadEdges(edgesDirectory)
            ) {
        return true;
    } else {
        return false;
    }

}

bool AdjacencyListSchemaHashedTableManager::loadVertices(std::string verticesDirectory) {

    uint64_t loadCounter = 0;
    uint64_t rowCount = 0;

    std::map<std::string, std::map<std::string, std::string> > vertexSchemaHashedMap;
    std::set<std::string> vertexIds;

    for (auto & p : fs::directory_iterator(verticesDirectory)) {
        rowCount = 0;
        std::cout << p.path() << std::endl;
        std::ifstream vertexFile(p.path().string());
        std::string vertexLine, headerLine;
        std::getline(vertexFile, headerLine);
        std::map<std::string, std::string> properties;
        std::map<int, std::string> propertiesPositions = this->getVertexProperties(headerLine);

        std::istringstream iss(p.path().filename().string());
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
            //this->adjacencyList.insertVertex(properties[0] + "_" + vertexOriginalId);
            vertexIds.emplace(vertexType + "_" + vertexOriginalId);
            //this->universalTable->upsertVertex(properties[0] + "_" + vertexOriginalId, properties);
            vertexSchemaHashedMap.emplace(vertexType + "_" + vertexOriginalId, properties);
            //std::fill(properties.begin(), properties.end(), "");
            properties.clear();
            if (++loadCounter % batchSize == 0) {

                this->adjacencyList.insertVertex(vertexIds);
                vertexIds.clear();
                this->schemaHashedTable.upsertVertex(vertexSchemaHashedMap);
                vertexSchemaHashedMap.clear();

                std::cout << "file: " << p.path().filename().string() << std::endl;
                std::cout << "Adjacency List Size: " << this->adjacencyList.getAdjacencyListSize() << std::endl;
                std::cout << "Schema Hashed Table Size: " << this->schemaHashedTable.getVertexSchemaHashedTableSize() << std::endl;
                std::cout << "--------------------------------------------------------------------------" << std::endl;
            }
        }
    }
    this->adjacencyList.insertVertex(vertexIds);
    vertexIds.clear();
    this->schemaHashedTable.upsertVertex(vertexSchemaHashedMap);
    vertexSchemaHashedMap.clear();

    //std::cout << "file: " << p.path().filename().string() << std::endl;
    std::cout << "Adjacency List Size: " << this->adjacencyList.getAdjacencyListSize() << std::endl;
    std::cout << "Schema Hashed Table Size: " << this->schemaHashedTable.getVertexSchemaHashedTableSize() << std::endl;
    std::cout << "--------------------------------------------------------------------------" << std::endl;
    return true;
}

bool AdjacencyListSchemaHashedTableManager::loadEdges(std::string edgesDirectory) {

    uint64_t loadCounter = 0;
    uint64_t rowCount = 0;

    std::map<std::pair<std::string, std::string>, std::map<std::string, std::map<std::string, std::string> > > edgeSchemaHashedMap;
    std::vector<std::tuple<std::string, std::string, std::string> > edges;

    for (auto & p : fs::directory_iterator(edgesDirectory)) {
        rowCount = 0;
        std::cout << p.path() << std::endl;
        std::ifstream edgeFile(p.path().string());
        std::string edgeLine, headerLine;
        std::getline(edgeFile, headerLine);

        std::map<std::string, std::string> properties;
        std::map<int, std::string> propertiesPositions = this->getEdgeProperties(headerLine);

        std::istringstream iss(p.path().filename().string());
        std::string sourceVertex, edgeLabel, targetVertex;
        getline(iss, sourceVertex, '_');
        getline(iss, edgeLabel, '_');
        getline(iss, targetVertex, '_');


        while (std::getline(edgeFile, edgeLine)) {
            std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
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

            edges.emplace_back(std::make_tuple(sourceVertex + "_" + sourceVertexId, edgeLabel, targetVertex + "_" + targetVertexId));
            edgeSchemaHashedMap[std::make_pair(sourceVertex + "_" + sourceVertexId, targetVertex + "_" + targetVertexId)][edgeLabel] = properties;
            //edgeIds.insert(properties[0] + "_" + sourceOriginalId);
            //this->universalTable->upsertVertex(properties[0] + "_" + vertexOriginalId, properties);
            //edgeUniversalMap[properties[0] + "_" + sourceOriginalId] = properties;
            if (++loadCounter % batchSize == 0) {


                //                this->adjacencyList.insertVertex(edgeIds);
                //                edgeIds.clear();
                //                this->universalTable->upsertVertex(edgeUniversalMap);
                //                edgeUniversalMap.clear();

                std::vector<bool> result = this->adjacencyList.addNeighbourVertex(edges);
                for (int i = 0; i < result.size(); i++) {
                    if (!result[i]) {
                        edgeSchemaHashedMap.erase(std::make_pair(std::get<0>(edges[i]), std::get<2>(edges[i])));
                    }
                }
                edges.clear();


                this->schemaHashedTable.upsertEdge(edgeSchemaHashedMap);
                edgeSchemaHashedMap.clear();

                std::cout << "file: " << p.path().filename().string() << std::endl;
                std::cout << "Adjacency List Size: " << this->adjacencyList.getAdjacencyListSize() << std::endl;
                std::cout << "Vertex Schema Hashed Table Size: " << this->schemaHashedTable.getVertexSchemaHashedTableSize() << std::endl;
                std::cout << "Edge Schema Hashed Table Size: " << this->schemaHashedTable.getEdgeSchemaHashedTableSize() << std::endl;
                std::cout << "--------------------------------------------------------------------------" << std::endl;

                std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
                std::cout << "Duration(schemaHashedTable.upsertEdge(edgeSchemaHashedMap)): " << duration << std::endl;
            }
        }
        properties.clear();
    }

    std::vector<bool> result = this->adjacencyList.addNeighbourVertex(edges);
    for (int i = 0; i < result.size(); i++) {
        if (!result[i]) {
            edgeSchemaHashedMap.erase(std::make_pair(std::get<0>(edges[i]), std::get<2>(edges[i])));
        }
    }
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    this->schemaHashedTable.upsertEdge(edgeSchemaHashedMap);

    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << "Duration(schemaHashedTable.upsertEdge(edgeSchemaHashedMap)): " << duration << std::endl;
    //edgeSchemaHashedMap.clear();

    //std::cout << "file: " << p.path().filename().string() << std::endl;
    std::cout << "Adjacency List Size: " << this->adjacencyList.getAdjacencyListSize() << std::endl;
    std::cout << "Vertex Schema Hashed Table Size: " << this->schemaHashedTable.getVertexSchemaHashedTableSize() << std::endl;
    std::cout << "Edge Schema Hashed Table Size: " << this->schemaHashedTable.getEdgeSchemaHashedTableSize() << std::endl;
    std::cout << "--------------------------------------------------------------------------" << std::endl;
    return true;
}

std::map<int, std::string> AdjacencyListSchemaHashedTableManager::getVertexProperties(std::string vertexHeaderLine) {
    std::map<int, std::string> propertiesPositions;
    std::istringstream iss(vertexHeaderLine);
    std::string propertyName;
    while (getline(iss, propertyName, '|')) {
        propertiesPositions[propertiesPositions.size()] = propertyName;
    }
    return propertiesPositions;
}

std::map<int, std::string> AdjacencyListSchemaHashedTableManager::getEdgeProperties(std::string edgeHeaderLine) {
    std::map<int, std::string> propertiesPositions;
    std::istringstream iss(edgeHeaderLine);
    std::string propertyName;
    while (getline(iss, propertyName, '|')) {
        propertiesPositions[propertiesPositions.size()] = propertyName;
    }
    return propertiesPositions;
}
