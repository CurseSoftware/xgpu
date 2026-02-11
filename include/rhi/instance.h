#ifndef RHI_INSTANCE_H
#define RHI_INSTANCE_H

#include "rhi/instance_context.h"
#include "rhi/expected.h"
#include "rhi/error.h"
#include "rhi/platform.h"
#include <memory>

namespace xgpu
{
    class IInstance
    {
        public:
            // Destroy the handle
            virtual auto destroy() noexcept -> void = 0;
            virtual auto backend() const noexcept -> Backend = 0;
    };
    
    class [[nodiscard]] Instance : public IInstance
    {
        public:
            [[nodiscard("This returns a status code depending on if failed")]] 
            static auto create(const InstanceContext& ctx) noexcept -> expected<Instance, Error>;

            auto destroy() noexcept -> void override { _handle->destroy(); }

            [[nodiscard]] auto backend() const noexcept -> Backend override { return _handle->backend(); }

            [[nodiscard]] auto handle() const noexcept -> IInstance* { return _handle.get(); }
        
        protected:
            explicit Instance() noexcept = default;

        private:
            std::unique_ptr<IInstance> _handle { nullptr };
    };
} // namespace xgpu

#endif // RHI_INSTANCE_H
