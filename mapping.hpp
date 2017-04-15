
struct Point {
    uint8_t x;
    uint8_t y;
};

#ifdef HAVE_JSON11
std::vector<Point> points_from_json(const json11::Json &json) {
    //Config config;
    json11::Json::array arr = json.array_items();
    std::cout << "items: " << arr.size() << std::endl;
    std::vector<Point> points(arr.size());
    for (const auto& value : arr) {
        const Point p = Point { value["x"].number_value(), value["y"].number_value() };
        std::cout << std::to_string(p.x) << "," << std::to_string(p.y) << std::endl;
        points.push_back(p);
    }
    return points;
}
#endif

static inline int interpolate_linear(int x0, int x1, int y0, int y1, int x) {
    return y0 + (y1-y0)*(x-x0)/(x1-x0);
};

int
mapping(Point *points, size_t length, int time, int timebase, int valuebase) {
    static const uint8_t POINT_MAX = 255; 
    const uint8_t pointtime = time/POINT_MAX; 
    // FIXME: error if time > timebase
    // FIXME: error if using higher than 16bit timebase

    // find the relevant point
    Point previous = points[0];
    for (int i=1; i<length; i++) {
        const Point current = points[i];
        if (pointtime > previous.x and pointtime < current.x) {
            return interpolate_linear(previous.x, previous.y, valuebase*previous.x, valuebase*previous.x, time);
        }
        previous = current;
    }
    return -1;
};
