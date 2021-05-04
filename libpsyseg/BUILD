cc_library(
    name = "libpsyseg",
    srcs = ["psyseg.c"],
    hdrs = ["common.h", "psyseg.h"],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "psyseg",
    srcs = ["main.c"],
    deps = [
        "//libpsyseg:libpsyseg",
    ],
)
