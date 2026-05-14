#pragma once
#include "instance_desc.h"
#include "xgpu/core/core.h"
#include "xgpu/traits/adapter_manager.h"
#include "xgpu/traits/physical_device_manager.h"

namespace xgpu
{
    template <
        core::GraphicsApi             GAPI = core::DefaultGraphicsApi,
        // traits::IPhysicalDevice       PhysicalDeviceManager = traits::PhysicalDeviceManager<GAPI>,
        traits::IAdapterManager<GAPI> AdapterManager = traits::AdapterManager<GAPI>>
    class Instance : public AdapterManager
    {
      public:
        [[nodiscard]] explicit Instance(const InstanceDesc desc) {}
    };
} // namespace xgpu