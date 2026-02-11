#ifndef RHI_INSTANCE_H
#define RHI_INSTANCE_H

#include "types.h"
#include "xgpu/constants.h"
#include "xgpu/instance_context.h"
#include "xgpu/expected.h"
#include "xgpu/error.h"
#include "xgpu/platform.h"
#include <cstddef>
#include <cstring>
#include <functional>
#include <memory>
#include <type_traits>
#include <utility>

namespace xgpu
{
    // class Instance : public IInstance
    class Instance
    {
        // VTable info for type erasure
        private:
            struct VTable
            {
                std::function<auto(void*)->void> destroy;
                std::function<auto(const void*)->Backend> backend;
            };

            alignas(std::max_align_t) char8_t _buffer[BackendObjectBufferSize];
            const VTable* _vtable { nullptr };
            
            // Getters for underlying pointer to buffer
            auto ptr() -> void* { return _buffer; }
            auto ptr() const -> const void* { return _buffer; }

        public:
            template <typename T>
            static auto get_vtable() -> const VTable&
            {
                static const VTable& table = {
                    .destroy = [](void* obj) { static_cast<T*>(obj)->destroy(); },
                    .backend = [](const void* obj) { return static_cast<const T*>(obj)->backend(); },
                };

                return table;
            }


            template <typename T>
            auto get_as() -> T* { return reinterpret_cast<T*>(_buffer); }

        public:
            auto destroy() noexcept -> void
            {
                _vtable->destroy(ptr());
            }

            auto backend() const noexcept -> Backend 
            {
                return _vtable->backend(ptr());
            }

        // Special members
        public:
            Instance(Instance&&) = default;
            Instance(const Instance&) = default;

            auto operator=(Instance&&) -> Instance& = default;
            auto operator=(const Instance&) -> Instance& = default;

        // Private conversion constructor from backend object instance
        private:
            template <typename T, typename = std::enable_if_t<!std::is_same_v<std::decay_t<T>, xgpu::Instance>>>
            Instance(T&& backend_instance)
            {
                using DecayT = std::decay_t<T>;
                static_assert(sizeof(DecayT) <= BackendObjectBufferSize, "Backend object T is too large to store in xgpu::Instance");
                static_assert(alignof(DecayT) <= alignof(std::max_align_t), "Backend object T has incompatible alignment");

                new (_buffer) DecayT(std::forward<T>(backend_instance));
                _vtable = &get_vtable<DecayT>();
            }

        public:
            [[nodiscard("This returns a status code depending on if failed")]] 
            static auto create(const InstanceContext& ctx) noexcept -> expected<Instance, Error>;
        
        protected:
            explicit Instance() noexcept = default;
    };
} // namespace xgpu

#endif // RHI_INSTANCE_H
