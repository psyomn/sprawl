cc_test(
    name = "misc_test",
    srcs = ["algorithm.cc"],
    deps = [
         "@googletest//:gtest_main"
    ]
)

cc_binary(
    # dwm window status updater
    name = "dsu",
    srcs = ["dsu.cc"],
    linkopts = ["-lX11"],
    deps = ["//psycal:psycal"],
)

cc_binary(
    name = "psy-udp",
    srcs = ["udp.cc"],
    deps = [
        "//net:psynet"
    ]
)

cc_binary(
    # little book of semaphores examples
    name = "lbos",
    srcs = ["lbos.cc"],
    linkopts = ["-lpthread"],
)

cc_binary(
    name = "multi-vector",
    srcs = ["multi-vector.cc"],
    linkopts = ["-pthread"],
    deps = [
        "//common:psycommon"
    ]
)
