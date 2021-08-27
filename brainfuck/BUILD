cc_library(
    name = "brainfuck",
    srcs = ["brainfuck.cc"],
    hdrs = ["brainfuck.h"],
    visibility = ["//visibility:__pkg__"]
)

cc_binary(
    name = "bf",
    srcs = ["main.cc"],
    deps = [":brainfuck"]
)

cc_test(
    name = "test",
    srcs = [
        "bftest.cc",
    ],
    deps = [
        "@googletest//:gtest_main",
        ":brainfuck",
    ]
)
