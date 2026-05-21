#pragma once
#include "instance_desc.h"
#include "xgpu/components/adapter_manager.h"
#include "xgpu/components/physical_device_manager.h"
#include "xgpu/core/core.h"

namespace xgpu
{
    /// @brief The entry point into an xgpu application
    template <
        GraphicsApi                       GAPI           = DefaultGraphicsApi,
        components::IAdapterManager<GAPI> AdapterManager = components::AdapterManager<GAPI>>
    class Instance : public AdapterManager
    {
      public:
        [[nodiscard]] explicit Instance(const InstanceDesc desc) {}
    };
} // namespace xgpu