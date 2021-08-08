cc_test(
    name = "tvm",
    srcs = [
        "test.h",
        "tvm.cc",
        "tvm_data_tests.cc",
        "image.cc"
    ],
    deps = [
        "//tvm:tvm",
        "@googletest//:gtest_main",
    ],
    data = [":test-data"],
)

filegroup(
    name = "test-data",
    srcs = [
        "data/ands.asm",
        "data/arithmetic.asm",
        "data/hello-world.asm",
        "data/simple.asm",
        "data/many-variables.asm",
    ]
)
