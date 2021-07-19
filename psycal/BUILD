cc_library(
    name = "psycal",
    srcs = ["psycal.cc", "utils.cc"],
    hdrs = ["psycal.h", "utils.h"],
    deps = ["//common:psycommon"],
)

cc_binary(
    name = "psycald",
    srcs = ["main.cc"],
    deps = ["//psycal:psycal",
            "//common:psycommon",
            "//net:psynet"],
)
