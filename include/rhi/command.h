#ifndef RHI_COMMAND_H
#define RHI_COMMAND_H

#include "rhi/device.h"
#include "rhi/error.h"
#include "rhi/expected.h"
#include <memory>
namespace rhi
{
    class ICommandPool
    {
        public:
            virtual auto destroy() noexcept -> void = 0;
    };
    class CommandPool : public ICommandPool
    {
        public:
            // Create a command pool from the device
            static auto create(rhi::Device& device) noexcept -> expected<CommandPool, Error>;

        private:
            std::unique_ptr<ICommandPool> _handle { nullptr };
    };
} // namespace rhi

#endif // RHI_COMMAND_H
