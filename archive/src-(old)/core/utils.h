#ifndef RHI_CORE_UTILS_H
#define RHI_CORE_UTILS_H

namespace xgpu::utils
{
    // Overload struct for handling std::variant
    template <typename ...Ts>
    struct Overload : Ts...
    {
        using Ts::operator()...;
    };
    template<class ...Ts> Overload(Ts...) -> Overload<Ts...>;
} // namesapce rhi::utils

#endif // RHI_CORE_UTILS_H
