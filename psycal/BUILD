cc_library(
    name = "psycal",
    srcs = ["dump.cc", "event.cc", "utils.cc", "message.cc", "server.cc", "session.cc"],
    hdrs = ["dump.h", "event.h", "utils.h", "message.h", "server.h", "session.h"],
    linkopts = ["-lpthread"],
    deps = ["//common:psycommon",
            "//net:psynet"],
    visibility = ["//psycal/test:__pkg__"],
)

cc_binary(
    name = "psycald",
    srcs = ["main.cc"],
    deps = ["//psycal:psycal",
            "//common:psycommon"],
)
