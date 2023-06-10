#include "color.hpp"
#include <vector>
#include <string>
/* White point chromaticities. */

#define IlluminantC 0.3101, 0.3162         /* For NTSC television */
#define IlluminantD65 0.3127, 0.3291       /* For EBU and SMPTE */
#define IlluminantE 0.33333333, 0.33333333 /* CIE equal-energy illuminant */

/*  Gamma of nonlinear correction.

    See Charles Poynton's ColorFAQ Item 45 and GammaFAQ Item 6 at:

       http://www.poynton.com/ColorFAQ.html
       http://www.poynton.com/GammaFAQ.html

*/

#define GAMMA_REC709 0 /* Rec. 709 */

class Image
{
public:
    int width;
    int height;
    std::vector<Color> pixels;
    std::vector<Color> sensitivityFunction;

    Image(int w, int h)
    {
        width = w;
        height = h;
        for (int i = 0; i < w * h; i++)
        {
            pixels.push_back(black);
            sensitivityFunction.push_back(white);
        }
    }

    // bottom left: (0,0). top right: (width-1, height-1)
    Color getPixel(int x, int y);
    void setPixel(int x, int y, Color c);

    std::string dump_ppm();
    struct colorSystem;
    void spectrum_to_xyz(Color c, float *x, float *y, float *z);
    void xyz_to_rgb(struct colorSystem *cs,
                    float xc, float yc, float zc,
                    float *r, float *g, float *b);
    int constrain_rgb(float *r, float *g, float *b);
    void norm_rgb(float *r, float *g, float *b);
    void gamma_correct(const struct colorSystem *cs, float *c);
    void gamma_correct_rgb(const struct colorSystem *cs, float *r, float *g, float *b);

};