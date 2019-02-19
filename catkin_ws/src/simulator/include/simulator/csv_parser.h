#ifndef CSV_PARSER_HPP
#define CSV_PARSER_HPP
#include <vector>
#include <string>
#include <simulator/map_handle.h>

std::vector<std::vector<MapCell>> ParseCSVMap(const std::string& filename);


#endif // CSV_PARSER_HPP
