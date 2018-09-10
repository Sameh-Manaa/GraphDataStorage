#include"CSR.hpp"

void CSR::addNeighbourVertex(std::string edgeLabel, std::map<std::string, std::set<std::string> > &edges) {

    if (edges.empty()) {
        return;
    }

    std::pair<labeledCSR_it, bool> csrEntry = this->labeledCSR.emplace(edgeLabel, CSR_tuple());

    CSR_tuple &csrTuple = csrEntry.first->second;
    vertexIndexMap &viMap = std::get<0>(csrTuple);
    vertexRowIndex &vrIndex = std::get<1>(csrTuple);
    vertexColumnIndex &vcIndex = std::get<2>(csrTuple);

    if (csrEntry.second) {
        vrIndex.emplace_back(0);
    }

    uint32_t maxIndex = vrIndex.size() - 1;

    for (auto &edge : edges) {

        std::string sourceVertexId = edge.first;

        vertexMap_it src = viMap.find(sourceVertexId);
        std::pair<vertexMap_it, bool> srcIt;
        if (src == viMap.end()) {
            srcIt = viMap.insert(std::make_pair(sourceVertexId, maxIndex));
            maxIndex++;
        } else if (src->second == -1) {
            src->second = maxIndex;
            maxIndex++;
            srcIt.first = src;
            srcIt.second = true;
        } else {
            srcIt.first = src;
            srcIt.second = false;
        }



        int insertions = 0;

        if (srcIt.second) {
            vrIndex.emplace_back(vrIndex.back());
            vrIndex[srcIt.first->second + 1] += edge.second.size();
            srcIt.second = false;
        } else {
            for (int k = srcIt.first->second + 1; k < vrIndex.size(); k++) {
                vrIndex[k] += edge.second.size();
            }
        }

        uint32_t i = vrIndex.at(srcIt.first->second);

        for (auto& targetVertexId : edge.second) {

            std::pair<vertexMap_it, bool> tgtIt = viMap.insert(std::make_pair(targetVertexId, -1));

            bool inserted = false;
            for (; i < (vrIndex.at(srcIt.first->second + 1) - edge.second.size() + insertions); i++) {
                if (vcIndex.at(i)->first < targetVertexId) {
                    continue;
                } else if (vcIndex.at(i)->first > targetVertexId) {
                    vcIndex.emplace(vcIndex.begin() + i, tgtIt.first);
                    insertions++;
                    inserted = true;
                    break;
                } else {
                    for (int i = srcIt.first->second + 1; i < vrIndex.size(); i++) {
                        vrIndex[i]--;
                    }
                    inserted = true;
                    break;
                }
            }
            if (!inserted) {
                vcIndex.emplace(vcIndex.begin() + vrIndex[srcIt.first->second + 1] - edge.second.size() + insertions, tgtIt.first);
                insertions++;
                i++;
            }
        }
    }
}

uint64_t CSR::getCSRSize() {
    uint64_t size = 0;
    for (labeledCSR_it it = this->labeledCSR.begin(); it != this->labeledCSR.end(); it++) {
        size += vertexIndexMap(std::get<0>(it->second)).size();
    }
    return size;
}

uint64_t CSR::getCSRSizeInBytes() {
    uint64_t size = sizeof (this->labeledCSR);
    for (auto const& labeledEdge : this->labeledCSR) {
        size += sizeof (labeledEdge.first);
        size += labeledEdge.first.length() + 1;
        size += sizeof (labeledEdge.second);
        size += sizeof (vertexIndexMap);
        size += sizeof (vertexRowIndex);
        size += sizeof (vertexColumnIndex);
        for (auto const& vertexIndex : vertexIndexMap(std::get<0>(labeledEdge.second))) {
            size += sizeof (vertexIndex.first);
            size += vertexIndex.first.length() + 1;
            size += sizeof (vertexIndex.second);
        }
        for (auto const& vrIndex : vertexRowIndex(std::get<1>(labeledEdge.second))) {
            size += sizeof (vrIndex);
        }
        for (auto const& vcIndex : vertexColumnIndex(std::get<2>(labeledEdge.second))) {
            size += sizeof (vcIndex);
        }
    }

    return size * sizeof (char);
}

void CSR::getTargetVertex(std::string edgeLabel, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet) {
    labeledCSR_it labeledEdgesSet_it = this->labeledCSR.find(edgeLabel);
    vertexIndexMap& vi_map = std::get<0>(labeledEdgesSet_it->second);
    vertexRowIndex& vr_vec = std::get<1>(labeledEdgesSet_it->second);
    vertexColumnIndex& vc_vec = std::get<2>(labeledEdgesSet_it->second);


    for (uint32_t i = 0; i < resultSet.size(); i++) {
        std::string sourceVertexId = resultSet[i].first[0];
        vertexMap_it sourceVertexIndex = vi_map.find(sourceVertexId);
        while (sourceVertexIndex->second != -1) {
            sourceVertexIndex = vc_vec.at(vr_vec.at(sourceVertexIndex->second));
        }
        resultSet[i].first.emplace_back(sourceVertexIndex->first);
    }
}

void CSR::getTargetVertexWithReplacement(std::string edgeLabel, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet) {
    labeledCSR_it labeledEdgesSet_it = this->labeledCSR.find(edgeLabel);
    vertexIndexMap& vi_map = std::get<0>(labeledEdgesSet_it->second);
    vertexRowIndex& vr_vec = std::get<1>(labeledEdgesSet_it->second);
    vertexColumnIndex& vc_vec = std::get<2>(labeledEdgesSet_it->second);


    for (uint32_t i = 0; i < resultSet.size(); i++) {
        std::string sourceVertexId = resultSet[i].first[0];
        vertexMap_it sourceVertexIndex = vi_map.find(sourceVertexId);
        sourceVertexIndex = vc_vec.at(vr_vec.at(sourceVertexIndex->second));
        resultSet[i].first[0] = sourceVertexIndex->first;
    }
}

void CSR::getAllEdges(std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet) {
    for (auto const &labeledCSREntry : this->labeledCSR) {
        vertexIndexMap const &viMap = vertexIndexMap(std::get<0>(labeledCSREntry.second));
        vertexRowIndex const &vrIndex = vertexRowIndex(std::get<1>(labeledCSREntry.second));
        vertexColumnIndex const &vcIndex = vertexColumnIndex(std::get<2>(labeledCSREntry.second));
        for (auto const &sourceVertex : viMap) {
            if (sourceVertex.second != -1) {
                for (uint32_t i = vrIndex.at(sourceVertex.second); i < vrIndex.at(sourceVertex.second + 1); i++) {
                    resultSet.emplace_back(std::make_pair(std::vector<std::string>() = {"out", labeledCSREntry.first, sourceVertex.first}
                    , std::vector<double>() = {1}));
                    resultSet.emplace_back(std::make_pair(std::vector<std::string>() = {"in", labeledCSREntry.first, vcIndex.at(i)->first}
                    , std::vector<double>() = {1}));
                }
            }
        }
    }
}