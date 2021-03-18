cc_test(
    name = "misc_test",
    srcs = glob(["**/*.cc"]),
    deps = [
         "@googletest//:gtest_main"
    ]
)

cc_binary(
    # dwm window status updater
    name = "dsu",
    srcs = ["dsu.c"],
    linkopts = ["-lX11"]
)
