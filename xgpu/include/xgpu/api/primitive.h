#pragma once

namespace xgpu
{
    /// @brief The baseline behavior for an API primitive
    template <typename T>
    struct PrimitiveBase
    {
        virtual ~PrimitiveBase() = default;

        /// @brief Destruction behavior for an API primitive
        void
        destroy()
        {
            static_cast<T &>(*this).destroy();
        }
    };
} // namespace xgpu