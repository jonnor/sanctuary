
struct Point {
    uint16_t x;
    uint16_t y;
};

#define m_constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

#ifdef HAVE_JSON11
std::vector<Point> points_from_json(const json11::Json &json) {
    //Config config;
    json11::Json::array arr = json.array_items();
    std::vector<Point> points;
    static const uint32_t POINT_MAX = 15000;
    for (const auto& value : arr) {
        const int x = m_constrain(value["x"].number_value(), 0, POINT_MAX);
        const int y = m_constrain(value["y"].number_value(), 0, POINT_MAX);
        points.push_back(Point { (uint16_t)x , (uint16_t)y });
    }
    return points;
}
#endif

static inline int interpolate_linear(int x0, int x1, int y0, int y1, int x) {
    return y0 + (y1-y0)*(x-x0)/(x1-x0);
};

int
mapping(Point *points, size_t length, int time) {
    // find the relevant point
    Point previous = points[0];
    for (unsigned int i=1; i<length; i++) {
        const Point current = points[i];
      
        if (time >= previous.x and time <= current.x) {
            if (previous.x != current.x) {
                return interpolate_linear(previous.x, current.x, previous.y, current.y, time);
            } else {
                return current.y; // doesn't matter which
            }
        }
        previous = current;
    }
    return -1;
};

int test_mapping() {

    std::vector<Point> points = {
        { 0, 0 }, { 10, 10 }, { 30, 10 }, { 35, 2 }
    };

    const int zero = mapping(&points[0], points.size(), 0);
    fprintf(stderr, "time=0 should be 0: %d\n",
          zero);
    assert(zero == 0);

    const int onpoint = mapping(&points[0], points.size(), 10);
    fprintf(stderr, "time=10 should be 10: %d\n",
          onpoint);
    assert(onpoint == 10);

    const int midflat = mapping(&points[0], points.size(), 15);
    fprintf(stderr, "time=15 should be 10: %d\n",
          midflat);
    assert(midflat == 10);

    const int midslope = mapping(&points[0], points.size(), 5);
    fprintf(stderr, "time=5 should be 5: %d\n",
          midslope);
    assert(midslope == 5);
    return 0;
}
