#include "xgpu/api/api.h"
#include "xgpu/traits/metal/metal.h"

namespace xgpu::traits
{
    Adapter<>
    AdapterManager<core::GraphicsApi::Metal>::create_adapter() noexcept
    {
        return {};
    }
} // namespace xgpu::traits