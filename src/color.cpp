#include "color.hpp"

Color Color::operator+(const Color &c)
{
    int size = channels.size();
    Color output;
    for (int i = 0; i < size; i++)
    {
        output.channels[i] = channels[i] + c.channels[i];
    }
    return output;
}
Color &Color::operator+=(const Color &c)
{
    int size = channels.size();
    for (int i = 0; i < size; i++)
    {
        channels[i] += c.channels[i];
    }
    return (*this);
}
Color Color::operator-(const Color &c)
{
    int size = channels.size();
    Color output;
    for (int i = 0; i < size; i++)
    {
        output.channels[i] = channels[i] - c.channels[i];
    }
    return output;
}
Color &Color::operator-=(const Color &c)
{
    int size = channels.size();
    for (int i = 0; i < size; i++)
    {
        channels[i] -= c.channels[i];
    }
    return (*this);
}
Color Color::operator*(float c)
{
    int size = channels.size();
    Color output;
    for (int i = 0; i < size; i++)
    {
        output.channels[i] = channels[i] * c;
    }
    return output;
}
Color &Color::operator*=(float c)
{
    int size = channels.size();
    for (int i = 0; i < size; i++)
    {
        channels[i] *= c;
    }
    return (*this);
}
Color Color::operator*(const Color &c)
{
    int size = channels.size();
    Color output;
    for (int i = 0; i < size; i++)
    {
        output.channels[i] = channels[i] * c.channels[i];
    }
    return output;
}
Color &Color::operator*=(const Color &c)
{
    int size = channels.size();
    for (int i = 0; i < size; i++)
    {
        channels[i] *= c.channels[i];
    }
    return (*this);
}
Color Color::operator/(float c)
{
    int size = channels.size();
    Color output;
    for (int i = 0; i < size; i++)
    {
        output.channels[i] = channels[i] / c;
    }
    return output;
}
Color Color::operator/(const Color &c)
{
    int size = channels.size();
    Color output;
    for (int i = 0; i < size; i++)
    {
        output.channels[i] = channels[i] / c.channels[i];
    }
    return output;
}

Color &Color::operator/=(float c)
{
    int size = channels.size();
    for (int i = 0; i < size; i++)
    {
        channels[i] /= c;
    }
    return (*this);
}

bool Color::operator==(const Color &c)
{
    bool isSame = true;
    int size = channels.size();
    for (int i = 0; i < size; i++)
    {
        isSame = isSame && channels[i] == c.channels[i];
    }
    return isSame;
}

float Color::dot(const Color &c)
{
    float output = 0;
    int size = channels.size();
    for (int i = 0; i < size; i++)
    {
        output += channels[i] * c.channels[i];
    }
    return output;
}

json Color::serialize()
{
    json json_{
        {"entries", channels}};
    return json_;
}
Color Color::deserialize(json json_)
{
    channels = json_["entries"].get<std::vector<float>>();
    return *this;
}

int Color::getLambda(int sample)
{
    return sampledLambdaStart + sample * (sampledLambdaEnd - sampledLambdaStart) / nSpectralSamples;
}

int Color::getIdx(int lambda)
{
    return (lambda - sampledLambdaStart) * nSpectralSamples / (sampledLambdaEnd - sampledLambdaStart);
}

Color Color::getRed()
{
    Color c = Color(0);
    const int redMinWaveLength = 630;
    const int redMaxWaveLength = 740;
    for (int i = c.getIdx(redMinWaveLength); i < c.getIdx(redMaxWaveLength); i++)
    {
        c.channels[i] = 1;
    }
    return c;
}

Color Color::getGreen()
{
    Color c = Color(0);
    const int greenMinWaveLength = 500;
    const int greenMaxWaveLength = 570;
    for (int i = c.getIdx(greenMinWaveLength); i < c.getIdx(greenMaxWaveLength); i++)
    {
        c.channels[i] = 1;
    }
    return c;
}

Color Color::getBlue()
{
    Color c = Color(0);
    const int blueMinWaveLength = 430;
    const int blueMaxWaveLength = 490;
    for (int i = c.getIdx(blueMinWaveLength); i < c.getIdx(blueMaxWaveLength); i++)
    {
        c.channels[i] = 1;
    }
    return c;
}