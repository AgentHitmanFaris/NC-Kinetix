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
        ElasticOut,
        BackIn,
        BackOut,
        BackInOut,
        BounceIn,
        BounceOut,
        BounceInOut
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

        // Back Ease In
        static T BackIn(T t) {
            constexpr T c1 = 1.70158;
            constexpr T c3 = c1 + 1;
            return c3 * t * t * t - c1 * t * t;
        }

        // Back Ease Out
        static T BackOut(T t) {
            constexpr T c1 = 1.70158;
            constexpr T c3 = c1 + 1;
            return 1 + c3 * std::pow(t - 1, 3) + c1 * std::pow(t - 1, 2);
        }

        // Back Ease In Out
        static T BackInOut(T t) {
            constexpr T c1 = 1.70158;
            constexpr T c2 = c1 * 1.525;
            return t < 0.5
                ? (std::pow(2 * t, 2) * ((c2 + 1) * 2 * t - c2)) / 2
                : (std::pow(2 * t - 2, 2) * ((c2 + 1) * (t * 2 - 2) + c2) + 2) / 2;
        }

        // Bounce Ease Out
        static T BounceOut(T t) {
            constexpr T n1 = 7.5625;
            constexpr T d1 = 2.75;

            if (t < 1 / d1) {
                return n1 * t * t;
            } else if (t < 2 / d1) {
                return n1 * (t -= 1.5 / d1) * t + 0.75;
            } else if (t < 2.5 / d1) {
                return n1 * (t -= 2.25 / d1) * t + 0.9375;
            } else {
                return n1 * (t -= 2.625 / d1) * t + 0.984375;
            }
        }

        // Bounce Ease In
        static T BounceIn(T t) {
            return 1 - BounceOut(1 - t);
        }

        // Bounce Ease In Out
        static T BounceInOut(T t) {
            return t < 0.5
                ? (1 - BounceOut(1 - 2 * t)) / 2
                : (1 + BounceOut(2 * t - 1)) / 2;
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
                case EasingType::BackIn: return BackIn(t);
                case EasingType::BackOut: return BackOut(t);
                case EasingType::BackInOut: return BackInOut(t);
                case EasingType::BounceIn: return BounceIn(t);
                case EasingType::BounceOut: return BounceOut(t);
                case EasingType::BounceInOut: return BounceInOut(t);
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
