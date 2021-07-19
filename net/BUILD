cc_library(
    name = "psynet",
    srcs = ["udp.cc"],
    hdrs = ["udp.h"],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "psynet-example",
    srcs = ["example.cc"],
    linkopts = ["-lpthread"],
    deps = [":psynet"]
)
