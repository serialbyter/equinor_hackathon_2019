#include <stdexcept>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <fstream>

#include "equinor2018_simulator/csv_parser.h"


class WordDelimitedByComma : public std::string {};

std::istream& operator>>(std::istream& is, WordDelimitedByComma& output) {
    std::getline(is, output, ',');
    return is;
}

std::vector<std::string> ReadLine(std::istream& file){
    std::string rawline;
    std::getline(file, rawline);
    

    std::istringstream iss(rawline, std::ios_base::in);
    std::vector<std::string> cells((std::istream_iterator<WordDelimitedByComma>(iss)),
                                    std::istream_iterator<WordDelimitedByComma>());
    
    return cells;
}

std::vector<MapCell> ProcessRow(const std::vector<std::string>& csvRow) {
        std::vector<MapCell> row;
        for (const auto& cell : csvRow) {
            row.emplace_back(cell);
        }
        
        return std::move(row);
}

std::vector<std::vector<MapCell>> ParseCSVMap(const std::string& filename){
    
    std::ifstream file(filename, std::ios_base::in);
    
    if (!file.good()) {
        std::cerr << "File opening failed: " << filename << std::endl;
    }

    std::vector<std::vector<MapCell>> grid;
    auto row = ReadLine(file);
    while (!row.empty()) {
/*
	for (const auto& col : row) {
		if (col == "w") {
			std::cout << 1;
		} else {
			std::cout << " ";
		}
	}
	std::cout << std::endl;
*/

        grid.emplace_back(ProcessRow(row));
        row = ReadLine(file);
	
    }
    

    /*
            case '1':
                if (cellEmpty){
                    row.push_back(CELL_TYPE::NUM1);
                    cellEmpty = false;
                }
                else {
                    throw std::invalid_argument("Cell occupied");
                }
                break;
            case '4':
                if (cellEmpty){
                    row.push_back(CELL_TYPE::NUM4);
                    cellEmpty = false;
                }
                else {
                    throw std::invalid_argument("Cell occupied");
                }
                break;
            case '7':
                if (cellEmpty){
                    row.push_back(CELL_TYPE::NUM7);
                    cellEmpty = false;
                }
                else {
                    throw std::invalid_argument("Cell occupied");
                }
                break;
            case '9':
                if (cellEmpty){
                    row.push_back(CELL_TYPE::NUM9);
                    cellEmpty = false;
                }
                else {
                    throw std::invalid_argument("Cell occupied");
                }
                break;
            case ',':
                if (cellEmpty){
                    row.push_back(CELL_TYPE::EMPTY);
                }
                cellEmpty = true;
                break;
            case '\n':
                if (cellEmpty){
                    row.push_back(CELL_TYPE::EMPTY);
                }
                grid.push_back(std::move(row));
                row.clear();
                cellEmpty = true;
                break;
            default:
                const std::string msg = "unexpected input: " + ch;
                throw std::invalid_argument(msg);
        }
    }
    */
    
    return grid;
}
