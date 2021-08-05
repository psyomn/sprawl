cc_test(
    name = "tvm",
    srcs = ["tvm.cc", "tvm_data_tests.cc"],
    deps = ["//tvm:tvm",
            "@googletest//:gtest_main",
            ],
    data = [":test-data"],
)

filegroup(
    name = "test-data",
    srcs = [
        "data/arithmetic.asm",
        "data/hello-world.asm",
        "data/simple.asm",
    ]
)
