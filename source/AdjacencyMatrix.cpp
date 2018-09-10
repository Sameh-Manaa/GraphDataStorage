#include"AdjacencyMatrix.hpp"

void AdjacencyMatrix::resize(std::string edgeLabel) {
    if (this->labeledAdjacencyMatrix.at(edgeLabel).second.size() < this->labeledAdjacencyMatrix.at(edgeLabel).first.first.size()) {
        //construct a new vertex neighborhood row
        dynamic_bitarray neighbours(this->labeledAdjacencyMatrix.at(edgeLabel).first.first.size() + this->batchSize, false);

        for (auto& nodeNeighbours : this->labeledAdjacencyMatrix.at(edgeLabel).second) {
            nodeNeighbours.resize(neighbours.size(), false);
        }

        this->labeledAdjacencyMatrix.at(edgeLabel).second.resize(neighbours.size(), neighbours);
    }
}

void AdjacencyMatrix::shrinkToFit() {
    for (auto &adjMatEntry : this->labeledAdjacencyMatrix) {
        if (adjMatEntry.second.second.size() > adjMatEntry.second.first.first.size()) {

            adjMatEntry.second.second.resize(adjMatEntry.second.first.first.size());

            for (auto& nodeNeighbours : adjMatEntry.second.second) {
                nodeNeighbours.resize(adjMatEntry.second.first.first.size());
            }

        }
    }
}

void AdjacencyMatrix::setBatchSize(int batchSize) {
    this->batchSize = batchSize;
}

void AdjacencyMatrix::addNeighbourVertex(std::string edgeLabel, std::map<std::string, std::set<std::string> > &edges) {
    if (edges.empty()) {
        return;
    }

    std::pair< labeledAdjMat_it, bool> adjMatEntry = this->labeledAdjacencyMatrix.emplace(edgeLabel, std::pair<std::pair<vertexIndex_map, indexVertex_map>, vertexAdjacency_map>());

    for (auto& edge : edges) {
        std::pair < vertexIndex_it, bool> srcIt =
                adjMatEntry.first->second.first.first.emplace(edge.first, adjMatEntry.first->second.first.first.size());
        if (srcIt.second) {
            adjMatEntry.first->second.first.second.emplace(srcIt.first->second, edge.first);
        }
        for (auto& tgtVertex : edge.second) {
            std::pair < vertexIndex_it, bool> tgtIt =
                    adjMatEntry.first->second.first.first.emplace(tgtVertex, adjMatEntry.first->second.first.first.size());
            if (tgtIt.second) {
                adjMatEntry.first->second.first.second.emplace(tgtIt.first->second, tgtVertex);
            }
        }
    }

    this->resize(edgeLabel);

    for (auto& edge : edges) {
        uint64_t sourceVertexIndex = adjMatEntry.first->second.first.first.at(edge.first);
        for (auto& tgtVertex : edge.second) {
            uint64_t targetVertexIndex = adjMatEntry.first->second.first.first.at(tgtVertex);
            adjMatEntry.first->second.second[sourceVertexIndex].set(targetVertexIndex);
            unsigned long int ii = 666;
            adjMatEntry.first->second.second[sourceVertexIndex].getFirstIndex(0, ii);
            ii = ii;
        }
    }
}

std::string AdjacencyMatrix::getVertexIdByVertexIndex(uint64_t vertexIndex, std::string edgeLabel) {
    return this->labeledAdjacencyMatrix.at(edgeLabel).first.second.at(vertexIndex);
}

uint64_t AdjacencyMatrix::getAdjacencyMatrixSize() {
    uint64_t size = 0;
    for (labeledAdjMat_it it = this->labeledAdjacencyMatrix.begin(); it != this->labeledAdjacencyMatrix.end(); it++) {
        size += it->second.first.first.size();
    }
    return size;
}

uint64_t AdjacencyMatrix::getAdjacencyMatrixSizeInBytes() {
    uint64_t size = sizeof (this->labeledAdjacencyMatrix) + sizeof (this->batchSize);
    for (auto & labeledEdge : this->labeledAdjacencyMatrix) {
        size += sizeof (labeledEdge.first);
        size += labeledEdge.first.length() + 1;
        size += sizeof (labeledEdge.second);
        size += sizeof (labeledEdge.second.first);
        size += sizeof (labeledEdge.second.second);
        for (auto const& edgeIndex : labeledEdge.second.first.first) {
            size += sizeof (edgeIndex.first);
            size += edgeIndex.first.length() + 1;
            size += sizeof (edgeIndex.second);
        }

        for (auto const& edgeIndex : labeledEdge.second.first.second) {
            size += sizeof (edgeIndex.first);
            size += sizeof (edgeIndex.second);
            size += edgeIndex.second.length() + 1;
        }

        for (auto & edgeMap : labeledEdge.second.second) {
            size += sizeof (edgeMap);
            size += edgeMap.size() / 8;
        }
    }
    return size * sizeof (char);
}

void AdjacencyMatrix::getTargetVertex(std::string edgeLabel, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet) {
    labeledAdjMat_it labeledEdgesSet_it = this->labeledAdjacencyMatrix.find(edgeLabel);

    for (uint32_t i = 0; i < resultSet.size(); i++) {
        std::string sourceVertexId = resultSet[i].first[0];
        uint32_t sourceVertexIndex = labeledEdgesSet_it->second.first.first.at(sourceVertexId);
        dynamic_bitarray& neighboursVector = labeledEdgesSet_it->second.second.at(sourceVertexIndex);
        int32_t targetVertexIndex = sourceVertexIndex;
        sourceVertexIndex = -1;

        long unsigned int ii = 0;
        if (!neighboursVector.getFirstIndex(0, ii)) {
            continue;
        }

        targetVertexIndex = ii;
        sourceVertexIndex = ii;

        std::string targetVertex = getVertexIdByVertexIndex(targetVertexIndex, edgeLabel);

        while (targetVertex.substr(0, 4) != "post") {
            dynamic_bitarray& neighboursVector = labeledEdgesSet_it->second.second.at(sourceVertexIndex);
            sourceVertexIndex = -1;

            neighboursVector.getFirstIndex(0, ii);
            targetVertexIndex = ii;
            sourceVertexIndex = ii;

            targetVertex = getVertexIdByVertexIndex(targetVertexIndex, edgeLabel);
        }
        resultSet[i].first.emplace_back(targetVertex);
    }
}

void AdjacencyMatrix::getTargetVertexWithReplacement(std::string edgeLabel, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet) {
    labeledAdjMat_it labeledEdgesSet_it = this->labeledAdjacencyMatrix.find(edgeLabel);

    for (uint32_t i = 0; i < resultSet.size(); i++) {
        std::string sourceVertexId = resultSet[i].first[0];
        uint32_t sourceVertexIndex = labeledEdgesSet_it->second.first.first.at(sourceVertexId);
        dynamic_bitarray& neighboursVector = labeledEdgesSet_it->second.second.at(sourceVertexIndex);
        int32_t targetVertexIndex = sourceVertexIndex;
        sourceVertexIndex = -1;

        long unsigned int ii = 0;
        neighboursVector.getFirstIndex(0, ii);
        targetVertexIndex = ii;
        sourceVertexIndex = ii;

        resultSet[i].first[0] = getVertexIdByVertexIndex(targetVertexIndex, edgeLabel);
    }
}

void AdjacencyMatrix::getAllEdges(std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet) {
    for (auto &labeledAdjMat : this->labeledAdjacencyMatrix) {
        vertexIndex_map &viMap = labeledAdjMat.second.first.first;
        vertexAdjacency_map &vaMap = labeledAdjMat.second.second;
        for (auto &sourceVertex : viMap) {
            auto &neighboursVector = vaMap.at(sourceVertex.second);
            unsigned long int index = 0;
            bool neighbourhoodFlag = neighboursVector.getFirstIndex(0, index);
            while (neighbourhoodFlag) {
                resultSet.emplace_back(std::make_pair(std::vector<std::string>() = {"out", labeledAdjMat.first, sourceVertex.first}
                , std::vector<double>() = {1}));
                resultSet.emplace_back(std::make_pair(std::vector<std::string>() = {"in", labeledAdjMat.first, this->getVertexIdByVertexIndex(index, labeledAdjMat.first)}
                , std::vector<double>() = {1}));
                if ((index + 1) < neighboursVector.size()) {
                    neighbourhoodFlag = neighboursVector.getFirstIndex(index + 1, index);
                } else {
                    break;
                }
            }
        }
    }
}