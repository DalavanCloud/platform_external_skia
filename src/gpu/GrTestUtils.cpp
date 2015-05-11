/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GrTestUtils.h"
#include "SkMatrix.h"
#include "SkPath.h"
#include "SkRRect.h"

#ifdef GR_TEST_UTILS

static const SkMatrix& test_matrix(SkRandom* random, bool includePerspective) {
    static SkMatrix gMatrices[5];
    static const int kPerspectiveCount = 1;
    static bool gOnce;
    if (!gOnce) {
        gOnce = true;
        gMatrices[0].reset();
        gMatrices[1].setTranslate(SkIntToScalar(-100), SkIntToScalar(100));
        gMatrices[2].setRotate(SkIntToScalar(17));
        gMatrices[3].setRotate(SkIntToScalar(185));
        gMatrices[3].postTranslate(SkIntToScalar(66), SkIntToScalar(-33));
        gMatrices[3].postScale(SkIntToScalar(2), SK_ScalarHalf);

        // Perspective matrices
        gMatrices[4].setRotate(SkIntToScalar(215));
        gMatrices[4].set(SkMatrix::kMPersp0, 0.00013f);
        gMatrices[4].set(SkMatrix::kMPersp1, -0.000039f);
    }

    uint32_t count = static_cast<uint32_t>(SK_ARRAY_COUNT(gMatrices));
    if (includePerspective) {
        return gMatrices[random->nextULessThan(count)];
    } else {
        return gMatrices[random->nextULessThan(count - kPerspectiveCount)];
    }
}

namespace GrTest {
const SkMatrix& TestMatrix(SkRandom* random) { return test_matrix(random, true); }

const SkMatrix& TestMatrixPreservesRightAngles(SkRandom* random) {
    static SkMatrix gMatrices[5];
    static bool gOnce;
    if (!gOnce) {
        gOnce = true;
        // identity
        gMatrices[0].reset();
        // translation
        gMatrices[1].setTranslate(SkIntToScalar(-100), SkIntToScalar(100));
        // scale
        gMatrices[2].setScale(SkIntToScalar(17), SkIntToScalar(17));
        // scale + translation
        gMatrices[3].setScale(SkIntToScalar(-17), SkIntToScalar(-17));
        gMatrices[3].postTranslate(SkIntToScalar(66), SkIntToScalar(-33));
        // orthogonal basis vectors
        gMatrices[4].reset();
        gMatrices[4].setScale(SkIntToScalar(-1), SkIntToScalar(-1));
        gMatrices[4].setRotate(47);

        for (size_t i = 0; i < SK_ARRAY_COUNT(gMatrices); i++) {
            SkASSERT(gMatrices[i].preservesRightAngles());
        }
    }
    return gMatrices[random->nextULessThan(static_cast<uint32_t>(SK_ARRAY_COUNT(gMatrices)))];
}

const SkMatrix& TestMatrixRectStaysRect(SkRandom* random) {
    static SkMatrix gMatrices[6];
    static bool gOnce;
    if (!gOnce) {
        gOnce = true;
        // identity
        gMatrices[0].reset();
        // translation
        gMatrices[1].setTranslate(SkIntToScalar(-100), SkIntToScalar(100));
        // scale
        gMatrices[2].setScale(SkIntToScalar(17), SkIntToScalar(17));
        // scale + translation
        gMatrices[3].setScale(SkIntToScalar(-17), SkIntToScalar(-17));
        gMatrices[3].postTranslate(SkIntToScalar(66), SkIntToScalar(-33));
        // reflection
        gMatrices[4].setScale(SkIntToScalar(-1), SkIntToScalar(-1));
        // 90 degress rotation
        gMatrices[5].setRotate(90);

        for (size_t i = 0; i < SK_ARRAY_COUNT(gMatrices); i++) {
            SkASSERT(gMatrices[i].rectStaysRect());
        }
    }
    return gMatrices[random->nextULessThan(static_cast<uint32_t>(SK_ARRAY_COUNT(gMatrices)))];
}

const SkMatrix& TestMatrixInvertible(SkRandom* random) { return test_matrix(random, false); }

const SkRect& TestRect(SkRandom* random) {
    static SkRect gRects[7];
    static bool gOnce;
    if (!gOnce) {
        gOnce = true;
        gRects[0] = SkRect::MakeWH(1.f, 1.f);
        gRects[1] = SkRect::MakeWH(1.0f, 256.0f);
        gRects[2] = SkRect::MakeWH(256.0f, 1.0f);
        gRects[4] = SkRect::MakeLargest();
        gRects[5] = SkRect::MakeLTRB(-65535.0f, -65535.0f, 65535.0f, 65535.0f);
        gRects[6] = SkRect::MakeLTRB(10.0f, 10.0f, -10.0f, -10.0f);
    }
    return gRects[random->nextULessThan(static_cast<uint32_t>(SK_ARRAY_COUNT(gRects)))];
}

const SkRRect& TestRRectSimple(SkRandom* random) {
    static SkRRect gRRect[4];
    static bool gOnce;
    if (!gOnce) {
        gOnce = true;
        SkRect square = SkRect::MakeWH(10.f, 10.f);
        SkRect rectangle = SkRect::MakeWH(10.f, 20.f);
        // rect
        gRRect[0].setRect(square);
        // oval
        gRRect[1].setOval(rectangle);
        // true round rect with circular corners
        gRRect[2].setRectXY(rectangle, 1.f, 1.f);
        // true round rect with elliptical corners
        gRRect[3].setRectXY(rectangle, 2.0f, 1.0f);

        for (size_t i = 0; i < SK_ARRAY_COUNT(gRRect); i++) {
            SkASSERT(gRRect[i].isSimple());
        }
    }
    return gRRect[random->nextULessThan(static_cast<uint32_t>(SK_ARRAY_COUNT(gRRect)))];
}

const SkPath& TestPath(SkRandom* random) {
    static SkPath gPath[7];
    static bool gOnce;
    if (!gOnce) {
        gOnce = true;
        // line
        gPath[0].moveTo(0.f, 0.f);
        gPath[0].lineTo(10.f, 10.f);
        // quad
        gPath[1].moveTo(0.f, 0.f);
        gPath[1].quadTo(10.f, 10.f, 20.f, 20.f);
        // conic
        gPath[2].moveTo(0.f, 0.f);
        gPath[2].conicTo(10.f, 10.f, 20.f, 20.f, 1.f);
        // cubic
        gPath[3].moveTo(0.f, 0.f);
        gPath[3].cubicTo(10.f, 10.f, 20.f, 20.f, 30.f, 30.f);
        // all three
        gPath[4].moveTo(0.f, 0.f);
        gPath[4].lineTo(10.f, 10.f);
        gPath[4].quadTo(10.f, 10.f, 20.f, 20.f);
        gPath[4].conicTo(10.f, 10.f, 20.f, 20.f, 1.f);
        gPath[4].cubicTo(10.f, 10.f, 20.f, 20.f, 30.f, 30.f);
        // convex
        gPath[5].moveTo(0.0f, 0.0f);
        gPath[5].lineTo(10.0f, 0.0f);
        gPath[5].lineTo(10.0f, 10.0f);
        gPath[5].lineTo(0.0f, 10.0f);
        gPath[5].close();
        // concave
        gPath[6].moveTo(0.0f, 0.0f);
        gPath[6].lineTo(5.0f, 5.0f);
        gPath[6].lineTo(10.0f, 0.0f);
        gPath[6].lineTo(10.0f, 10.0f);
        gPath[6].lineTo(0.0f, 10.0f);
        gPath[6].close();
    }

    return gPath[random->nextULessThan(static_cast<uint32_t>(SK_ARRAY_COUNT(gPath)))];
}

SkStrokeRec TestStrokeRec(SkRandom* random) {
    SkStrokeRec::InitStyle style =
            SkStrokeRec::InitStyle(random->nextULessThan(SkStrokeRec::kFill_InitStyle + 1));
    SkStrokeRec rec(style);
    bool strokeAndFill = random->nextBool();
    SkScalar strokeWidth = random->nextBool() ? 0.f : 1.f;
    rec.setStrokeStyle(strokeWidth, strokeAndFill);

    SkPaint::Cap cap = SkPaint::Cap(random->nextULessThan(SkPaint::kCapCount));
    SkPaint::Join join = SkPaint::Join(random->nextULessThan(SkPaint::kJoinCount));
    SkScalar miterLimit = random->nextRangeScalar(1.f, 5.f);
    rec.setStrokeParams(cap, join, miterLimit);
    return rec;
}

};

#endif
