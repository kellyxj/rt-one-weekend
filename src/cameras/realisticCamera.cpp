#include "realisticCamera.hpp"
#include <iostream>

bool debugMode = true;

void RealisticCamera::setEyePosition(vec4 pos) {
    eyePoint = pos;
}

void RealisticCamera::setLookDirection(float pan, float tilt) {
    tiltAngle = tilt;
    panAngle = pan;
    aimPoint = vec4(eyePoint.x+cos(PI*panAngle/180)*cos(PI*tiltAngle/180),
                    eyePoint.y+sin(-PI*panAngle/180)*cos(PI*tiltAngle/180),
                    eyePoint.z+sin(PI*tiltAngle/180),
                    1);
    up = vec4(cos(PI*panAngle/180)*cos(PI*(tiltAngle+90)/180),
                    sin(-PI*panAngle/180)*cos(PI*(tiltAngle+90)/180),
                    sin(PI*(tiltAngle+90)/180),
                    0);

    setUVN();
}

// Setup the coordinate axes of the camera
void RealisticCamera::setUVN() {
    nAxis = (eyePoint - aimPoint).normalize();
    uAxis = (up.cross(nAxis)).normalize();
    vAxis = nAxis.cross(uAxis);
}

void RealisticCamera::computeProperties() {
    aspect = float(height) / float(width);
    float x = sqrt(diagonal * diagonal / (1 + aspect * aspect));
    float y = aspect * x;
    bounds = Bounds2f(Point2f(-x/2,x/2), Point2f(-y/2,y/2));
    pixelWidth = (bounds.x.y - bounds.x.x) / float(width);
    pixelHeight = (bounds.y.y - bounds.y.x) / float(height);
    lensRearZ = elementInterfaces.back().thickness;
    lensFrontZ = 0;
    for (const LensElementInterface &element : elementInterfaces)
        lensFrontZ += element.thickness;
    rearElementRadius = elementInterfaces.back().apertureRadius;

    if (debugMode) std::cout 
    << "\nCamera Info:"
    << "\nlensRearZ:   " << lensRearZ 
    << "\nlensFrontZ:  " << lensFrontZ 
    << "\nAspect:      " << aspect 
    << "\nwidth:       " << width 
    << "\nheight:      " << height 
    << "\npixelWidth:  " << pixelWidth 
    << "\npixelHeight: " << pixelHeight 
    << "\nboundsX0:    " << bounds.x.x 
    << "\nboundsX1:    " << bounds.x.y 
    << "\nboundsY0:    " << bounds.y.x 
    << "\nboundsY1:    " << bounds.y.y 
    << "\ndiagonal:    " << diagonal 
    << "\n\n" << std::endl; 
}

// Return the origin and direction of the ray that has been ray-traced through the lens system
ray RealisticCamera::getEyeRay(float xPos, float yPos) {

    // generate ray origin on sensor
    // Note: xPos, yPos are already randomized within pixel
    float posU = bounds.x.x + xPos * pixelWidth;
    float posV = bounds.y.x + yPos * pixelHeight;

    // We will trace the ray in camera lensCoords and then convert
    // to world coords after. For lensCoords, the z=0 is at the sensor
    // plane and the lenses extend to the left along -z
    ray rLens;
    rLens.origin = vec4(posU,posV,0);

    // This is where the resulting ray will be stored
    ray rOut;

    // Keep looping until you find a ray that makes it out of the lens system
    while(true) {

        // Note: For now, our initial ray can hit any point on the first lens element
        // I.e. no exit pupil calculation (yet)
        Point2f rearElementSample = uniformDiskSample();
        vec4 rearElementPosn = vec4(rearElementSample.x,rearElementSample.y,0);

        rearElementPosn *= rearElementRadius;
        rearElementPosn += vec4(0,0,lensRearZ);

        rLens.direction = rearElementPosn - rLens.origin;

        if (debugMode) std::cout 
        << "RayOrigin: " << rLens.origin 
        << "\nRearElementPosn: " << rearElementPosn 
        << "\nrLensDirection: " << rLens.direction << "\n";

        bool rayExitedLenses = traceLensesFromSensor(rLens, rOut);
        if (rayExitedLenses) {
            if (debugMode) std::cout << "Made it out\n";
            break;
        }
        if (debugMode) std::cout << "looping...\n";
    }

    return rOut;
}


bool RealisticCamera::traceLensesFromSensor(ray &rLens, ray &rOut) {
    
    // We need to keep track of the z posn of the current element
    float elementZ = 0;

    // Now we trace that ray through the lens elements 
    // (this is closely following pbr's implementation)
    for (int i = elementInterfaces.size() - 1; i >= 0; --i) {

        if (debugMode) std::cout << "Now tracing element " << i << "\n";

        const LensElementInterface &element = elementInterfaces[i];
        elementZ -= element.thickness;
        float t; // parameterized intersection value
        vec4 n; // normal

        // Aperture stops are represented with lens elements of radius 0
        bool isStop = (element.curvatureRadius == 0);

        if (isStop) {

            // ray-plane intersection with plane perp. to z axis
            t = (elementZ - rLens.origin.z) / rLens.direction.z;

        } else {

            // generate the sphere that we will intersect the ray with
            float radius = element.curvatureRadius;

            // elementZ represents the left/rightmost part of the sphere
            // We need to add the radius to get to the center. Note that a 
            // negative radius will determine if elementZ is the left/rightmost part
            float zCenter = elementZ + element.curvatureRadius;

            // The call to intersectSphericalElement will do the actual ray tracing
            // and update t and n for us.
            if (!intersectSphericalElement(radius, zCenter, rLens, &t, &n)) {
                if (debugMode) std::cout << "\nFAILED INTERSECT!\n";
                return false;
            }
        }

        // check if intersection is within bounds of element aperture
        vec4 pHit = rLens.origin + (rLens.direction * t);
        float r2 = pHit.x * pHit.x + pHit.y * pHit.y;
        if (r2 > element.apertureRadius * element.apertureRadius) {
            if (debugMode) std::cout << "\nFAILED APERTURE BOUNDS!\n";
            return false;
        }
        rLens.origin = pHit;

        // update ray direction using snell's law
        if (!isStop) {

            // where we will store the outgoing direction
            vec4 w;

            // current element's IoR
            float IoRI = element.IoR;

            // next element's IoR (else case assumes air interface)
            float IoRT = (i > 0 && elementInterfaces[i-1].IoR != 0) ? elementInterfaces[i-1].IoR : 1;

            // ! May need to multiply by rLens direction by -1
            if (!refract((rLens.direction).normalize(), n, IoRI/IoRT, &w)) {
                if (debugMode) std::cout << "\nFAILED INTERNAL REFLECT!\n";
                return false;
            }
            rLens.direction = w;
        }
    }

    // ! This may be missing a check (look at pbr)
    rOut = rLens;

    // convert the origin and ray direction from lens coords to camera coords
    // rOut = //TODO

    return true;
}

bool RealisticCamera::intersectSphericalElement(float radius, float zCenter, ray inRay, float *t, vec4 *n) {

    // solve for intersection points t0 and t1
    // This is solving for [D^2 t^2 + 2 O D t + O^2 - R^2 = 0] so that A = D^2, B = 2 O D, C = O^2 - R^2
    // where D = direction vector of the ray, O = vector from ray origin to sphere center, R = sphere radius
    vec4 o = vec4(0,0,zCenter) - inRay.origin; // vector from ray origin to center of the sphere
    float A = inRay.direction.length_squared();
    float B = 2 * inRay.direction.dot(o);
    float C = o.length_squared() - radius*radius;
    float t0, t1;

    if (debugMode) std::cout 
    << "InRayDirection: " << inRay.direction 
    << "\nO: " << o
    << "\nA: " << A 
    << " B: " << B 
    << " C: " << C 
    << "\n";

    if (!quadratic(A,B,C,&t0,&t1)) return false;

    if (debugMode) std::cout << "passed quadratic!\n";

    // select the appropriate t based on ray direction and element curvature
    bool useCloserT = (inRay.direction.z > 0) ^ (radius < 0);
    *t = useCloserT? std::min(t0,t1) : std::max(t0,t1);
    if (*t < 0) return false;

    // compute surface normal at intersection point
    *n = (o + inRay.direction * (*t)).normalize();

    // We want the dot prod to be negative such that inRay.direction is opposite the normal
    if (inRay.direction.dot(*n) > 0) {
        *n = *n * -1;
    }

    return true;
}

inline bool RealisticCamera::quadratic(float a, float b, float c, float *t0, float *t1) {
    
    // use doubles to minimize floating point error, especially for sqrt
    double discriminant = (double)b*(double)b - 4*(double)a*(double)c;
    if (debugMode) std::cout << "Discriminant: " << discriminant << "\n";
    if (discriminant < 0) return false;
    double rootDiscriminant = sqrt(discriminant);

    double q;
    if (b < 0) q = -0.5 * (b - rootDiscriminant);
    else q = -0.5 * (b + rootDiscriminant);

    // "more stable form"... (?)
    *t0 = q/a;
    *t1 = c/q;

    // make sure t0 < t1
    if (*t0 > *t1) std::swap(*t0, *t1);
    return true;
}

bool RealisticCamera::refract(const vec4 &wi, const vec4 &n, float IoR, vec4 *wt) {
    
    vec4 n_ = n;
    vec4 wi_ = wi;
    float cosThetaI = n_.dot(wi_);

    float sin2ThetaI = std::max(0.f, 1.f - cosThetaI * cosThetaI);
    float sin2ThetaT = IoR*IoR*sin2ThetaI;

    // ignore total internal reflection case
    if (sin2ThetaT >= 1) return false;

    float cosThetaT = sqrt(1 - sin2ThetaT);

    *wt = (wi_*-1)*IoR + n_*(IoR*cosThetaI - cosThetaT);

    return true; 
}

json RealisticCamera::serialize() {
    json json_ = {
        {"width", width},
        {"height", height},
        {"eyePoint", eyePoint.serialize()},
        {"aimPoint", aimPoint.serialize()},
        {"up", up.serialize()},
        //TODO...
        {"exposure", exposure},
        {"gamma", gamma}
    };

    return json_;
}