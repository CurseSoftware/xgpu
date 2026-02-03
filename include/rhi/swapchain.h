#ifndef RHI_SWAPCHAIN_H
#define RHI_SWAPCHAIN_H

#include "rhi/error.h"
#include "rhi/expected.h"
#include "rhi/swapchain_context.h"
#include <memory>
namespace rhi
{
    class ISwapChain
    {
        public:
            virtual auto destroy() noexcept -> void = 0;
    };

    class SwapChain : public ISwapChain
    {
        public:
            static auto create(const SwapChainContext& ctx) noexcept -> expected<SwapChain, Error>;
        
        private:
            std::unique_ptr<ISwapChain> _handle { nullptr };
    };
} // namespace rhi

#endif // RHI_SWAPCHAIN_H
