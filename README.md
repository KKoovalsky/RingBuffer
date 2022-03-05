# RingBuffer

Yet another RingBuffer, with features:

* C++20 (uses concepts),
* lock-free, no dependency to <mutex>, etc.,
* can be easily used on embedded devices - is leightweight and no external dependencies, 
* simple `push()`, `pop()` interface,
* no control on overflow: `push()` called on the full buffer is allowed, thus will clear the buffer,
* calling `pop()` on empty buffer will throw,
* unit-tested: one can run them by enabling CMake option: `JUNGLES_RING_BUFFER_ENABLE_TESTING` to `ON`,
* size can be only power of two,
* the type of element must be default-constructible.

## How to use it

### Option #1

Copy the header directly to your project.

### Option #2

Use CMake `FetchContent`:

```
include(FetchContent)
FetchContent_Declare(
    JunglesRingBuffer
    GIT_REPOSITORY https://github.com/jungles/RingBuffer.git
    GIT_TAG <grab-the-latest-git-tag>
)
FetchContent_MakeAvailable(JunglesRingBuffer)

# Consume jungles::ring_buffer target e.g.:
target_link_libraries(my_executable PRIVATE jungles::ring_buffer) 

```

## TODO:

* Implement policies with what to do, when overflowing. Options: throw, overwrite, no-overwrite, ...
* Implement other types of RingBuffers, which return `Maybe<T>` on `pop()` called on empty buffer.
* Make the library installable.
