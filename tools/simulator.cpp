
#include <json11.hpp>
#include <json11.cpp>


#include <cstdio>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "../mapping.hpp"


std::string read_file(std::string filename) {
    using charIterator = std::istreambuf_iterator<char>;
    std::ifstream filestream(filename);
    std::string content((charIterator(filestream)),(charIterator()));
    return content;
}

int
main(int argc, char *argv[]) {

    if (argc > 2) {
        std::cerr << "Usage: simulator <points.json>" << std::endl;
        exit(1);
    }

    const std::string path = argv[1];
    std::cout << "path: " << path << std::endl; 

    auto data = read_file(path);
    std::cout << "data: " << data << std::endl;
    
    std::string parse_error;
    auto json = json11::Json::parse(data, parse_error);
    auto points = points_from_json(json);
    const int xmax = 30;
    const int ymax = 100;
    for (int i=0; i<xmax; i++) {
        int v = mapping(&points[0], points.size(), i, xmax, ymax);
        std::cout << v << std::endl;
    }
}
