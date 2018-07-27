/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ParallelAdjacencyListSchemaHashedTableManager.hpp"

bool ParallelAdjacencyListSchemaHashedTableManager::loadGraph(std::string verticesDirectory, std::string edgesDirectory) {
    std::vector<std::thread> threads;

    DIR *pdir = NULL;
    struct dirent *pent = NULL;
    pdir = opendir(verticesDirectory.data());

    while ((pent = readdir(pdir)) != NULL) {

        std::cout << verticesDirectory + "/" + pent->d_name << std::endl;
        std::ifstream vertexFile(verticesDirectory + "/" + pent->d_name);

        std::string headerLine;
        std::getline(vertexFile, headerLine);

        std::map<int, std::string> propertiesPositions = this->getVertexProperties(headerLine);

        std::istringstream iss(pent->d_name);
        std::string vertexType;
        getline(iss, vertexType, '_');


        for (int i = 0; i < this->parallelismDegree; i++) {
            threads.push_back(std::thread([i, &vertexFile, &propertiesPositions, &vertexType, this]() {
                this->loadVertices(vertexFile, propertiesPositions, vertexType);
            }));
        }


        for (auto& thread : threads) {
            thread.join();
        }
        threads.clear();

        //loadVertices(vertexFile, propertiesPositions, vertexType);

        std::cout << "file: " << vertexType << std::endl;
        std::cout << "Adjacency List Size: " << this->parallelAdjacencyList.getAdjacencyListSize() << std::endl;
        std::cout << "Schema Hashed Table Size: " << this->parallelSchemaHashedTable.getVertexSchemaHashedTableSize() << std::endl;
        std::cout << "--------------------------------------------------------------------------" << std::endl;

    }




    pdir = opendir(edgesDirectory.data());

    while ((pent = readdir(pdir)) != NULL) {

        std::cout << edgesDirectory + "/" + pent->d_name << std::endl;
        std::ifstream edgeFile(edgesDirectory + "/" + pent->d_name);

        std::string headerLine;
        std::getline(edgeFile, headerLine);

        std::map<int, std::string> propertiesPositions = this->getEdgeProperties(headerLine);

        std::string edgeFileName = pent->d_name;

        for (int i = 0; i < this->parallelismDegree; i++) {
            threads.push_back(std::thread([i, &edgeFile, &propertiesPositions, &edgeFileName, this]() {
                this->loadEdges(edgeFile, propertiesPositions, edgeFileName);
            }));
        }


        for (auto& thread : threads) {
            thread.join();
        }
        threads.clear();

        //loadVertices(vertexFile, propertiesPositions, vertexType);


        std::cout << "file: " << pent->d_name << std::endl;
        std::cout << "Adjacency List Size: " << this->parallelAdjacencyList.getAdjacencyListSize() << std::endl;
        std::cout << "Vertex Schema Hashed Table Size: " << this->parallelSchemaHashedTable.getVertexSchemaHashedTableSize() << std::endl;
        std::cout << "Edge Schema Hashed Table Size: " << this->parallelSchemaHashedTable.getEdgeSchemaHashedTableSize() << std::endl;
        std::cout << "--------------------------------------------------------------------------" << std::endl;

    }


    return true;

}

bool ParallelAdjacencyListSchemaHashedTableManager::loadVertices(std::ifstream &vertexFile, std::map<int, std::string> &propertiesPositions, std::string &vertexType) {

    uint64_t loadCounter = 0;
    uint64_t rowCount = 0;

    std::map<std::string, std::map<std::string, std::string> > vertexSchemaHashedMap;
    std::set<std::string> vertexIds;

    rowCount = 0;

    std::map<std::string, std::string> properties;

    std::string vertexLine;

    fileReadMutex.lock();
    while (std::getline(vertexFile, vertexLine)) {
        fileReadMutex.unlock();

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

            this->parallelAdjacencyList.insertVertex(vertexIds);
            vertexIds.clear();
            this->parallelSchemaHashedTable.upsertVertex(vertexSchemaHashedMap);
            vertexSchemaHashedMap.clear();

        }
        fileReadMutex.lock();
    }
    fileReadMutex.unlock();

    this->parallelAdjacencyList.insertVertex(vertexIds);
    vertexIds.clear();
    this->parallelSchemaHashedTable.upsertVertex(vertexSchemaHashedMap);
    vertexSchemaHashedMap.clear();

    return true;
}

bool ParallelAdjacencyListSchemaHashedTableManager::loadEdges(std::ifstream &edgeFile, std::map<int, std::string> &propertiesPositions, std::string &edgeFileName) {

    uint64_t loadCounter = 0;
    uint64_t rowCount = 0;

    std::map<std::pair<std::string, std::string>, std::map<std::string, std::map<std::string, std::string> > > edgeSchemaHashedMap;
    std::vector<std::tuple<std::string, std::string, std::string> > edges;

    std::map<std::string, std::string> properties;

    std::istringstream iss(edgeFileName);
    std::string sourceVertex, edgeLabel, targetVertex;
    getline(iss, sourceVertex, '_');
    getline(iss, edgeLabel, '_');
    getline(iss, targetVertex, '_');

    std::string edgeLine;

    fileReadMutex.lock();
    while (std::getline(edgeFile, edgeLine)) {
        fileReadMutex.unlock();
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

        if (++loadCounter % batchSize == 0) {

            this->parallelAdjacencyList.addNeighbourVertex(edges);
            edges.clear();
            this->parallelSchemaHashedTable.upsertEdge(edgeSchemaHashedMap);
            edgeSchemaHashedMap.clear();

        }
        fileReadMutex.lock();
    }
    fileReadMutex.unlock();

    this->parallelAdjacencyList.addNeighbourVertex(edges);
    edges.clear();
    this->parallelSchemaHashedTable.upsertEdge(edgeSchemaHashedMap);
    edgeSchemaHashedMap.clear();

    return true;
}

std::map<int, std::string> ParallelAdjacencyListSchemaHashedTableManager::getVertexProperties(std::string vertexHeaderLine) {
    std::map<int, std::string> propertiesPositions;
    std::istringstream iss(vertexHeaderLine);
    std::string propertyName;
    while (getline(iss, propertyName, '|')) {
        propertiesPositions[propertiesPositions.size()] = propertyName;
    }
    return propertiesPositions;
}

std::map<int, std::string> ParallelAdjacencyListSchemaHashedTableManager::getEdgeProperties(std::string edgeHeaderLine) {
    std::map<int, std::string> propertiesPositions;
    std::istringstream iss(edgeHeaderLine);
    std::string propertyName;
    while (getline(iss, propertyName, '|')) {
        propertiesPositions[propertiesPositions.size()] = propertyName;
    }
    return propertiesPositions;
}
