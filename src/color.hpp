#ifndef __COLOR_H__
#define __COLOR_H__

#include <iostream>
#include <vector>
#include "util/json.hpp"
using json = nlohmann::json;

class Color
{
public:
    // image::dump_ppm() will use the first 3 color channels listed in entries for now
    std::vector<float> channels;
    Color(float color)
    {
        channels.reserve(nSpectralSamples);
        for (int i = 0; i < nSpectralSamples; i++)
            channels.push_back(color);
    }
    Color()
    {
        channels.reserve(nSpectralSamples);
        for (int i = 0; i < nSpectralSamples; i++)
            channels.push_back(.0f);
    }
    Color(float r, float g, float b)
    {
        Color cr = getRed();
        Color cg = getGreen();
        Color cb = getBlue();
        Color c = cr * r + cg * g + cb * b;
        channels = c.channels;
    }
    // Color(float red, float green, float blue)
    // {
    //     channels.reserve(3);
    //     channels.push_back(red);
    //     channels.push_back(green);
    //     channels.push_back(blue);
    // }

    // for now, all these operators assume the spectra have the same number of channels
    Color operator+(const Color &c);
    Color &operator+=(const Color &c);
    Color operator-(const Color &c);
    Color &operator-=(const Color &c);
    Color operator*(float c);
    Color &operator*=(float c);
    Color operator*(const Color &c);
    Color &operator*=(const Color &c);
    Color operator/(float c);
    Color operator/(const Color &c);
    Color &operator/=(float c);

    float dot(const Color &c);

    bool operator==(const Color &c);

    json serialize();
    Color deserialize(json json_);

    static const int sampledLambdaStart = 380;
    static const int sampledLambdaEnd = 780;
    static const int nSpectralSamples = 10;
    int getLambda(int sample);
    int getIdx(int lambda);
    Color getRed();
    Color getGreen();
    Color getBlue();  
};

inline std::ostream &operator<<(std::ostream &os, const Color &c)
{
    os << "(";
    auto actual_delim = ", ";
    auto delim = "";

    for (const auto &elem : c.channels)
    {
        std::cout << delim << elem;
        delim = actual_delim;
    }

    os << ")";

    return os;
}

const Color black(0);
const Color white(1);

const Color red = Color().getRed();
const Color green = Color().getGreen();
const Color blue = Color().getBlue();

const Color grey(.4);
const Color sky(.85);

#endif