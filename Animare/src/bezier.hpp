#pragma once

template<typename T>
class bezier {
public:
    template<typename T>
    inline T Interpolate(const bezier<T>& curve, float t) {
        return curve.point1 * ((1 - t) * (1 - t) * (1 - t)) +
            curve.control1 * (3.0f * ((1 - t) * (1 - t)) * t) +
            curve.control2 * (3.0f * (1 - t) * (t * t)) +
            curve.point2 * (t * t * t);
    }

public:
    T point1;
    T control1;
    T point2;
    T control2;
};

template<typename T>
T hermite(float t, T& p1, T& s1, T& p2, T& s2) {
    return
        p1 * ((1.0f + 2.0f * t) * ((1.0f - t) * (1.0f - t))) +
        s1 * (t * ((1.0f - t) * (1.0f - t))) +
        p2 * ((t * t) * (3.0f - 2.0f * t)) +
        s2 * ((t * t) * (t - 1.0f));
}