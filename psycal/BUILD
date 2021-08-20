cc_library(
    name = "psycal",
    srcs = ["dump.cc", "event.cc", "utils.cc", "message.cc", "server.cc", "session.cc"],
    hdrs = ["dump.h", "event.h", "utils.h", "message.h", "server.h", "session.h"],
    linkopts = ["-lpthread"],
    deps = ["//common:psycommon",
            "//net:psynet"],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "psycald",
    srcs = ["main.cc"],
    deps = ["//psycal:psycal",
            "//common:psycommon"],
)

cc_test(
    name = "test",
    srcs = [
        "message_test.cc",
        "utils_test.cc",
        "event_test.cc",
    ],
    deps = [
        "@googletest//:gtest_main",
        ":psycal",
    ]
)
