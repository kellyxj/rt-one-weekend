#include "image.hpp"

struct Image::colorSystem
{
    char *name;         /* color system name */
    float xRed, yRed,   /* Red x, y */
        xGreen, yGreen, /* Green x, y */
        xBlue, yBlue,   /* Blue x, y */
        xWhite, yWhite, /* White point x, y */
        gamma;          /* Gamma correction for system */
};

static struct Image::colorSystem
    /* Name                  xRed    yRed    xGreen  yGreen  xBlue  yBlue    White point        Gamma   */
    NTSCsystem = {"NTSC", 0.67, 0.33, 0.21, 0.71, 0.14, 0.08, IlluminantC, GAMMA_REC709},
    EBUsystem = {"EBU (PAL/SECAM)", 0.64, 0.33, 0.29, 0.60, 0.15, 0.06, IlluminantD65, GAMMA_REC709},
    SMPTEsystem = {"SMPTE", 0.630, 0.340, 0.310, 0.595, 0.155, 0.070, IlluminantD65, GAMMA_REC709},
    HDTVsystem = {"HDTV", 0.670, 0.330, 0.210, 0.710, 0.150, 0.060, IlluminantD65, GAMMA_REC709},
    CIEsystem = {"CIE", 0.7355, 0.2645, 0.2658, 0.7243, 0.1669, 0.0085, IlluminantE, GAMMA_REC709},
    Rec709system = {"CIE REC 709", 0.64, 0.33, 0.30, 0.60, 0.15, 0.06, IlluminantD65, GAMMA_REC709};

void Image::spectrum_to_xyz(Color c,
                            float *x, float *y, float *z)
{
    int i;
    float lambda, X = 0, Y = 0, Z = 0;

    /* CIE color matching functions xBar, yBar, and zBar for
       wavelengths from 380 through 780 nanometers, every 5
       nanometers.  For a wavelength lambda in this range:

            cie_color_match[(lambda - 380) / 5][0] = xBar
            cie_color_match[(lambda - 380) / 5][1] = yBar
            cie_color_match[(lambda - 380) / 5][2] = zBar

    */

    static float cie_color_match[81][3] = {
        {0.0014, 0.0000, 0.0065}, {0.0022, 0.0001, 0.0105}, {0.0042, 0.0001, 0.0201}, {0.0076, 0.0002, 0.0362}, {0.0143, 0.0004, 0.0679}, {0.0232, 0.0006, 0.1102}, {0.0435, 0.0012, 0.2074}, {0.0776, 0.0022, 0.3713}, {0.1344, 0.0040, 0.6456}, {0.2148, 0.0073, 1.0391}, {0.2839, 0.0116, 1.3856}, {0.3285, 0.0168, 1.6230}, {0.3483, 0.0230, 1.7471}, {0.3481, 0.0298, 1.7826}, {0.3362, 0.0380, 1.7721}, {0.3187, 0.0480, 1.7441}, {0.2908, 0.0600, 1.6692}, {0.2511, 0.0739, 1.5281}, {0.1954, 0.0910, 1.2876}, {0.1421, 0.1126, 1.0419}, {0.0956, 0.1390, 0.8130}, {0.0580, 0.1693, 0.6162}, {0.0320, 0.2080, 0.4652}, {0.0147, 0.2586, 0.3533}, {0.0049, 0.3230, 0.2720}, {0.0024, 0.4073, 0.2123}, {0.0093, 0.5030, 0.1582}, {0.0291, 0.6082, 0.1117}, {0.0633, 0.7100, 0.0782}, {0.1096, 0.7932, 0.0573}, {0.1655, 0.8620, 0.0422}, {0.2257, 0.9149, 0.0298}, {0.2904, 0.9540, 0.0203}, {0.3597, 0.9803, 0.0134}, {0.4334, 0.9950, 0.0087}, {0.5121, 1.0000, 0.0057}, {0.5945, 0.9950, 0.0039}, {0.6784, 0.9786, 0.0027}, {0.7621, 0.9520, 0.0021}, {0.8425, 0.9154, 0.0018}, {0.9163, 0.8700, 0.0017}, {0.9786, 0.8163, 0.0014}, {1.0263, 0.7570, 0.0011}, {1.0567, 0.6949, 0.0010}, {1.0622, 0.6310, 0.0008}, {1.0456, 0.5668, 0.0006}, {1.0026, 0.5030, 0.0003}, {0.9384, 0.4412, 0.0002}, {0.8544, 0.3810, 0.0002}, {0.7514, 0.3210, 0.0001}, {0.6424, 0.2650, 0.0000}, {0.5419, 0.2170, 0.0000}, {0.4479, 0.1750, 0.0000}, {0.3608, 0.1382, 0.0000}, {0.2835, 0.1070, 0.0000}, {0.2187, 0.0816, 0.0000}, {0.1649, 0.0610, 0.0000}, {0.1212, 0.0446, 0.0000}, {0.0874, 0.0320, 0.0000}, {0.0636, 0.0232, 0.0000}, {0.0468, 0.0170, 0.0000}, {0.0329, 0.0119, 0.0000}, {0.0227, 0.0082, 0.0000}, {0.0158, 0.0057, 0.0000}, {0.0114, 0.0041, 0.0000}, {0.0081, 0.0029, 0.0000}, {0.0058, 0.0021, 0.0000}, {0.0041, 0.0015, 0.0000}, {0.0029, 0.0010, 0.0000}, {0.0020, 0.0007, 0.0000}, {0.0014, 0.0005, 0.0000}, {0.0010, 0.0004, 0.0000}, {0.0007, 0.0002, 0.0000}, {0.0005, 0.0002, 0.0000}, {0.0003, 0.0001, 0.0000}, {0.0002, 0.0001, 0.0000}, {0.0002, 0.0001, 0.0000}, {0.0001, 0.0000, 0.0000}, {0.0001, 0.0000, 0.0000}, {0.0001, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000}};
    float total = 0;
    // for (i = 0, lambda = 380; lambda < 780.1; i++, lambda += 5)
    for (i = 0; i < c.nSpectralSamples; i++)
    {
        // int l = round((lambda - c.sampledLambdaStart) / (c.sampledLambdaEnd - c.sampledLambdaStart) * c.nSpectralSamples);
        //  if (l < 0)
        //      l = 0; 
        //  if (l >= c.nSpectralSamples)
        //      l = c.nSpectralSamples - 1;
        float Me = c.channels[i];
        total += Me;
        int l = (c.getLambda(i) - 380) / 5;
        X += Me * cie_color_match[l][0];
        Y += Me * cie_color_match[l][1];
        Z += Me * cie_color_match[l][2];
    }
    float delta = (c.sampledLambdaEnd - c.sampledLambdaStart) * c.nSpectralSamples;
    float XYZ = (X + Y + Z) / 3.0 / total;
    *x = X / XYZ; // c.nSpectralSamples * 81 / 21.3714 * 0.6;
    *y = Y / XYZ; // c.nSpectralSamples * 81 / 21.3711 * 0.6;
    *z = Z / XYZ; // c.nSpectralSamples * 81 / 21.3715 * 0.6;
}
void Image::xyz_to_rgb(struct colorSystem *cs,
                       float xc, float yc, float zc,
                       float *r, float *g, float *b)
{
    float xr, yr, zr, xg, yg, zg, xb, yb, zb;
    float xw, yw, zw;
    float rx, ry, rz, gx, gy, gz, bx, by, bz;
    float rw, gw, bw;

    xr = cs->xRed;
    yr = cs->yRed;
    zr = 1 - (xr + yr);
    xg = cs->xGreen;
    yg = cs->yGreen;
    zg = 1 - (xg + yg);
    xb = cs->xBlue;
    yb = cs->yBlue;
    zb = 1 - (xb + yb);

    xw = cs->xWhite;
    yw = cs->yWhite;
    zw = 1 - (xw + yw);

    /* xyz -> rgb matrix, before scaling to white. */

    rx = (yg * zb) - (yb * zg);
    ry = (xb * zg) - (xg * zb);
    rz = (xg * yb) - (xb * yg);
    gx = (yb * zr) - (yr * zb);
    gy = (xr * zb) - (xb * zr);
    gz = (xb * yr) - (xr * yb);
    bx = (yr * zg) - (yg * zr);
    by = (xg * zr) - (xr * zg);
    bz = (xr * yg) - (xg * yr);

    /* White scaling factors.
       Dividing by yw scales the white luminance to unity, as conventional. */

    rw = ((rx * xw) + (ry * yw) + (rz * zw)) / yw;
    gw = ((gx * xw) + (gy * yw) + (gz * zw)) / yw;
    bw = ((bx * xw) + (by * yw) + (bz * zw)) / yw;

    /* xyz -> rgb matrix, correctly scaled to white. */

    rx = rx / rw;
    ry = ry / rw;
    rz = rz / rw;
    gx = gx / gw;
    gy = gy / gw;
    gz = gz / gw;
    bx = bx / bw;
    by = by / bw;
    bz = bz / bw;

    /* rgb of the desired point */

    *r = (rx * xc) + (ry * yc) + (rz * zc);
    *g = (gx * xc) + (gy * yc) + (gz * zc);
    *b = (bx * xc) + (by * yc) + (bz * zc);
}

int Image::constrain_rgb(float *r, float *g, float *b)
{
    float w;

    /* Amount of white needed is w = - min(0, *r, *g, *b) */

    w = (0 < *r) ? 0 : *r;
    w = (w < *g) ? w : *g;
    w = (w < *b) ? w : *b;
    w = -w;

    /* Add just enough white to make r, g, b all positive. */

    if (w > 0)
    {
        *r += w;
        *g += w;
        *b += w;
        return 1; /* color modified to fit RGB gamut */
    }

    return 0; /* color within RGB gamut */
}

void Image::norm_rgb(float *r, float *g, float *b)
{
    float greatest = std::max(*r, std::max(*g, *b));

    if (greatest > 0)
    {
        *r /= greatest;
        *g /= greatest;
        *b /= greatest;
    }
}

void Image::gamma_correct(const struct colorSystem *cs, float *c)
{
    float gamma = cs->gamma;
    if (gamma == GAMMA_REC709)
    {
        /* Rec. 709 gamma correction. */
        float cc = 0.018;

        if (*c < cc)
        {
            *c *= ((1.099 * pow(cc, 0.45)) - 0.099) / cc;
        }
        else
        {
            *c = (1.099 * pow(*c, 0.45)) - 0.099;
        }
    }
    else
    {
        /* Nonlinear color = (Linear color)^(1/gamma) */
        *c = pow(*c, 1.0 / gamma);
    }
}

void Image::gamma_correct_rgb(const struct colorSystem *cs, float *r, float *g, float *b)
{
    gamma_correct(cs, r);
    gamma_correct(cs, g);
    gamma_correct(cs, b);
}

Color Image::getPixel(int x, int y)
{
    return this->pixels[y * width + x];
}
void Image::setPixel(int x, int y, Color c)
{
    this->pixels[y * width + x] = c;
}
std::string Image::dump_ppm()
{
    std::string output = "P3\n" + std::to_string(this->width) + " " + std::to_string(this->height) + "\n255\n";
    for (int j = this->height - 1; j >= 0; j--)
    {
        for (int i = 0; i < this->width; i++)
        {
            Color c = this->getPixel(i, j);
            
            float r = 0, g = 0, b = 0, x = 0, y = 0, z = 0;
            colorSystem *cs = &CIEsystem;

            spectrum_to_xyz(c, &x, &y, &z);
            xyz_to_rgb(cs, x, y, z, &r, &g, &b);
            cs->gamma = 2.0;
            // gamma_correct_rgb(cs, &r, &g, &b);

            r = 255 * r;
            g = 255 * g;
            b = 255 * b;

            r = r > 255 ? 255 : r;
            g = g > 255 ? 255 : g;
            b = b > 255 ? 255 : b;

            output += std::to_string(int(r)) + " " + std::to_string(int(g)) + " " + std::to_string(int(b)) + "\n";
        }
    }
    return output;
}