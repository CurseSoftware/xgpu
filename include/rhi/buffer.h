#ifndef RHI_BUFFER_H
#define RHI_BUFFER_H

#include "rhi/device.h"
#include "rhi/expected.h"
#include "rhi/error.h"
#include "rhi/types.h"

#include <span>
#include <memory>

namespace rhi
{
    struct BufferDescription
    {
        std::size_t size {};
        // std::span<char8_t> data {};
        BufferUsageFlags usage {};
        BufferMapping mapping {};
    };

    class IBuffer
    {
        public:
            virtual auto destroy() noexcept -> void = 0;

            virtual auto map(void** dst, std::size_t size) noexcept -> void = 0;

            virtual auto unmap() noexcept -> void = 0;
    };

    class Buffer : public IBuffer
    {
        public:
            // NOTE: we do not use `const&` here because description is cheap to copy
            static auto create(Device& device, BufferDescription description) noexcept -> expected<Buffer, Error>;

            auto map(void** dst, std::size_t size) noexcept -> void override { _handle->map(dst, size); }

            auto unmap() noexcept -> void override { _handle->unmap(); }

            auto destroy() noexcept -> void override { _handle->destroy(); }

            template <typename T>
            [[nodiscard]] auto get_as() const noexcept -> T* { return dynamic_cast<T*>(_handle.get()); }

        private:
            explicit Buffer() = default;

        private:
            std::unique_ptr<IBuffer> _handle { nullptr };
    };
} // namespace rhi

#endif // RHI_BUFFER_H
