/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "AdjacencyListEmergingSchemaManager.hpp"

bool AdjacencyListEmergingSchemaManager::loadGraph(std::string verticesDirectory, std::string edgesDirectory) {
    if (loadVertices(verticesDirectory) &&
            loadEdges(edgesDirectory)
            ) {
        this->emergingSchema.generateVerticesEmergingSchema(this->universalTable);
        this->emergingSchema.generateEdgesEmergingSchema(this->universalTable);
        this->universalTable.clearUniversalTable();
        //std::cout << "==========================================================================" << std::endl;
        //std::cout << "Vertex Universal Table Size In Bytes: " << this->universalTable.getVertexUniversalTableSizeInBytes() << std::endl;
        //std::cout << "Edge Universal Table Size In Bytes: " << this->universalTable.getEdgeUniversalTableSizeInBytes() << std::endl;
        //std::cout << "==========================================================================" << std::endl;
        return true;
    } else {
        return false;
    }
}

bool AdjacencyListEmergingSchemaManager::loadVertices(std::string verticesDirectory) {

    uint64_t loadCounter = 0;
    uint64_t rowCount = 0;

    std::vector < std::pair < std::string, std::vector<char*> > > vertexUniversalMap;

    DIR *pdir = NULL;
    struct dirent *pent = NULL;
    pdir = opendir(verticesDirectory.data());

    while ((pent = readdir(pdir)) != NULL) {
        rowCount = 0;

        std::cout << verticesDirectory + "/" + pent->d_name << std::endl;
        std::ifstream vertexFile(verticesDirectory + "/" + pent->d_name);

        std::string vertexLine, headerLine;
        std::getline(vertexFile, headerLine);

        std::vector<int16_t> propertiesPositions = this->addVertexProperties(headerLine);
        std::vector<char*> properties(universalTable.getVertexPropertyIndex().size());

        std::istringstream iss(pent->d_name);
        std::string vertexType;
        getline(iss, vertexType, '_');


        while (std::getline(vertexFile, vertexLine)) {
            rowCount++;

            std::istringstream iss(vertexLine);
            std::string property, vertexOriginalId;

            uint64_t propertyCounter = 0;

            while (getline(iss, property, '|')) {
                if (propertiesPositions[propertyCounter] == -1) {
                    vertexOriginalId = property;
                    propertyCounter++;
                    continue;
                }
                char * propertyVal = new char [property.length() + 1];
                strcpy(propertyVal, property.c_str());
                properties[propertiesPositions[propertyCounter++]] = propertyVal;
            }
            vertexUniversalMap.emplace_back(vertexType + "_" + vertexOriginalId, properties);

            if (++loadCounter % batchSize == 0) {

                this->universalTable.upsertVertex(vertexUniversalMap);
                vertexUniversalMap.clear();

            }
        }

        this->universalTable.upsertVertex(vertexUniversalMap);
        vertexUniversalMap.clear();

        std::cout << "file: " << pent->d_name << std::endl;
        std::cout << "Adjacency List Size: " << this->adjacencyList.getAdjacencyListSize() << std::endl;
        std::cout << "Vertex Universal Table Size: " << this->universalTable.getVertexUniversalTableSize() << std::endl;
        std::cout << "Edge Universal Table Size: " << this->universalTable.getEdgeUniversalTableSize() << std::endl;
        std::cout << "--------------------------------------------------------------------------" << std::endl;
    }

    closedir(pdir);
    return true;
}

bool AdjacencyListEmergingSchemaManager::loadEdges(std::string edgesDirectory) {

    uint64_t loadCounter = 0;
    uint64_t rowCount = 0;

    std::vector < std::pair < std::string, std::vector<char*> > > vertexUniversalMap;
    std::vector<char*> vertexProperties(universalTable.getVertexPropertyIndex().size());

    std::vector< std::pair < std::string, std::vector<char*> > > edgeUniversalMap;
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

        std::vector<int16_t> propertiesPositions = this->addEdgeProperties(headerLine);
        std::vector<char*> properties(universalTable.getEdgePropertyIndex().size());

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
                    char * propertyVal = new char [property.length() + 1];
                    strcpy(propertyVal, property.c_str());
                    properties[propertiesPositions[propertyCounter++]] = propertyVal;
                }
            }

            vertexUniversalMap.emplace_back(sourceVertex + "_" + sourceVertexId, vertexProperties);
            vertexUniversalMap.emplace_back(targetVertex + "_" + targetVertexId, vertexProperties);

            edges.emplace_back(std::make_tuple(sourceVertex + "_" + sourceVertexId, edgeLabel, targetVertex + "_" + targetVertexId));
            edgeUniversalMap.emplace_back(sourceVertex + "_" + sourceVertexId + "$" + edgeLabel + "$" + targetVertex + "_" + targetVertexId, properties);

            if (++loadCounter % batchSize == 0) {

                this->universalTable.upsertVertex(vertexUniversalMap);
                vertexUniversalMap.clear();

                this->universalTable.upsertEdge(edgeUniversalMap);
                edgeUniversalMap.clear();

                this->adjacencyList.addNeighbourVertex(edges);
                edges.clear();

            }
        }

        this->universalTable.upsertVertex(vertexUniversalMap);
        vertexUniversalMap.clear();

        this->universalTable.upsertEdge(edgeUniversalMap);
        edgeUniversalMap.clear();

        this->adjacencyList.addNeighbourVertex(edges);
        edges.clear();

        std::cout << "file: " << pent->d_name << std::endl;
        std::cout << "Adjacency List Size: " << this->adjacencyList.getAdjacencyListSize() << std::endl;
        std::cout << "Vertex Universal Table Size: " << this->universalTable.getVertexUniversalTableSize() << std::endl;
        std::cout << "Edge Universal Table Size: " << this->universalTable.getEdgeUniversalTableSize() << std::endl;
        std::cout << "--------------------------------------------------------------------------" << std::endl;
    }

    closedir(pdir);
    return true;
}

std::vector<int16_t> AdjacencyListEmergingSchemaManager::addVertexProperties(std::string vertexHeaderLine) {
    std::vector<int16_t> propertiesPositions(0);
    std::istringstream iss(vertexHeaderLine);
    std::string propertyName;
    while (getline(iss, propertyName, '|')) {
        if (propertyName == "id") {
            propertiesPositions.push_back(-1);
            continue;
        }
        propertiesPositions.push_back(this->universalTable.addVertexProperty(propertyName));
    }
    return propertiesPositions;
}

std::vector<int16_t> AdjacencyListEmergingSchemaManager::addEdgeProperties(std::string edgeHeaderLine) {
    std::vector<int16_t> propertiesPositions(0);
    std::istringstream iss(edgeHeaderLine);
    std::string propertyName;
    while (getline(iss, propertyName, '|')) {
        if (propertyName.substr(propertyName.size() - 3, 3) == ".id") {
            propertiesPositions.push_back(-1);
            continue;
        }
        propertiesPositions.push_back(this->universalTable.addEdgeProperty(propertyName));
    }
    return propertiesPositions;
}

void AdjacencyListEmergingSchemaManager::executeQueryBI1(tm messageCreationDate, std::vector<std::pair<std::vector<std::string>, std::vector<std::string> > >& resultSet) {

    // 1- get all vertices with type comment or post
    std::unordered_map<std::string, std::pair<uint16_t, uint16_t> > vertexPropertyOrder = this->emergingSchema.getVertexPropertyIndex();

    std::pair<uint16_t, uint16_t> creationDatePropertyIndex = vertexPropertyOrder.at("creationDate");
    std::pair<uint16_t, uint16_t> lengthPropertyIndex = vertexPropertyOrder.at("length");

    std::pair<std::map<std::string, std::vector<char*> >::const_iterator, std::map<std::string, std::vector<char*> >::const_iterator> commentVertices_creationDate;
    commentVertices_creationDate = this->emergingSchema.getVertices("comment", "creationDate");
    std::cout << "Count of Comment Vertices Found: " << std::distance(commentVertices_creationDate.first, commentVertices_creationDate.second) << std::endl;

    std::pair<std::map<std::string, std::vector<char*> >::const_iterator, std::map<std::string, std::vector<char*> >::const_iterator> postVertices_creationDate;
    postVertices_creationDate = this->emergingSchema.getVertices("post", "creationDate");
    std::cout << "Count of Post Vertices Found: " << std::distance(postVertices_creationDate.first, postVertices_creationDate.second) << std::endl;

    std::pair<std::map<std::string, std::vector<char*> >::const_iterator, std::map<std::string, std::vector<char*> >::const_iterator> commentVertices_length;
    std::pair<std::map<std::string, std::vector<char*> >::const_iterator, std::map<std::string, std::vector<char*> >::const_iterator> postVertices_length;

    if (lengthPropertyIndex.first == creationDatePropertyIndex.first) {
        commentVertices_length = commentVertices_creationDate;
        postVertices_length = postVertices_creationDate;
    } else {
        commentVertices_length = this->emergingSchema.getVertices("comment", "length");
        postVertices_length = this->emergingSchema.getVertices("post", "length");
    }
    // 2- filter result on creation date before $date

    for (std::map<std::string, std::vector<char*> >::const_iterator it = commentVertices_creationDate.first, it_length = commentVertices_length.first;
            it != commentVertices_creationDate.second;
            ++it, ++it_length) {
        const char* str = it->second.at(creationDatePropertyIndex.second);

        tm tm1;

        sscanf(str, "%4d-%2d-%2d", &tm1.tm_year, &tm1.tm_mon, &tm1.tm_mday);

        if (messageCreationDate.tm_year > tm1.tm_year ||
                (messageCreationDate.tm_year == tm1.tm_year && messageCreationDate.tm_mon > tm1.tm_mon) ||
                (messageCreationDate.tm_year == tm1.tm_year && messageCreationDate.tm_mon == tm1.tm_mon && messageCreationDate.tm_mday > tm1.tm_mday)
                ) {
            std::pair<std::vector<std::string>, std::vector<std::string> > resultRecord;

            resultRecord.first.emplace_back(std::to_string(tm1.tm_year));

            resultRecord.first.emplace_back("true");

            std::string::size_type sz; // alias of size_t

            uint64_t messageLength = std::stoi(it_length->second.at(lengthPropertyIndex.second), &sz);
            std::string messageLengthCategory;
            if (messageLength >= 0 && messageLength < 40) {
                messageLengthCategory = "0";
            } else if (messageLength >= 0 && messageLength < 40) {
                messageLengthCategory = "1";
            } else if (messageLength >= 0 && messageLength < 40) {
                messageLengthCategory = "2";
            } else {
                messageLengthCategory = "3";
            }
            resultRecord.first.emplace_back(messageLengthCategory);

            resultRecord.second.emplace_back(std::to_string(messageLength));

            resultSet.emplace_back(resultRecord);
        }
    }

    for (std::map<std::string, std::vector<char*> >::const_iterator it = postVertices_creationDate.first, it_length = postVertices_length.first;
            it != postVertices_creationDate.second;
            ++it, ++it_length) {
        const char* str = it->second.at(creationDatePropertyIndex.second);

        tm tm1;

        sscanf(str, "%4d-%2d-%2d", &tm1.tm_year, &tm1.tm_mon, &tm1.tm_mday);

        if (messageCreationDate.tm_year > tm1.tm_year ||
                (messageCreationDate.tm_year == tm1.tm_year && messageCreationDate.tm_mon > tm1.tm_mon) ||
                (messageCreationDate.tm_year == tm1.tm_year && messageCreationDate.tm_mon == tm1.tm_mon && messageCreationDate.tm_mday > tm1.tm_mday)
                ) {
            std::pair<std::vector<std::string>, std::vector<std::string> > resultRecord;

            resultRecord.first.emplace_back(std::to_string(tm1.tm_year));

            resultRecord.first.emplace_back("false");

            std::string::size_type sz; // alias of size_t

            uint64_t messageLength = std::stoi(it_length->second.at(lengthPropertyIndex.second), &sz);
            std::string messageLengthCategory;
            if (messageLength >= 0 && messageLength < 40) {
                messageLengthCategory = "0";
            } else if (messageLength >= 0 && messageLength < 40) {
                messageLengthCategory = "1";
            } else if (messageLength >= 0 && messageLength < 40) {
                messageLengthCategory = "2";
            } else {
                messageLengthCategory = "3";
            }
            resultRecord.first.emplace_back(messageLengthCategory);

            resultRecord.second.emplace_back(std::to_string(messageLength));

            resultSet.emplace_back(resultRecord);
        }
    }

    std::cout << "Count of Relevant Messages Found: " << resultSet.size() << std::endl;

}