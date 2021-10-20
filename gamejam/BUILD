cc_library(
    name = "gamejam",
    srcs = [
        "colshape.cc",
        "level.cc",
    ],

    hdrs = [
        "colshape.h",
        "level.h",
    ],
)

cc_test(
    name = "test",
    srcs = [
        "colshape_test.cc",
        "level_test.cc",
    ],
    deps = [
        "@googletest//:gtest_main",
        ":gamejam",
    ],
)
