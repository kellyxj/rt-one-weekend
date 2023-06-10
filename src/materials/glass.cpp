#include "glass.hpp"

Color Glass::getColor(vec4 &pos)
{
    return this->c;
}

ray Glass::scatter(ray &inRay, vec4 &pos, vec4 &normal)
{
    vec4 dir = inRay.direction.normalize();
    float dot = dir.dot(normal);
    vec4 parallel = normal * dot;
    vec4 orthog = dir - parallel;

    ray outRay(pos, parallel + orthog);
    outRay.color = inRay.color;

    // assuming only air-medium interfaces
    if (inRay.n_i.spectrum == white)
    {
        outRay.n_i = n_i;
    }
    else
    {
        outRay.n_i.spectrum = white;
    }
    float incidentIndex = inRay.n_i.spectrum.dot(inRay.color);
    float outgoingIndex = outRay.n_i.spectrum.dot(inRay.color);

    float sin_theta2_squared = incidentIndex * incidentIndex / (outgoingIndex * outgoingIndex) * (1 - (dot) * (dot));
    float cos_theta2_squared = 1 - sin_theta2_squared;
    Color r0_pdf = (inRay.n_i.spectrum - outRay.n_i.spectrum) * (inRay.n_i.spectrum - outRay.n_i.spectrum) / ((inRay.n_i.spectrum + outRay.n_i.spectrum) * (inRay.n_i.spectrum + outRay.n_i.spectrum));
    Color pdf = r0_pdf + (Color(1) - r0_pdf) * pow((1 + dot), 5);
    // total internal reflection case
    if (sin_theta2_squared > 1)
    {
        outRay.origin += normal * EPSILON;
        outRay.direction = inRay.direction - normal * 2 * inRay.direction.dot(normal);
        outRay.pdf = Color(1) - pdf;
    }
    else
    {
        // Schlick approximation https://en.wikipedia.org/wiki/Schlick%27s_approximation
        float cos_theta = -dot;
        float r0 = (incidentIndex - outgoingIndex) * (incidentIndex - outgoingIndex) / ((incidentIndex + outgoingIndex) * (incidentIndex + outgoingIndex));
        float reflectedPercent = r0 + (1 - r0) * pow((1 - cos_theta), 5);

        float randomVariable = ((float)rand() / RAND_MAX);

        if (randomVariable > reflectedPercent)
        {
            outRay.origin -= normal * EPSILON;
            outRay.direction = normal * -1 * sqrt(cos_theta2_squared);
            outRay.direction += orthog.normalize() * sqrt(sin_theta2_squared);
            outRay.pdf = pdf;
        }
        else
        {
            outRay.origin += normal * EPSILON;
            outRay.direction = inRay.direction - normal * 2 * inRay.direction.dot(normal);
            outRay.pdf = Color(1) - pdf;
        }
    }

    return outRay;
}

float Glass::sampleBrdf(ray &inRay, ray &outRay, vec4 &pos)
{
    return 0;
}

json Glass::serialize()
{
    json json_ = {
        {"type", type},
        {"n_i", n_i.spectrum.serialize()},
        {"color", c.serialize()}};
    return json_;
}