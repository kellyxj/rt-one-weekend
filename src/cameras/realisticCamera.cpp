#include "realisticCamera.hpp"
#include <iostream>

bool debugMode = false;

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
    camToWorldMatrix = camToWorldMatrix.cameraToWorld(aimPoint, eyePoint, up);
}

void RealisticCamera::computeProperties() {
    aspect = float(height) / float(width);
    float x = sqrt(diagonal * diagonal / (1 + aspect * aspect));
    float y = aspect * x;

    bounds = Bounds2f(Point2f(-x/2.0,x/2.0), Point2f(-y/2.0,y/2.0));
    pixelWidth = (bounds.x.y - bounds.x.x) / float(width);
    pixelHeight = (bounds.y.y - bounds.y.x) / float(height);
    
    if (elementInterfaces.size() > 0) {
        lensRearZ = elementInterfaces.back().thickness;
        
        lensFrontZ = 0;
        for (const LensElementInterface &element : elementInterfaces) {
            lensFrontZ += element.thickness;
        }
        
        rearElementRadius = elementInterfaces.back().apertureRadius;
    }
}

ray RealisticCamera::getEyeRay(float xPos, float yPos) {
    // generate ray origin on sensor
    // Note: xPos, yPos are already randomized within pixel
    float posU = bounds.x.x + xPos * pixelWidth;
    float posV = bounds.y.x + yPos * pixelHeight;

    // We will trace the ray in camera lensCoords and then convert
    // to world coords after. For lensCoords, the z=0 is at the sensor
    // plane and the lenses extend to the 'left' along -z
    ray rLens;
    rLens.origin = vec4(posU,posV,0,1);

    // This is where the resulting ray will be stored
    ray rOut;

    // Keep looping until you find a ray that makes it out of the lens system
    // while(true) {
    for (int i = 0; i < 10; ++i) {
        
        // We are currently assuming the camera coords where objects are out 
        // in the world along +z (which is why we use +lensRearZ, for example). This 
        // will explicitly be reversed when we traceLensesFromSensor.
        vec4 rearElementPosn;

        if (elementInterfaces.size() > 0) {
            Point2f rearElementSample = uniformDiskSample();
            rearElementPosn = vec4(rearElementSample.x,rearElementSample.y,0);
            rearElementPosn *= rearElementRadius;
            rearElementPosn += vec4(0,0,-lensRearZ, 1); // set w=1 after scaling!
        } else { // this case should be about equivalent to pinhole...
            rearElementPosn = vec4(0,0,-diagonal,1);
        }

        rLens.direction = rearElementPosn - rLens.origin;
        // if (debugMode) std::cout << "rearElementPosn: " << rearElementPosn << "\n";
        // if (debugMode) std::cout << "Pre-traced rLens.direction: " << rLens.direction << "\n";

        bool rayExitedLenses = traceLensesFromSensor(rLens, rOut);
        if (rayExitedLenses) {
            break;
        }
    }

    return rOut;
}


bool RealisticCamera::traceLensesFromSensor(ray &rLens, ray &rOut) {

    // We need to keep track of the z posn of the current element
    float elementZ = 0;

    // Now we trace that ray (backwards) through the lens elements 
    for (int i = elementInterfaces.size() - 1; i >= 0; --i) {

        // if (debugMode) std::cout << "Shouldn't be here...\n";

        const LensElementInterface &element = elementInterfaces[i];
        elementZ -= element.thickness;
        float t;
        vec4 n;

        // Aperture stops are represented with lens elements of radius 0
        bool isStop = (element.curvatureRadius == 0);

        if (isStop) {

            // ray-plane intersection with plane perp. to z axis
            t = (elementZ - rLens.origin.z) / rLens.direction.z;

        } else {

            float radius = element.curvatureRadius;

            // elementZ represents the left/rightmost part of the sphere
            // If radius is negative, we'll take the closer hit, else further
            float zCenter = elementZ + element.curvatureRadius;

            // The call to intersectSphericalElement will do the actual ray tracing
            // and update t and n for us.
            if (!intersectSphericalElement(radius, zCenter, rLens, &t, &n)) {
                if (debugMode) std::cout << "FAILED INTERSECT SPHERICAL ELEMENT \n";
                return false;
            }
        }

        // check if intersection is within bounds of element aperture
        vec4 pHit = rLens.origin + (rLens.direction * t);
        // if (debugMode) std::cout << "pHit: " << pHit << "\n";
        float r2 = pHit.x * pHit.x + pHit.y * pHit.y;
        if (r2 > element.apertureRadius * element.apertureRadius) {
            return false;
        }
        rLens.origin = pHit;

        // if not an aperture stop...
        if (!isStop) {

            // where we will store the outgoing direction
            vec4 w;

            // current element's index of refraction
            float etaI = element.eta;

            // next element's eta (else case assumes air interface)
            float etaT = (i > 0 && elementInterfaces[i-1].eta != 0) ? elementInterfaces[i-1].eta : 1;

            // compute the refracted ray direction
            if (!refract((rLens.direction * -1).normalize(), n, etaI/etaT, &w)) {
                if (debugMode) std::cout << "FAILED REFRACT! (total internal reflection)\n\n";
                return false;
            }
            rLens.direction = w;
        }
    }

    // if (debugMode) std::cout << rLens.direction << "\n";

    // transform from lens coords to camera coords
    // rLens.direction.z *= -1;

    // the origin is a point so we need to set it as such for transformations
    rLens.origin.w = 1;
    rOut = rLens;

    // convert the origin and ray direction from camera coords to world coords
    rOut.direction = camToWorldMatrix.transform(rOut.direction);
    rOut.origin = camToWorldMatrix.transform(rOut.origin);

    if (debugMode) {
        std::cout << "origin: " << rOut.origin << "\n";
        std::cout << "direct: " << rOut.direction << "\n";
        if (rOut.origin.z < -3) {
            std::getchar();
        }
    }

    return true;
}

bool RealisticCamera::intersectSphericalElement(float radius, float zCenter, ray inRay, float *t, vec4 *n) {

    // solve for intersection points t0 and t1
    // This is solving for [D^2 t^2 + 2 O D t + O^2 - R^2 = 0] so that A = D^2, B = 2 O D, C = O^2 - R^2
    // where D = direction vector of the ray, O = vector from ray origin to sphere center, R = sphere radius
    vec4 o = vec4(0,0,zCenter,1) - inRay.origin; // vector from ray origin to center of the sphere
    float A = inRay.direction.length_squared();
    float B = 2 * inRay.direction.dot(o);
    float C = o.length_squared() - radius*radius;
    float t0, t1;

    if (!quadratic(A,B,C,&t0,&t1)) {
        if (debugMode) std::cout << "FAILED QUADRATIC\n\n";
        return false;
    }

    // select the appropriate t based on ray direction and element curvature
    bool useCloserT = (inRay.direction.z > 0) ^ (radius < 0);

    *t = useCloserT ? std::min(t0,t1) : std::max(t0,t1);

    if (*t < 0) {
        if (debugMode) std::cout << "T INTERSECT IS NEGATIVE\n\n";
        return false;
    }

    // compute surface normal at intersection point
    *n = (o + inRay.direction * (*t)).normalize();

    // We want the normal to be in the same direction as the outgoing vector
    if (inRay.direction.dot(*n) < 0) {
        *n = *n * -1;
    }

    return true;
}

void RealisticCamera::computeExitPupilBounds(int nSamples) {
    exitPupilBounds.resize(nSamples);
    
    // ParallelFor([&](int i) { // TODO
    // }, nSamples);
    
    for (int i = 0; i < nSamples; ++i) {
        float r0 = (float)i / nSamples * diagonal/2.0;
        float r1 = (float)(i+1) / nSamples * diagonal/2.0;
        exitPupilBounds[i] = boundExitPupil(r0, r1);
    }
}

Bounds2f RealisticCamera::boundExitPupil(float pFilmX0, float pFilmX1) const {
    
    Bounds2f pupilBounds;

    // Sample a collection of points on the rear lens to find exit pupil
    const int nSamples = 1024*1024;
    int nExitingRays = 0;

    // Compute bounding box of projection of rear element on sampling plane
    Bounds2f projRearBounds(Point2f(-1.5f*rearElementRadius, -1.5f*rearElementRadius), 
                            Point2f( 1.5f*rearElementRadius,  1.5f*rearElementRadius));

    for (int i = 0; i < nSamples; ++i) {

        // Find loccation of sample points on x segment and rear lens element
        vec4 pFilm = vec4(lerp((i+0.5f)/nSamples, pFilmX0, pFilmX1), 0, 0);
        
        // Note: here pbrt uses a special function to calculate inverse roots
        // TODO: implement the faster inverse root method
        // float u[2] = {radicalInverse(0, i), radicalInverse(1, i)};

        float u[2] = {1/sqrt(i), 1/cbrt(i)};
        // vec4 pRear(lerp(u[0], projRearBounds.pMin.x, projRearBounds.pMax.x),
        //            lerp(u[1], projRearBounds.pMin.y, projRearBounds.pMax.y),
        //            lensRearZ);

        // // Expand pupil bounds if ray makes it rhough the lens system
        // ray rLens = ray(pFilm, pRear-pFilm);
        // ray nullOut;
        // if (inside(Point2f(pRear.x, pRear.y), pupilBounds) || traceLensesFromSensor(rLens, nullOut)) {
        //     pupilBounds = union(pupilBounds, Point2f(pRear.x, pRear.y));
        //     ++nExitingRays;
        // }
    }

    // return entire element bounds if no rays made it through the lens system
    // (i.e. it won't matter either way)
    if (nExitingRays == 0) {
        return projRearBounds;
    }

    // pupilBounds = expand(pupilBounds, 2 * projRearBounds.diagonal().length() / sqrt(nSamples));

    return pupilBounds;
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