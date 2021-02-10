cc_library(
    name = "tinydb_core",
    srcs = [
        "core.cc",
        "statement.cc",
    ],
    hdrs = [
        "core.h",
        "statement.h",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "tinydb_repl",
    srcs = [
        "repl.cc",
    ],
    hdrs = [
        "repl.h",
    ],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "tinydb",
    srcs = [
        "main.cc",
    ],
    deps = [
        "//tinydb:tinydb_core",
        "//tinydb:tinydb_repl",
    ],
    visibility = ["//visibility:public"],
)
