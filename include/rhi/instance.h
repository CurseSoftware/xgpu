#ifndef RHI_INSTANCE_H
#define RHI_INSTANCE_H

#include "instance_context.h"
#include "expected.h"
#include "error.h"

namespace rhi
{
    class [[nodiscard]] Instance
    {
        public:
            [[nodiscard("This returns a status code depending on if failed")]] 
            static auto create(const InstanceContext& ctx) noexcept -> expected<Instance, Error>;
        
        protected:
            explicit Instance() noexcept = default;
    };
} // namespace rhi

#endif // RHI_INSTANCE_H
