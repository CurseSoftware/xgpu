#ifndef RHI_DEVICE_H
#define RHI_DEVICE_H

namespace rhi
{
    struct device_context {};

    class [[nodiscard]] device
    {
        public:
            static auto create(device_context ctx) noexcept;
        
        private:
            explicit device() noexcept;
    };
} // namespace rhi

#endif // RHI_DEVICE_H
