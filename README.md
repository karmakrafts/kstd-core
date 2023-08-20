# kstd-core
A collection of common types to plug holes in pre-C++20 versions.  
This library provides the following things:
* Rust-like type definitions for `cstdint` types and atomics (`i8`, `f32` etc.)
* Assertions with source locations (source traces)
* Bitflag types (enum classes with bitwise operators using macros)
* `kstd::Box` for encapsulating references as pointers
* Copy/move implementation macros for default/delete implementation
* Hash API for hashing more than one value and for combining hashes
* Readable macro definitions for detecting the C++ version (C++11/14/17/20/23)
* Type-safe cross-platform wrapper for the C standard library
* `kstd::NonZero` for integers and pointers
* `kstd::Option` as a `std::optional` replacement with reference support and NonZero optimizations
* `kstd::OutPtr` [as suggested by C++26](https://en.cppreference.com/w/cpp/memory/out_ptr_t/out_ptr)
* `kstd::Pack` for emulating first-class type support of parameter packs
* Non-IO-stream print API based on {fmt}
* `kstd::RelativePtr` for defining relative pointers
* `kstd::Result` result type with support for `void` and references
* Safe allocation API which wraps around `new`/`make_unique`/`make_shared` and provides results
* `kstd::Slice` as a pre-C++20 replacement for `std::span`
* `kstd::SourceLocation` for easily accessing source information like file name, line number and function name
* `kstd::Tuple` as a replacement for `std::tuple` with support for references and easy concatination
* Cross-platform unicode conversion API with support for UTF-8/16/32
* `kstd::transmute` function as a replacement for `std::bitcast`
* `kstd::Void` type for denoting empty states in `std::variant` and as a dummy type

### Using


### Testing
In order to build kstd-core with unit tests, you can simply use CMake as follows after cloning the repository:
```shell
cmake -S . -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug -DKSTD_CORE_BUILD_TESTS=ON
cmake --build cmake-build-debug
```
This will produce an executable `kstd-core-tests(.exe)` inside of the `cmake-build-debug` directory.  
You can directly run this to invoke the Google Test suite.
