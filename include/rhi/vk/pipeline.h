#ifndef RHI_VK_PIPELINE_H
#define RHI_VK_PIPELINE_H

#include "rhi/pipeline.h"
#include "rhi/vk/core.h"

namespace rhi::vk
{
    class Pipeline : public IPipeline
    {
        public:

        private:
            VkPipeline _pipeline { VK_NULL_HANDLE };
    };
} // namespace rhi::vk

#endif // RHI_VK_PIPELINE_H
