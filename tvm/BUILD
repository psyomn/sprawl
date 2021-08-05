cc_library(
    name = "tvm",
    srcs = ["tvm.cc"],
    hdrs = ["tvm.h"],
    visibility = ["//tvm/test:__pkg__"],
)

cc_binary(
    name = "tvmc",
    srcs = ["tvmc.cc"],
    deps = [":tvm"],
)

cc_binary(
    name = "tvmi",
    srcs = ["tvmi.cc"],
    deps = [":tvm"],
)
