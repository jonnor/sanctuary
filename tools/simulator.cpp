
#include <json11.hpp>
#include <json11.cpp>

#include "../flowtrace.hpp"

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

void
write_file(std::string filename, const std::string &data) {
    std::ofstream out(filename);
    out << data;
    out.close();
}

std::string isotime(time_t basetime, long millis) {
    // XXX: HACK
    const long seconds = millis/1000;
    const long milliseconds = millis%1000;
    const time_t time = basetime + seconds;
    char buf[sizeof "2011-10-08T07:07:09.999Z"];
    const size_t len = strftime(buf, sizeof buf, "%FT%TZ", gmtime(&time));
    std::string str = std::string(buf);

    std::ostringstream ms;
    ms << "." << std::setw(3) << std::setfill('0') << milliseconds << "Z";
    str.replace(len-1, len+4, ms.str());
    return str;
}

struct State {
    long time;
    uint8_t brightness;
};

struct Config {
    const int samples = 30;
};

json11::Json
create_flowtrace(const std::vector<State> &history,
                const Config &config) {

    std::vector<flowtrace::Event> events;

    time_t basetime;
    time(&basetime);

    const std::string graphData = read_file("./graphs/main.json");
    std::string parseError;
    const json11::Json graph = json11::Json::parse(graphData, parseError);

    // TODO: IIPs

    for (auto &state : history) {
        using Ev = flowtrace::Event;
        const std::string time = isotime(basetime, state.time);

        Ev in((int)state.time);
        in.time = time;
        in.src = { "clock", "out" };
        in.tgt = { "anim", "time" };
        events.push_back(in);

        Ev out(state.brightness);
        out.time = time;
        out.src = { "anim", "out" };
        out.tgt = { "leds", "pwm" };
        events.push_back(out);
    }

    using namespace json11;
    return Json::object {
        { "header", Json::object {
            { "graphs", Json::object {
                { "default", graph },
            }},
        }},
        { "events", events },
    };
}

int
main(int argc, char *argv[]) {

    test_mapping();

    if (argc > 2) {
        std::cerr << "Usage: simulator <points.json>" << std::endl;
        exit(1);
    }

    const std::string path = argv[1];
    auto data = read_file(path);
    
    std::string parse_error;
    auto json = json11::Json::parse(data, parse_error);
    auto points = points_from_json(json);
    const int max = points[points.size()-1].x;
    
    Config config;
    std::vector<State> history;

    for (int i=0; i<max; i+=(max/config.samples)) {
        const int time = i;
        const int v = mapping(&points[0], points.size(), time);
        auto s = State { time: time, brightness: v };
        history.push_back(s);
    }

    const auto trace = create_flowtrace(history, config);
    write_file("trace.json", trace.dump());
}
