#ifndef RHI_ERROR_H
#define RHI_ERROR_H

#include <string>
namespace xgpu
{
    struct Error 
    {
        [[nodiscard]] Error(std::string_view message) noexcept : message{ message } {}
        
        std::string message;
    };
} // namespace xgpu

#endif // RHI_ERROR_H
