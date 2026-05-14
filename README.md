# X-GPU: A Rendering Hardware Interface
> [!NOTE]
> We are currently refactoring the architecture to more easliy support multiple backend APIs. See [refactor](https://github.com/CurseSoftware/xgpu/tree/refactor) branch for updates

## Purpose
This is an experimental, cross-gpu API layer meant for GPU-agnostic rendering code supporting C++ 20.
The main goal of this is to get enough support for use in the [Blade Game Engine](https://github.com/CurseSoftware/blade).

## Building
X-GPU is built using CMake.

Run this from the project root:
```bash
cmake -B build .
```
Followed by this
```bash
cmake --build build
```
This will build the library and examples from source.

## Basic examples & concepts
### Instance
Like many modern graphics applications, the first step is to create an instance:
Note that we opt to use explicit creation through `T::create` static methods and each backend object is **explicitly** destroyed with a `T.destroy()` method.
```cpp
auto instance = xgpu::Instance::create(xgpu::InstanceDescription {
    .enable_debug = true
}).unwrap();

// other code

instance.destroy();
```

#### Note:
As many operations can fail, many calls for creating objects return a `xgpu::expected<T, E>` object where `T` is the expected type and `E` is an error value returned on failure.
We use a custom `expected` type for now as this is meant to support C++ 20.

Similar to Rust's `Result<T, E>` the method to get the value from the object is `unwrap()`. This will throw a `wgpu::bad_expected_access` exception when called on error-containing objects.

### Device
Once an instance is created it can be used to create a `xgpu::Device`
```cpp
auto device = xgpu::Device::create(xgpu::DeviceDescription {
    .instance = instance,
    .graphics_preference = xgpu::Preference::Required,
    .transfer_preference = xgpu::Preference::Required,
    .compute_preference = xgpu::Preference::NoPreference,
}).unwrap();

// ...
device.destroy();
instance.destroy();
```
The preferences are whether certain functionalities should be required, preferred, or does not matter.
If a required preference is not found, `Device::create` will return a `xgpu::Error` containing a messge letting you know.

### Shaders
Unlike other libraries like WebGPU, we do not have a DSL shading languge, and we do not enforce any standard on it.


