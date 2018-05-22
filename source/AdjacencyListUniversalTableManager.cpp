/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "AdjacencyListUniversalTableManager.hpp"

namespace fs = std::experimental::filesystem;

bool AdjacencyListUniversalTableManager::loadGraph(std::string verticesDirectory, std::string edgesDirectory) {
    if (loadVertices(verticesDirectory) &&             loadEdges(edgesDirectory)
            ) {
        return true;
    } else {
        return false;
    }

}

bool AdjacencyListUniversalTableManager::loadVertices(std::string verticesDirectory) {

    uint64_t loadCounter = 0;
    uint64_t rowCount = 0;
    this->addVertexProperties("VertexType");

    std::map < std::string, std::vector<std::string> > vertexUniversalMap;
    std::set<std::string> vertexIds;

    for (auto & p : fs::directory_iterator(verticesDirectory)) {
        rowCount = 0;
        std::cout << p.path() << std::endl;
        std::ifstream vertexFile(p.path().string());
        std::string vertexLine, headerLine;
        std::getline(vertexFile, headerLine);

        std::vector<int64_t> propertiesPositions = this->addVertexProperties(headerLine);

        std::vector<std::string> properties(universalTable->getVertexPropertyOrder().size());

        std::istringstream iss(p.path().filename().string());
        std::string vertexType;
        getline(iss, vertexType, '_');


        while (std::getline(vertexFile, vertexLine) && rowCount < 30000000) {
            rowCount++;
            properties[0] = vertexType;
            std::istringstream iss(vertexLine);
            std::string property, vertexOriginalId;
            uint64_t propertyCounter = 0;
            while (getline(iss, property, '|')) {
                if (propertiesPositions[propertyCounter] == -1) {
                    vertexOriginalId = property;
                    propertyCounter++;
                    continue;
                }
                properties[propertiesPositions[propertyCounter++]] = property;
            }
            //this->adjacencyList->insertVertex(properties[0] + "_" + vertexOriginalId);
            vertexIds.insert(properties[0] + "_" + vertexOriginalId);
            //this->universalTable->upsertVertex(properties[0] + "_" + vertexOriginalId, properties);
            vertexUniversalMap[properties[0] + "_" + vertexOriginalId] = properties;
            std::fill(properties.begin(), properties.end(), "");
            if (++loadCounter % 10000 == 0) {

                this->adjacencyList->insertVertex(vertexIds);
                vertexIds.clear();
                this->universalTable->upsertVertex(vertexUniversalMap);
                vertexUniversalMap.clear();

                std::cout << "file: " << p.path().filename().string() << std::endl;
                std::cout << "Adjacency List Size: " << this->adjacencyList->getAdjacencyListSize() << std::endl;
                std::cout << "Universal Table Size: " << this->universalTable->getVertexUniversalTableSize() << std::endl;
                std::cout << "--------------------------------------------------------------------------" << std::endl;
            }
        }
    }
    this->adjacencyList->insertVertex(vertexIds);
    vertexIds.clear();
    this->universalTable->upsertVertex(vertexUniversalMap);
    vertexUniversalMap.clear();

    //std::cout << "file: " << p.path().filename().string() << std::endl;
    std::cout << "Adjacency List Size: " << this->adjacencyList->getAdjacencyListSize() << std::endl;
    std::cout << "Universal Table Size: " << this->universalTable->getVertexUniversalTableSize() << std::endl;
    std::cout << "--------------------------------------------------------------------------" << std::endl;
    return true;
}

bool AdjacencyListUniversalTableManager::loadEdges(std::string edgesDirectory) {

    uint64_t loadCounter = 0;
    uint64_t rowCount = 0;
    //this->addEdgeProperties("EdgeLabel");

    std::map<std::pair<std::string, std::string>, std::map<std::string, std::vector<std::string> > > edgeUniversalMap;
    std::set<std::string> edgeIds;

    for (auto & p : fs::directory_iterator(edgesDirectory)) {
        rowCount = 0;
        std::cout << p.path() << std::endl;
        std::ifstream edgeFile(p.path().string());
        std::string edgeLine, headerLine;
        std::getline(edgeFile, headerLine);

        std::vector<int64_t> propertiesPositions = this->addEdgeProperties(headerLine);

        std::vector<std::string> properties(universalTable->getEdgePropertyOrder().size());

        std::istringstream iss(p.path().filename().string());
        std::string sourceVertex, edgeLabel, targetVertex;
        getline(iss, sourceVertex, '_');
        getline(iss, edgeLabel, '_');
        getline(iss, targetVertex, '_');


        while (std::getline(edgeFile, edgeLine)) {
            rowCount++;
            //properties[0] = edgeLabel;
            std::istringstream iss(edgeLine);
            std::string property, sourceVertexId, targetVertexId;
            uint64_t propertyCounter = 0;
            while (getline(iss, property, '|')) {
                if (propertiesPositions[propertyCounter] == -1 && propertyCounter == 0) {
                    sourceVertexId = property;
                    propertyCounter++;
                } else if (propertiesPositions[propertyCounter] == -1 && propertyCounter == 1) {
                    targetVertexId = property;
                    propertyCounter++;
                } else {
                    properties[propertiesPositions[propertyCounter++]] = property;
                }
            }
            if (this->adjacencyList->addNeighbourVertex(sourceVertex + "_" + sourceVertexId, targetVertex + "_" + targetVertexId, edgeLabel)) {
                edgeUniversalMap[std::make_pair(sourceVertex + "_" + sourceVertexId, targetVertex + "_" + targetVertexId)][edgeLabel] = properties;
                //this->universalTable->upsertEdge(sourceVertex + "_" + sourceVertexId, targetVertex + "_" + targetVertexId, properties);
            }
            //edgeIds.insert(properties[0] + "_" + sourceOriginalId);
            //this->universalTable->upsertVertex(properties[0] + "_" + vertexOriginalId, properties);
            //edgeUniversalMap[properties[0] + "_" + sourceOriginalId] = properties;
            std::fill(properties.begin(), properties.end(), "");
            if (++loadCounter % 10000 == 0) {

                /*
                this->adjacencyList->insertVertex(edgeIds);
                edgeIds.clear();
                this->universalTable->upsertVertex(edgeUniversalMap);
                edgeUniversalMap.clear();
                 */
                this->universalTable->upsertEdge(edgeUniversalMap);
                edgeUniversalMap.clear();

                std::cout << "file: " << p.path().filename().string() << std::endl;
                std::cout << "Adjacency List Size: " << this->adjacencyList->getAdjacencyListSize() << std::endl;
                std::cout << "Vertex Universal Table Size: " << this->universalTable->getVertexUniversalTableSize() << std::endl;
                std::cout << "Edge Universal Table Size: " << this->universalTable->getEdgeUniversalTableSize() << std::endl;
                std::cout << "--------------------------------------------------------------------------" << std::endl;
            }
        }
    }

    this->universalTable->upsertEdge(edgeUniversalMap);
    edgeUniversalMap.clear();

    //std::cout << "file: " << p.path().filename().string() << std::endl;
    std::cout << "Adjacency List Size: " << this->adjacencyList->getAdjacencyListSize() << std::endl;
    std::cout << "Vertex Universal Table Size: " << this->universalTable->getVertexUniversalTableSize() << std::endl;
    std::cout << "Edge Universal Table Size: " << this->universalTable->getEdgeUniversalTableSize() << std::endl;
    std::cout << "--------------------------------------------------------------------------" << std::endl;
    return true;
}

std::vector<int64_t> AdjacencyListUniversalTableManager::addVertexProperties(std::string vertexHeaderLine) {
    std::vector<int64_t> propertiesPositions(0);
    std::istringstream iss(vertexHeaderLine);
    std::string propertyName;
    while (getline(iss, propertyName, '|')) {
        if (propertyName == "id") {
            propertiesPositions.push_back(-1);
            continue;
        }
        propertiesPositions.push_back(this->universalTable->addVertexProperty(propertyName));
    }
    return propertiesPositions;
}

std::vector<int64_t> AdjacencyListUniversalTableManager::addEdgeProperties(std::string edgeHeaderLine) {
    std::vector<int64_t> propertiesPositions(0);
    std::istringstream iss(edgeHeaderLine);
    std::string propertyName;
    while (getline(iss, propertyName, '|')) {
        if (propertyName.substr(propertyName.size() - 3, 3) == ".id") {
            propertiesPositions.push_back(-1);
            continue;
        }
        propertiesPositions.push_back(this->universalTable->addEdgeProperty(propertyName));
    }
    return propertiesPositions;
}
