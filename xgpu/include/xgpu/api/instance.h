#pragma once
#include "instance_desc.h"
#include "xgpu/core/core.h"
#include "xgpu/traits/adapter_manager.h"
#include "xgpu/traits/physical_device_manager.h"

namespace xgpu
{
    /// @brief The entry point into an xgpu application
    template <
        GraphicsApi                   GAPI           = DefaultGraphicsApi,
        traits::IAdapterManager<GAPI> AdapterManager = traits::AdapterManager<GAPI>>
    class Instance : public AdapterManager
    {
      public:
        [[nodiscard]] explicit Instance(const InstanceDesc desc) {}
    };
} // namespace xgpu