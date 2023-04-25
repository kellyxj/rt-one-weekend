#ifndef __COLOR_H__
#define __COLOR_H__

#include <iostream>
#include <vector>
#include "util/json.hpp"
using json = nlohmann::json;

class Color {
public:
    //image::dump_ppm() will use the first 3 color channels listed in entries for now
    std::vector<float> channels;
    Color() {
        channels.push_back(0);
        channels.push_back(0);
        channels.push_back(0);
    }
    Color(float red, float green, float blue) {
        channels.push_back(red);
        channels.push_back(green);
        channels.push_back(blue);
    }

    //for now, all these operators assume the spectra have the same number of channels
    Color operator + (const Color & c);
    Color & operator += (const Color & c);
    Color operator - (const Color & c);
    Color & operator -= (const Color & c);
    Color operator * (float c);
    Color & operator *= (float c);
    Color operator * (const Color & c);
    Color & operator *= (const Color & c);
    Color operator / (float c);
    Color & operator /= (float c);

    float dot(const Color & c);

    bool operator == (const Color & c);

    json serialize();
    Color deserialize(json json_);
};

inline std::ostream& operator << (std::ostream &os, const Color &c) {
    os << "(";
    auto actual_delim = ", ";
    auto delim = "";

    for (const auto& elem : c.channels) {
        std::cout << delim << elem;
        delim = actual_delim;
    }

    os << ")";

    return os;
}

const Color black(0,0,0);
const Color white(1,1,1);

const Color red(1,0,0);
const Color green(0,1,0);
const Color blue(0,0,1);

const Color grey(.4, .4, .4);
const Color sky(.85, .85, .85);

#endif