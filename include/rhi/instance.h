#ifndef RHI_INSTANCE_H
#define RHI_INSTANCE_H

#include "instance_context.h"
#include "expected.h"
#include "error.h"
#include <memory>

namespace rhi
{
    class IInstance
    {
        public:
            // Destroy the handle
            virtual auto destroy() noexcept -> void = 0;
    };
    
    class [[nodiscard]] Instance : public IInstance
    {
        public:
            [[nodiscard("This returns a status code depending on if failed")]] 
            static auto create(const InstanceContext& ctx) noexcept -> expected<Instance, Error>;

            auto destroy() noexcept -> void override { _handle->destroy(); }
        
        protected:
            explicit Instance() noexcept = default;

        private:
            std::unique_ptr<IInstance> _handle { nullptr };
    };
} // namespace rhi

#endif // RHI_INSTANCE_H
