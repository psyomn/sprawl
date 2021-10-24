cc_library(
    name = "gamejam",
    srcs = [
        "colshape.cc",
        "level.cc",
        "level_builder.cc",
        "tile.cc",
        "utils.cc",
    ],
    hdrs = [
        "colshape.h",
        "level.h",
        "level_builder.h",
        "tile.h",
        "utils.h",
    ],
)

cc_test(
    name = "test",
    srcs = [
        "colshape_test.cc",
        "level_test.cc",
        "utils_test.cc",
    ],
    deps = [
        "@googletest//:gtest_main",
        ":gamejam",
    ],
)
