#include "../math/ray.hpp"
#include "../math/mat4.hpp"
#include "../hit.hpp"
#include "../color.hpp"
#include "../geometry/geometry.hpp"

enum class LightType
{
    none,
    rectangle
};

class Light : public Geometry
{
public:
    LightType lightType = LightType::none;

    virtual vec4 getPointOnLight() = 0;
    virtual float area() = 0;

    virtual json serialize() = 0;
};