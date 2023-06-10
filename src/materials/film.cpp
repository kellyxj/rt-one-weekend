#include "film.hpp"

Color Film::getColor(vec4 &pos)
{
    return this->c;
}

ray Film::scatter(ray &inRay, vec4 &pos, vec4 &normal)
{
    vec4 dir = inRay.direction.normalize();
    float dot = dir.dot(normal);
    vec4 parallel = normal * dot;
    vec4 orthog = dir - parallel;

    ray outRay(pos, parallel + orthog);
    outRay.color = inRay.color;

    // Schlick approximation https://en.wikipedia.org/wiki/Schlick%27s_approximation
    float cos_theta = -dot;
    // float r0 = (inRay.n_i - outRay.n_i) * (inRay.n_i - outRay.n_i) / ((inRay.n_i + outRay.n_i) * (inRay.n_i + outRay.n_i));
    // float reflectedPercent = r0 + (1 - r0) * pow((1 - cos_theta), 5);

    float *fresnel = this->fresnelCoating(cos_theta, inRay, outRay);
    float reflectedPercent = fresnel[0];

    float cos2 = fresnel[1];
    float sin2 = fresnel[2];
    if (sin2 > 1)
    {
        outRay.origin += normal * EPSILON;
        outRay.direction = inRay.direction - normal * 2 * inRay.direction.dot(normal);
    }
    else
    {
        double intPart;
        float randomVariable = ((float)rand() / RAND_MAX);

        if (randomVariable > reflectedPercent)
        {
            outRay.origin -= normal * EPSILON;
            outRay.direction = normal * -1 * cos2;
            outRay.direction += orthog.normalize() * sin2;
        }
        else
        {
            outRay.origin += normal * EPSILON;
            outRay.direction = inRay.direction - normal * 2 * inRay.direction.dot(normal);
        }
    }

    return outRay;
}

float Film::sampleBrdf(ray &inRay, ray &outRay, vec4 &pos)
{
    return 0;
}

// https://www.gamedev.net/articles/programming/graphics/thin-film-interference-for-computer-graphics-r2962/
float *Film::fresnelCoating(float cos0, ray &inRay, ray &outRay)
{
    // Precompute the reflection phase changes (depends on IOR)

    // if (!(inRay.n_i.spectrum == white)) // if not air
    // {
    //     float ior = this->externalIOR;
    //     this->externalIOR = this->internalIOR;
    //     this->internalIOR = ior;
    // }

    // Amplitude reflection coefficient (s-polarized)
    auto rs = [](float n1, float n2, float cosI, float cosT)
    { return (n1 * cosI - n2 * cosT) / (n1 * cosI + n2 * cosT); };
    // Amplitude reflection coefficient (p-polarized)
    auto rp = [](float n1, float n2, float cosI, float cosT)
    { return (n2 * cosI - n1 * cosT) / (n1 * cosT + n2 * cosI); };
    // Amplitude transmission coefficient (s-polarized)
    auto ts = [](float n1, float n2, float cosI, float cosT)
    { return 2 * n1 * cosI / (n1 * cosI + n2 * cosT); };
    // Amplitude transmission coefficient (p-polarized)
    auto tp = [](float n1, float n2, float cosI, float cosT)
    { return 2 * n1 * cosI / (n1 * cosT + n2 * cosI); };
    float delta10 = (thinfilmIOR < externalIOR) ? PI : 0.0f;
    float delta12 = (thinfilmIOR < internalIOR) ? PI : 0.0f;
    float delta = delta10 + delta12;
    // Calculate the thin film layer (and transmitted) angle cosines.
    float sin1 = pow(externalIOR / thinfilmIOR, 2) * (1 - pow(cos0, 2));
    float sin2 = pow(externalIOR / internalIOR, 2) * (1 - pow(cos0, 2));
    float *fresnel = new float[3];
    if ((sin1 > 1) || (sin2 > 1))
    {
        fresnel[0] = 1;
        fresnel[1] = 0;
        fresnel[2] = 1.5;
        return fresnel;
    }
    // Account for TIR.
    float cos1 = sqrt(1 - sin1);
    float cos2 = sqrt(1 - sin2);
    // Calculate the interference phase change.
    float phi = 2 * thinfilmIOR * thickness * cos1;
    int lambda = 500;
    for (int i = 0; i < inRay.color.nSpectralSamples; i++)
    {
        if (inRay.color.channels[i] == 1)
        {
            lambda = inRay.color.getLambda(i);
            break;
        }
    }
    phi *= 2 * PI / lambda;
    phi += delta;
    // Obtain the various Fresnel amplitude coefficients.
    float alpha_s = rs(thinfilmIOR, externalIOR, cos1, cos0) * rs(thinfilmIOR, internalIOR, cos1, cos2);
    float alpha_p = rp(thinfilmIOR, externalIOR, cos1, cos0) * rp(thinfilmIOR, internalIOR, cos1, cos2);
    float beta_s = ts(externalIOR, thinfilmIOR, cos0, cos1) * ts(thinfilmIOR, internalIOR, cos1, cos2);
    float beta_p = tp(externalIOR, thinfilmIOR, cos0, cos1) * tp(thinfilmIOR, internalIOR, cos1, cos2);
    // Calculate the s- and p-polarized intensity transmission coefficient.
    float ts_tot = pow(beta_s, 2) / (pow(alpha_s, 2) - 2 * alpha_s * cos(phi) + 1);
    float tp_tot = pow(beta_p, 2) / (pow(alpha_p, 2) - 2 * alpha_p * cos(phi) + 1);
    // Calculate the transmitted power ratio for medium change.
    float beamRatio = (internalIOR * cos2) / (externalIOR * cos0);
    // Calculate the average reflectance.

    fresnel[0] = 1 - beamRatio * (ts_tot + tp_tot) * 0.5f;
    fresnel[1] = cos2;
    fresnel[2] = sqrt(sin2);
    return fresnel;
}

json Film::serialize()
{
    json json_ = {
        {"type", type},
        {"n_i", n_i.spectrum.serialize()},
        {"color", c.serialize()}};
    return json_;
}