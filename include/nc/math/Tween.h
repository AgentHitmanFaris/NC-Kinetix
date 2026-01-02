#pragma once

#include <cmath>
#include <concepts>
#include <numbers>

namespace nc::math {

    // Concept to ensure we are tweening floating point types
    template <typename T>
    concept Float = std::floating_point<T>;

    enum class EasingType {
        Linear,
        QuadIn,
        QuadOut,
        QuadInOut,
        ElasticOut
    };

    template <Float T>
    class Tween {
    public:
        // Linear Interpolation
        static T Linear(T t) {
            return t;
        }

        // Quadratic Ease In
        static T QuadIn(T t) {
            return t * t;
        }

        // Quadratic Ease Out
        static T QuadOut(T t) {
            return t * (2 - t);
        }

        // Quadratic Ease In-Out
        static T QuadInOut(T t) {
            if (t < 0.5) return 2 * t * t;
            return -1 + (4 - 2 * t) * t;
        }

        // Elastic Ease Out
        static T ElasticOut(T t) {
            constexpr T c4 = (2 * std::numbers::pi_v<T>) / 3;
            return t == 0 ? 0 : t == 1 ? 1 : std::pow(2, -10 * t) * std::sin((t * 10 - 0.75) * c4) + 1;
        }

        // Generic function to apply easing
        static T Apply(EasingType type, T t) {
            // Clamp t between 0 and 1
            if (t < 0) t = 0;
            if (t > 1) t = 1;

            switch (type) {
                case EasingType::Linear: return Linear(t);
                case EasingType::QuadIn: return QuadIn(t);
                case EasingType::QuadOut: return QuadOut(t);
                case EasingType::QuadInOut: return QuadInOut(t);
                case EasingType::ElasticOut: return ElasticOut(t);
                default: return t;
            }
        }
        
        // Interpolate between start and end using the easing function
        static T Interpolate(T start, T end, T t, EasingType type) {
            T alpha = Apply(type, t);
            return start + (end - start) * alpha;
        }
    };

}
