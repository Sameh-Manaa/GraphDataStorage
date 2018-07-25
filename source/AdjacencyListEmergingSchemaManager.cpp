/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "AdjacencyListEmergingSchemaManager.hpp"

bool AdjacencyListEmergingSchemaManager::loadGraph(std::string verticesDirectory, std::string edgesDirectory) {
    if (loadVertices(verticesDirectory) && loadEdges(edgesDirectory)
            ) {
        this->emergingSchema.clearUniversalTable();
        return true;
    } else {
        return false;
    }

}

bool AdjacencyListEmergingSchemaManager::loadVertices(std::string verticesDirectory) {

    uint64_t loadCounter = 0;
    uint64_t rowCount = 0;
    this->addVertexProperties("VertexType");

    std::map < std::string, std::vector<std::string> > vertexUniversalMap;
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

        std::vector<int64_t> propertiesPositions = this->addVertexProperties(headerLine);
        std::vector<std::string> properties(emergingSchema.getVertexPropertyOrder().size());

        std::istringstream iss(pent->d_name);
        std::string vertexType;
        getline(iss, vertexType, '_');


        while (std::getline(vertexFile, vertexLine)) {
            rowCount++;

            std::istringstream iss(vertexLine);
            std::string property, vertexOriginalId;

            uint64_t propertyCounter = 0;
            properties[0] = vertexType;

            while (getline(iss, property, '|')) {
                if (propertiesPositions[propertyCounter] == -1) {
                    vertexOriginalId = property;
                    propertyCounter++;
                    continue;
                }
                properties[propertiesPositions[propertyCounter++]] = property;
            }
            vertexIds.emplace(properties[0] + "_" + vertexOriginalId);
            vertexUniversalMap.emplace(properties[0] + "_" + vertexOriginalId, properties);

            if (++loadCounter % batchSize == 0) {

                this->adjacencyList.insertVertex(vertexIds);
                vertexIds.clear();
                this->emergingSchema.upsertVertex(vertexUniversalMap);
                vertexUniversalMap.clear();

            }
        }

        this->adjacencyList.insertVertex(vertexIds);
        vertexIds.clear();
        this->emergingSchema.upsertVertex(vertexUniversalMap);
        vertexUniversalMap.clear();

        //std::fill(properties.begin(), properties.end(), "");

        std::cout << "file: " << pent->d_name << std::endl;
        std::cout << "Adjacency List Size: " << this->adjacencyList.getAdjacencyListSize() << std::endl;
        std::cout << "Universal Table Size: " << this->emergingSchema.getVertexEmergingSchemaSize() << std::endl;
        std::cout << "--------------------------------------------------------------------------" << std::endl;
    }

    this->emergingSchema.generateVerticesEmergingSchema();

    return true;
}

bool AdjacencyListEmergingSchemaManager::loadEdges(std::string edgesDirectory) {

    uint64_t loadCounter = 0;
    uint64_t rowCount = 0;

    std::map<std::pair<std::string, std::string>, std::map<std::string, std::vector<std::string> > > edgeUniversalMap;
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

        std::vector<int64_t> propertiesPositions = this->addEdgeProperties(headerLine);
        std::vector<std::string> properties(emergingSchema.getEdgePropertyOrder().size());

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

            edges.emplace_back(std::make_tuple(sourceVertex + "_" + sourceVertexId, edgeLabel, targetVertex + "_" + targetVertexId));
            edgeUniversalMap[std::make_pair(sourceVertex + "_" + sourceVertexId, targetVertex + "_" + targetVertexId)][edgeLabel] = properties;

            if (++loadCounter % batchSize == 0) {

                this->adjacencyList.addNeighbourVertex(edges);
                edges.clear();
                this->emergingSchema.upsertEdge(edgeUniversalMap);
                edgeUniversalMap.clear();

            }
        }
        this->adjacencyList.addNeighbourVertex(edges);
        edges.clear();
        this->emergingSchema.upsertEdge(edgeUniversalMap);
        edgeUniversalMap.clear();

        std::cout << "file: " << pent->d_name << std::endl;
        std::cout << "Adjacency List Size: " << this->adjacencyList.getAdjacencyListSize() << std::endl;
        std::cout << "Vertex Universal Table Size: " << this->emergingSchema.getVertexEmergingSchemaSize() << std::endl;
        std::cout << "Edge Universal Table Size: " << this->emergingSchema.getEdgeEmergingSchemaSize() << std::endl;
        std::cout << "--------------------------------------------------------------------------" << std::endl;
    }

    this->emergingSchema.generateEdgesEmergingSchema();

    return true;
}

std::vector<int64_t> AdjacencyListEmergingSchemaManager::addVertexProperties(std::string vertexHeaderLine) {
    std::vector<int64_t> propertiesPositions(0);
    std::istringstream iss(vertexHeaderLine);
    std::string propertyName;
    while (getline(iss, propertyName, '|')) {
        if (propertyName == "id") {
            propertiesPositions.push_back(-1);
            continue;
        }
        propertiesPositions.push_back(this->emergingSchema.addVertexProperty(propertyName));
    }
    return propertiesPositions;
}

std::vector<int64_t> AdjacencyListEmergingSchemaManager::addEdgeProperties(std::string edgeHeaderLine) {
    std::vector<int64_t> propertiesPositions(0);
    std::istringstream iss(edgeHeaderLine);
    std::string propertyName;
    while (getline(iss, propertyName, '|')) {
        if (propertyName.substr(propertyName.size() - 3, 3) == ".id") {
            propertiesPositions.push_back(-1);
            continue;
        }
        propertiesPositions.push_back(this->emergingSchema.addEdgeProperty(propertyName));
    }
    return propertiesPositions;
}
