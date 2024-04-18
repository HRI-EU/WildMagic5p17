// Geometric Tools, LLC
// Copyright (c) 1998-2014
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
//
// File Version: 5.0.1 (2010/10/01)

#ifndef WM5DISTSEGMENT3RECTANGLE3_H
#define WM5DISTSEGMENT3RECTANGLE3_H

#include "Wm5MathematicsLIB.h"
#include "Wm5Distance.h"
#include "Wm5Segment3.h"
#include "Wm5Rectangle3.h"

namespace Wm5
{

template <typename Real>
class WM5_MATHEMATICS_ITEM DistSegment3Rectangle3
    : public Distance<Real,Vector3<Real> >
{
public:
    DistSegment3Rectangle3 (const Segment3<Real>& segment,
        const Rectangle3<Real>& rectangle);

    // Object access.
    const Segment3<Real>& GetSegment () const;
    const Rectangle3<Real>& GetRectangle () const;

    // Static distance queries.
    virtual Real Get ();
    virtual Real GetSquared ();

    // Function calculations for dynamic distance queries.
    virtual Real Get (Real t, const Vector3<Real>& velocity0,
        const Vector3<Real>& velocity1);
    virtual Real GetSquared (Real t, const Vector3<Real>& velocity0,
        const Vector3<Real>& velocity1);

    // Information about the closest points.
    Real GetSegmentParameter () const;
    Real GetRectangleCoordinate (int i) const;

private:
    using Distance<Real,Vector3<Real> >::mClosestPoint0;
    using Distance<Real,Vector3<Real> >::mClosestPoint1;

    const Segment3<Real>* mSegment;
    const Rectangle3<Real>* mRectangle;

    // Information about the closest points.

    // closest0 = seg.origin + param*seg.direction
    Real mSegmentParameter;

    // closest1 = rect.center + param0*rect.dir0 + param1*rect.dir1
    Real mRectCoord[2];
};

typedef DistSegment3Rectangle3<float> DistSegment3Rectangle3f;
typedef DistSegment3Rectangle3<double> DistSegment3Rectangle3d;

}

#endif
