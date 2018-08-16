/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "AdjacencyListSchemaHashedTableManager.hpp"

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

    std::vector < std::pair < std::string, std::unordered_map<std::string, char*> > > vertexSchemaHashedMap;

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
        std::cout << "Adjacency List Size: " << this->adjacencyList.getAdjacencyListSize() << std::endl;
        std::cout << "Vertex Schema Hashed Table Size: " << this->schemaHashedTable.getVertexSchemaHashedTableSize() << std::endl;
        std::cout << "Edge Schema Hashed Table Size: " << this->schemaHashedTable.getEdgeSchemaHashedTableSize() << std::endl;
        std::cout << "--------------------------------------------------------------------------" << std::endl;
    }

    closedir(pdir);
    return true;
}

bool AdjacencyListSchemaHashedTableManager::loadEdges(std::string edgesDirectory) {

    uint64_t loadCounter = 0;
    uint64_t rowCount = 0;

    std::vector< std::pair< std::string, std::unordered_map<std::string, char*> > > vertexSchemaHashedMap;
    std::unordered_map<std::string, char*> vertexProperties;

    std::vector< std::pair<std::string, std::unordered_map<std::string, char*> > > edgeSchemaHashedMap;
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

                this->adjacencyList.addNeighbourVertex(edges);
                edges.clear();

            }
        }

        this->schemaHashedTable.upsertVertex(vertexSchemaHashedMap);
        vertexSchemaHashedMap.clear();

        this->schemaHashedTable.upsertEdge(edgeSchemaHashedMap);
        edgeSchemaHashedMap.clear();

        this->adjacencyList.addNeighbourVertex(edges);
        edges.clear();

        std::cout << "file: " << pent->d_name << std::endl;
        std::cout << "Adjacency List Size: " << this->adjacencyList.getAdjacencyListSize() << std::endl;
        std::cout << "Vertex Schema Hashed Table Size: " << this->schemaHashedTable.getVertexSchemaHashedTableSize() << std::endl;
        std::cout << "Edge Schema Hashed Table Size: " << this->schemaHashedTable.getEdgeSchemaHashedTableSize() << std::endl;
        std::cout << "--------------------------------------------------------------------------" << std::endl;
    }

    closedir(pdir);
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

void AdjacencyListSchemaHashedTableManager::executeQueryBI1(tm messageCreationDate, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet) {
    // 1- get all vertices with type comment or post
    std::pair<std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator, std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator> commentVertices;
    commentVertices = this->schemaHashedTable.getVertices("comment");
    std::cout << "Count of Comment Vertices Found: " << std::distance(commentVertices.first, commentVertices.second) << std::endl;

    std::pair<std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator, std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator> postVertices;
    postVertices = this->schemaHashedTable.getVertices("post");
    std::cout << "Count of Post Vertices Found: " << std::distance(postVertices.first, postVertices.second) << std::endl;

    // 2- filter result on creation date before $date

    for (std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator it = commentVertices.first; it != commentVertices.second; ++it) {
        
        const char* creationDate = it->second.at("creationDate");
        tm creationDate_tm = UtilityFunctions::getDateTime(creationDate);

        if (UtilityFunctions::compareDateTime(messageCreationDate, creationDate_tm) == 1) {
            std::pair<std::vector<std::string>, std::vector<double> > resultRecord;

            resultRecord.first.emplace_back(std::to_string(creationDate_tm.tm_year));

            resultRecord.first.emplace_back("true");

            std::string::size_type sz; // alias of size_t

            uint64_t messageLength = std::stoi(it->second.at("length"), &sz);
            std::string messageLengthCategory;
            if (messageLength >= 0 && messageLength < 40) {
                messageLengthCategory = "0";
            } else if (messageLength >= 40 && messageLength < 80) {
                messageLengthCategory = "1";
            } else if (messageLength >= 80 && messageLength < 160) {
                messageLengthCategory = "2";
            } else if (messageLength >= 160) {
                messageLengthCategory = "3";
            }
            resultRecord.first.emplace_back(messageLengthCategory);

            resultRecord.second.emplace_back(messageLength);

            resultSet.emplace_back(resultRecord);
        }
    }

    for (std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator it = postVertices.first; it != postVertices.second; ++it) {
        
        const char* creationDate = it->second.at("creationDate");
        tm creationDate_tm = UtilityFunctions::getDateTime(creationDate);

        if (UtilityFunctions::compareDateTime(messageCreationDate, creationDate_tm) == 1) {
            std::pair<std::vector<std::string>, std::vector<double> > resultRecord;

            resultRecord.first.emplace_back(std::to_string(creationDate_tm.tm_year));

            resultRecord.first.emplace_back("true");

            std::string::size_type sz; // alias of size_t

            uint64_t messageLength = std::stoi(it->second.at("length"), &sz);
            std::string messageLengthCategory;
            if (messageLength >= 0 && messageLength < 40) {
                messageLengthCategory = "0";
            } else if (messageLength >= 40 && messageLength < 80) {
                messageLengthCategory = "1";
            } else if (messageLength >= 80 && messageLength < 160) {
                messageLengthCategory = "2";
            } else if (messageLength >= 160) {
                messageLengthCategory = "3";
            }
            resultRecord.first.emplace_back(messageLengthCategory);

            resultRecord.second.emplace_back(messageLength);

            resultSet.emplace_back(resultRecord);
        }
    }

    std::cout << "Count of Relevant Messages Found: " << resultSet.size() << std::endl;

}


void AdjacencyListSchemaHashedTableManager::executeQueryBI18(tm messageCreationDate, uint16_t messageLength, std::vector<std::string> messageLanguages, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet) {

    // 1- get all vertices with type comment or post
    std::pair<std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator, std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator> commentVertices;
    commentVertices = this->schemaHashedTable.getVertices("comment");
    std::cout << "Count of Comment Vertices Found: " << std::distance(commentVertices.first, commentVertices.second) << std::endl;

    std::pair<std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator, std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator> postVertices;
    postVertices = this->schemaHashedTable.getVertices("post");
    std::cout << "Count of Post Vertices Found: " << std::distance(postVertices.first, postVertices.second) << std::endl;

    // 2- filter result on creation date before $date, length less than $messageLength, content is not empty and post language in $languages

    std::vector<std::pair<std::vector<std::string>, std::vector<double> > > tempResultSet;

    for (std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator it = commentVertices.first; it != commentVertices.second; ++it) {

        const char* content = it->second.at("content");

        std::string::size_type sz; // alias of size_t
        uint16_t length = std::stoi(it->second.at("length"), &sz);

        const char* creationDate = it->second.at("creationDate");
        tm creationDate_tm = UtilityFunctions::getDateTime(creationDate);

        if (
                (content && (content[0] != '\0')) &&
                (length < messageLength) &&
                (UtilityFunctions::compareDateTime(messageCreationDate, creationDate_tm) == 1)
                ) {

            std::pair<std::vector<std::string>, std::vector<double> > resultRecord;

            resultRecord.first.emplace_back(it->first);
            resultRecord.second.emplace_back(1);
            tempResultSet.emplace_back(resultRecord);
        }
    }

    this->adjacencyList.getTargetVertex("replyOf", tempResultSet);

    std::vector<std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator> replyOfPosts;
    replyOfPosts = this->schemaHashedTable.getVertices(tempResultSet);

    for (uint32_t i; i < replyOfPosts.size(); i++) {
        const char* language = replyOfPosts[i]->second.at("language");

        if (language && std::find(messageLanguages.begin(), messageLanguages.end(), std::string(language)) != messageLanguages.end()) {
            tempResultSet[i].first.erase(tempResultSet[i].first.end() - 1);
            resultSet.emplace_back(tempResultSet[i]);
        }
    }
    
    tempResultSet.clear();



    for (std::map<std::string, std::unordered_map<std::string, char*> >::const_iterator it = postVertices.first; it != postVertices.second; ++it) {

        const char* language = it->second.at("language");

        const char* content = it->second.at("content");

        std::string::size_type sz; // alias of size_t
        uint16_t length = std::stoi(it->second.at("length"), &sz);

        const char* creationDate = it->second.at("creationDate");
        tm creationDate_tm = UtilityFunctions::getDateTime(creationDate);

        if (
                (content && (content[0] != '\0')) &&
                (length < messageLength) &&
                (UtilityFunctions::compareDateTime(messageCreationDate, creationDate_tm) == 1) &&
                (language && std::find(messageLanguages.begin(), messageLanguages.end(), std::string(language)) != messageLanguages.end())
                ) {

            std::pair<std::vector<std::string>, std::vector<double> > resultRecord;

            resultRecord.first.emplace_back(it->first);
            resultRecord.second.emplace_back(1);
            resultSet.emplace_back(resultRecord);
        }
    }
    
    
    this->adjacencyList.getTargetVertexWithReplacement("hasCreator", resultSet);

    std::cout << "Count of Relevant Messages Found: " << resultSet.size() << std::endl;

}