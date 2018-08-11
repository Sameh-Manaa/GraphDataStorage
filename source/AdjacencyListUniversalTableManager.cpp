/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <forward_list>

#include "AdjacencyListUniversalTableManager.hpp"

bool AdjacencyListUniversalTableManager::loadGraph(std::string verticesDirectory, std::string edgesDirectory) {
    if (loadVertices(verticesDirectory) &&
            loadEdges(edgesDirectory)
            ) {
        //std::cout << "==========================================================================" << std::endl;
        //std::cout << "Vertex Universal Table Size In Bytes: " << this->universalTable.getVertexUniversalTableSizeInBytes() << std::endl;
        //std::cout << "Edge Universal Table Size In Bytes: " << this->universalTable.getEdgeUniversalTableSizeInBytes() << std::endl;
        //std::cout << "==========================================================================" << std::endl;
        return true;
    } else {
        return false;
    }
}

bool AdjacencyListUniversalTableManager::loadVertices(std::string verticesDirectory) {

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
            //this->universalTable.upsertVertex(vertexType + "_" + vertexOriginalId, properties);

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

bool AdjacencyListUniversalTableManager::loadEdges(std::string edgesDirectory) {

    uint64_t loadCounter = 0;
    uint64_t rowCount = 0;

    std::vector < std::pair < std::string, std::vector<char*> > > vertexUniversalMap;
    std::vector<char*> vertexProperties(universalTable.getVertexPropertyIndex().size());

    std::vector < std::pair <std::string, std::vector<char*> > > edgeUniversalMap;
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
            //this->universalTable.upsertVertex(sourceVertex + "_" + sourceVertexId, vertexProperties);
            //this->universalTable.upsertVertex(targetVertex + "_" + targetVertexId, vertexProperties);

            //this->universalTable.upsertEdge(sourceVertex + "_" + sourceVertexId, edgeLabel, targetVertex + "_" + targetVertexId, properties);
            //this->adjacencyList.addNeighbourVertex(sourceVertex + "_" + sourceVertexId, edgeLabel, targetVertex + "_" + targetVertexId);

            vertexUniversalMap.emplace_back(sourceVertex + "_" + sourceVertexId, vertexProperties);
            vertexUniversalMap.emplace_back(targetVertex + "_" + targetVertexId, vertexProperties);

            edgeUniversalMap.emplace_back(sourceVertex + "_" + sourceVertexId + "$" + edgeLabel + "$" + targetVertex + "_" + targetVertexId, properties);
            edges.emplace_back(std::make_tuple(sourceVertex + "_" + sourceVertexId, edgeLabel, targetVertex + "_" + targetVertexId));

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

std::vector<int16_t> AdjacencyListUniversalTableManager::addVertexProperties(std::string vertexHeaderLine) {
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

std::vector<int16_t> AdjacencyListUniversalTableManager::addEdgeProperties(std::string edgeHeaderLine) {
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

void AdjacencyListUniversalTableManager::executeQueryBI1(tm messageCreationDate, std::vector<std::pair<std::vector<std::string>, std::vector<std::string> > >& resultSet) {

    // 1- get all vertices with type comment or post
    std::pair<std::map<std::string, std::vector<char*> >::const_iterator, std::map<std::string, std::vector<char*> >::const_iterator> commentVertices;
    commentVertices = this->universalTable.getVertices("comment");
    std::cout << "Count of Comment Vertices Found: " << std::distance(commentVertices.first, commentVertices.second) << std::endl;

    std::pair<std::map<std::string, std::vector<char*> >::const_iterator, std::map<std::string, std::vector<char*> >::const_iterator> postVertices;
    postVertices = this->universalTable.getVertices("post");
    std::cout << "Count of Post Vertices Found: " << std::distance(postVertices.first, postVertices.second) << std::endl;

    // 2- filter result on creation date before $date
    std::unordered_map<std::string, uint16_t> vertexPropertyOrder = this->universalTable.getVertexPropertyIndex();

    uint64_t creationDatePropertyIndex = vertexPropertyOrder.at("creationDate");
    uint64_t lengthPropertyIndex = vertexPropertyOrder.at("length");

    for (std::map<std::string, std::vector<char*> >::const_iterator it = commentVertices.first; it != commentVertices.second; ++it) {
        const char* str = it->second.at(creationDatePropertyIndex);

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

            uint64_t messageLength = std::stoi(it->second.at(lengthPropertyIndex), &sz);
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

    for (std::map<std::string, std::vector<char*> >::const_iterator it = postVertices.first; it != postVertices.second; ++it) {
        const char* str = it->second.at(creationDatePropertyIndex);

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

            uint64_t messageLength = std::stoi(it->second.at(lengthPropertyIndex), &sz);
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

void AdjacencyListUniversalTableManager::executeQueryBI18(tm messageCreationDate, uint16_t messageLength, std::vector<std::string> messageLanguages, std::vector<std::pair<std::vector<std::string>, std::vector<std::string> > >& resultSet) {

    // 1- get all vertices with type comment or post
    std::pair<std::map<std::string, std::vector<char*> >::const_iterator, std::map<std::string, std::vector<char*> >::const_iterator> commentVertices;
    commentVertices = this->universalTable.getVertices("comment");
    std::cout << "Count of Comment Vertices Found: " << std::distance(commentVertices.first, commentVertices.second) << std::endl;

    std::pair<std::map<std::string, std::vector<char*> >::const_iterator, std::map<std::string, std::vector<char*> >::const_iterator> postVertices;
    postVertices = this->universalTable.getVertices("post");
    std::cout << "Count of Post Vertices Found: " << std::distance(postVertices.first, postVertices.second) << std::endl;

    // 2- filter result on creation date before $date, length less than $messageLength, content is not empty and post language in $languages
    std::unordered_map<std::string, uint16_t> vertexPropertyOrder = this->universalTable.getVertexPropertyIndex();

    uint64_t creationDatePropertyIndex = vertexPropertyOrder.at("creationDate");
    uint64_t lengthPropertyIndex = vertexPropertyOrder.at("length");
    uint64_t contentPropertyIndex = vertexPropertyOrder.at("content");
    uint64_t languagePropertyIndex = vertexPropertyOrder.at("language");


    for (std::map<std::string, std::vector<char*> >::const_iterator it = commentVertices.first; it != commentVertices.second; ++it) {

        const char* content = it->second.at(contentPropertyIndex);

        std::string::size_type sz; // alias of size_t
        uint16_t length = std::stoi(it->second.at(lengthPropertyIndex), &sz);

        const char* creationDate = it->second.at(creationDatePropertyIndex);
        tm tm1;
        sscanf(creationDate, "%4d-%2d-%2d", &tm1.tm_year, &tm1.tm_mon, &tm1.tm_mday);

        if ((messageCreationDate.tm_year > tm1.tm_year ||
                (messageCreationDate.tm_year == tm1.tm_year && messageCreationDate.tm_mon > tm1.tm_mon) ||
                (messageCreationDate.tm_year == tm1.tm_year && messageCreationDate.tm_mon == tm1.tm_mon && messageCreationDate.tm_mday > tm1.tm_mday))
                && (content && (content[0] != '\0'))
                && (length < messageLength)) {
            
            std::pair<std::vector<std::string>, std::vector<std::string> > resultRecord;

            resultRecord.first.emplace_back(std::to_string(tm1.tm_year));

            resultRecord.first.emplace_back("true");

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

    for (std::map<std::string, std::vector<char*> >::const_iterator it = postVertices.first; it != postVertices.second; ++it) {

        const char* language = it->second.at(languagePropertyIndex);

        const char* content = it->second.at(contentPropertyIndex);

        std::string::size_type sz; // alias of size_t
        uint16_t length = std::stoi(it->second.at(lengthPropertyIndex), &sz);

        const char* creationDate = it->second.at(creationDatePropertyIndex);
        tm tm1;
        sscanf(creationDate, "%4d-%2d-%2d", &tm1.tm_year, &tm1.tm_mon, &tm1.tm_mday);

        if ((messageCreationDate.tm_year > tm1.tm_year ||
                (messageCreationDate.tm_year == tm1.tm_year && messageCreationDate.tm_mon > tm1.tm_mon) ||
                (messageCreationDate.tm_year == tm1.tm_year && messageCreationDate.tm_mon == tm1.tm_mon && messageCreationDate.tm_mday > tm1.tm_mday))
                && (content && (content[0] != '\0'))
                && (length < messageLength)
                && (language && std::find(messageLanguages.begin(), messageLanguages.end(), std::string(language)) != messageLanguages.end())
                ) {

            std::pair<std::vector<std::string>, std::vector<std::string> > resultRecord;

            resultRecord.first.emplace_back(std::to_string(tm1.tm_year));

            resultRecord.first.emplace_back("false");

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