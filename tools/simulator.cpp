
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

    test_mapping();

    if (argc > 2) {
        std::cerr << "Usage: simulator <points.json>" << std::endl;
        exit(1);
    }

    const std::string path = argv[1];
    std::cout << "path: " << path << std::endl; 

    auto data = read_file(path);
    
    std::string parse_error;
    auto json = json11::Json::parse(data, parse_error);
    auto points = points_from_json(json);
    const int max = points[points.size()-1].x;
    //for (const auto &p : points) {
    //    std::cout << "p: " << p.x << "," << p.y << std::endl;
    //}
    std::cout << "max: " << max << std::endl;
    const int samples = 30;
    for (int i=0; i<max; i+=(max/samples)) {
        const int v = mapping(&points[0], points.size(), i);
        std::cout << v << std::endl;
    }
}
