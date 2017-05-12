
struct State {
    long time;
    uint8_t brightness;
};

struct Config {
    const int samples = 100;
};

struct Input {
    const int capacitance;
    const long time;
};
