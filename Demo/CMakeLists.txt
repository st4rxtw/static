add_executable(static_demo
    src/main.cpp
)

target_link_libraries(static_demo PRIVATE static::static)
