
struct State {
    long time;
    uint8_t brightness;
};

struct Config {
    const int samples = 30;
};

struct Input {
    const int capacitance;
    const long time;
};
