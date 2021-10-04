cc_library(
    name = "gamejam",
    srcs = [
        "level.cc",
    ],

    hdrs = [
        "level.h",
    ],
)

cc_test(
    name = "test",
    srcs = ["level_test.cc"],
    deps = [
        "@googletest//:gtest_main",
        ":gamejam",
    ],
)
