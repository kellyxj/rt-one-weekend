#include "realisticCamera.hpp"
#include <iostream>

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
}

// Return the origin and direction of the ray that has been ray-traced through the lens system
ray RealisticCamera::getEyeRay(float xPos, float yPos) {

    // generate ray origin on sensor
    // Note: xPos, yPos are already randomized within pixel
    float posU = bounds.x.x + xPos * pixelWidth;
    float posV = bounds.y.x + yPos * pixelHeight;

    ray rLens;
    ray rOut;

    // Everything is being kept in world space coordinates!
    rLens.origin = eyePoint + uAxis*posU + vAxis*posV;

    // Keep looping until you find a ray that makes it out of the lens system
    while(true) {

        // Note: For now, our initial ray can hit any point on the first lens element
        // I.e. no exit pupil calculation (yet)
        Point2f rearElementSample = uniformDiskSample();
        vec4 rearElementPosn = eyePoint + uAxis*rearElementSample.x + vAxis*rearElementSample.y + nAxis*rearElementRadius;

        rLens.direction = rearElementPosn - rLens.origin;

        bool rayExitedLenses = traceLensesFromSensor(rLens, rOut);
        if (rayExitedLenses) {
            break;
        }
    }

    return rOut;
}


bool RealisticCamera::traceLensesFromSensor(ray &rLens, ray &rOut) {
    
    // We need to keep track of the z posn (along the n-axis) of the current element
    float elementZ = 0;

    // Now we trace that ray through the lens elements in outward order from the camera sensor
    for (int i = 0; i < elementInterfaces.size(); ++i) {

        const LensElementInterface &element = elementInterfaces[i];
        elementZ += element.thickness;

        Hit lensHit;

        // Aperture stops are represented with lens elements of radius 0
        bool isStop = (element.curvatureRadius == 0);

        vec4 lensCenter = eyePoint + nAxis * (elementZ + element.curvatureRadius);

        if (isStop) {
            // represent aperture stop plane using a "near-infinte" radius sphere
            Sphere apertureStop;
            vec4 infTranslate = eyePoint + nAxis * 1e12;
            vec4 infRadiusScale = vec4(1,1,1,0)*1e12;
            apertureStop.translate(infTranslate);
            apertureStop.scale(infRadiusScale);
            lensHit = apertureStop.trace(rLens);

            // // ... or use an actual plane?
            // Plane apertureStop;
            // apertureStop.rotate((180.0/PI) * acos(up.dot(nAxis)), uAxis); // ! Maybe wrong
            // apertureStop.translate(lensCenter);
            // lensHit = apertureStop.trace(rLens);
        } 
        else {
            float radius = element.curvatureRadius;
            float absRadius = abs(radius);

            Sphere lens;
            lens.translate(lensCenter);
            vec4 scaleAmount;
            scaleAmount = vec4(absRadius, absRadius, absRadius);
            lens.scale(scaleAmount);
            Glass glass;
            glass.n_i = element.IoR;
            lens.setMaterial(glass);
            lensHit = lens.trace(rLens);

            // ! This only returns the first hit..., we need it to return both!
            // Make a special sphere-ish class that returns both?
        }

        // check if ray hit lens
        if (lensHit.t > 1e10) return false;

        // check if intersection is within bounds of element aperture
        float r2 = lensHit.pos - lensCenter
        if (r2 > element.apertureRadius * element.apertureRadius) {
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

            if (!refract((rLens.direction * -1).normalize(), n, IoRI/IoRT, &w)) { // ! rLens.direction * -1?
                return false;
            }
            // vec4 normalizedW = w.normalize();
            // std::cout << "dotProd: " << rLens.direction.normalize().dot(normalizedW) << "\n";
            rLens.direction = w;
            // std::cout << "preOrigin: " << rLens.origin << " preDirection: " << rLens.direction << "\n";
        }
    }

    // ! This may be missing a nullptr check (look at pbr) 
    // -- though we can't directly implement that here
    rOut = rLens;

    // convert the origin and ray direction from lens coords to camera coords
    rOut.direction = lookatMatrix.transform(rOut.direction);
    rOut.origin = lookatMatrix.transform(rOut.origin);

    // std::cout << "Origin: " << rOut.origin << " Direction: " << rOut.direction << "\n";

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