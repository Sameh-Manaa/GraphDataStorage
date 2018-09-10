/* 
 * File:   AdjacencyMatrix.hpp
 * Author: Sameh-Manaa
 *
 */

#ifndef ADJACENCYMATRIX_HPP
#define ADJACENCYMATRIX_HPP

#include <vector>
#include <list>
#include <stdint.h>
#include <map>
#include <unordered_map>
#include <set>
#include <iostream>
#include <bitset>

class AdjacencyMatrix {
private:

    class dynamic_bitarray {
    private:
        std::vector<uint8_t> dataInChar;
        long unsigned int data_index = 0;
        std::string binary_element = "00000000";
    public:

        dynamic_bitarray() {
        }

        dynamic_bitarray(long unsigned int size) {
            dataInChar.resize((long unsigned int) (size / 8) + ((size % 8) > 0 ? 1 : 0), 0);
        }

        dynamic_bitarray(long unsigned int size, bool value_initializer) {
            dataInChar.resize((long unsigned int) (size / 8) + ((size % 8) > 0 ? 1 : 0), 0);
        }

        void resize(long unsigned int new_size) {
            dataInChar.resize((long unsigned int) (new_size / 8) + ((new_size % 8) > 0 ? 1 : 0));
        }

        void resize(long unsigned int new_size, bool value_initializer) {
            dataInChar.resize((long unsigned int) (new_size / 8) + ((new_size % 8) > 0 ? 1 : 0), 0);
        }

        void set(long unsigned int index) {
            long unsigned int major_index = (long unsigned int) (index / 8);
            unsigned int minor_index = index % 8;

            if (major_index != this->data_index) {
                this->data_index = major_index;

                this->binary_element = std::bitset<8>(this->dataInChar.at(major_index)).to_string(); //to binary
            }
            this->binary_element.at(minor_index) = '1';

            dataInChar.at(major_index) = (uint8_t) std::bitset<8>(this->binary_element).to_ulong();
        }

        void unset(long unsigned int index) {
            long unsigned int major_index = (long unsigned int) (index / 8);
            unsigned int minor_index = index % 8;

            if (major_index != this->data_index) {
                this->data_index = major_index;

                this->binary_element = std::bitset<8>(this->dataInChar.at(major_index)).to_string(); //to binary
            }
            this->binary_element.at(minor_index) = '0';

            dataInChar.at(major_index) = (uint8_t) std::bitset<8>(this->binary_element).to_ulong();
        }

        bool get(long unsigned int index) {
            long unsigned int major_index = (long unsigned int) (index / 8);
            unsigned int minor_index = index % 8;

            if (major_index != this->data_index) {
                this->data_index = major_index;

                this->binary_element = std::bitset<8>(this->dataInChar.at(major_index)).to_string(); //to binary
            }
            return ((this->binary_element.at(minor_index) == '1') ? true : false);
        }

        bool getFirstIndex(long unsigned int startingFrom, long unsigned int& firstIndex) {
            long unsigned int major_index = (long unsigned int) (startingFrom / 8);
            unsigned int minor_index = startingFrom % 8;
            for (long unsigned int i = major_index; i < this->dataInChar.size(); i++, minor_index = 0) {
                if (this->dataInChar.at(i) > 0) {
                    this->data_index = i;
                    this->binary_element = std::bitset<8>(this->dataInChar.at(i)).to_string(); //to binary
                    std::string::size_type m_index = this->binary_element.find_first_of('1', minor_index);
                    if (m_index != std::string::npos) {
                        firstIndex = ((i * 8) + m_index);
                        return true;
                    }
                }
            }
            return false;
        }

        long unsigned int size() {
            long unsigned int size = (this->dataInChar.size() * 8);
            return size;
        }
    };

    typedef std::map<std::string, uint32_t> vertexIndex_map;
    typedef vertexIndex_map::iterator vertexIndex_it;
    typedef std::map<uint32_t, std::string> indexVertex_map;
    typedef indexVertex_map::iterator indexVertex_it;
    typedef std::vector< dynamic_bitarray > vertexAdjacency_map;
    typedef vertexAdjacency_map::iterator vertexAdjacency_it;
    typedef std::unordered_map<std::string, std::pair<std::pair<vertexIndex_map, indexVertex_map>, vertexAdjacency_map> > labeledAdjMat_map;
    typedef labeledAdjMat_map::iterator labeledAdjMat_it;

    labeledAdjMat_map labeledAdjacencyMatrix;
    int batchSize;

    void resize(std::string edgeLabel);

public:
    void addNeighbourVertex(std::string edgeLabel, std::map<std::string, std::set<std::string> > &edges);
    std::string getVertexIdByVertexIndex(uint64_t vertexIndex, std::string edgeLabel);
    uint64_t getAdjacencyMatrixSize();
    uint64_t getAdjacencyMatrixSizeInBytes();
    void getTargetVertex(std::string edgeLabel, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);
    void getTargetVertexWithReplacement(std::string edgeLabel, std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);
    void getAllEdges(std::vector<std::pair<std::vector<std::string>, std::vector<double> > >& resultSet);
    void shrinkToFit();
    void setBatchSize(int batchSize);

    AdjacencyMatrix() {
    };
};


#endif /* ADJACENCYMATRIX_HPP */

