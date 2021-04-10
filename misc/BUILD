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

cc_binary(
    # udp client that I always need handy and never
    # have one around...
    name = "psy-udp",
    srcs = ["udp.c"],
)

cc_binary(
    # little book of semaphores examples
    name = "lbos",
    srcs = ["lbos.cc"],
    linkopts = ["-lpthread"],
)
