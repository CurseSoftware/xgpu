#ifndef RHI_COMMAND_H
#define RHI_COMMAND_H

#include "rhi/device.h"
#include "rhi/error.h"
#include "rhi/expected.h"
#include <memory>

namespace rhi
{
    class ICommandBuffer
    {
        public:
    };
    
    class CommandBuffer : public ICommandBuffer
    {
        // API
        public:

        // Private special members
        private:
            [[nodiscard]] explicit CommandBuffer() noexcept = default;

        // Private fields
        private:
            std::unique_ptr<ICommandBuffer> _handle { nullptr };
    };

    struct CommandPoolDescription
    {
        std::uint32_t family_index {};
    };
    
    class ICommandPool
    {
        public:
            virtual auto destroy() noexcept -> void = 0;

            // Allocate 
            virtual auto allocateBuffers(std::uint32_t num_buffers) noexcept -> expected<std::vector<std::unique_ptr<ICommandBuffer>>, Error> = 0;
    };
    
    class CommandPool : public ICommandPool
    {
        public:
            // Create a command pool from the device
            // NOTE: we prefer copy over reference here since Description is lightweight
            [[nodiscard]] static auto create(rhi::Device& device, const CommandPoolDescription description) noexcept -> expected<CommandPool, Error>;

            // Destroy the command pool
            auto destroy() noexcept -> void { _handle->destroy(); }

            auto allocateBuffers(std::uint32_t num_buffers) noexcept -> expected<std::vector<std::unique_ptr<ICommandBuffer>>, Error> { return std::move(_handle->allocateBuffers(num_buffers)); }

        private:
            std::unique_ptr<ICommandPool> _handle { nullptr };
    };
} // namespace rhi

#endif // RHI_COMMAND_H
