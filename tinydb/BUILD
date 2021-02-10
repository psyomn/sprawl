cc_library(
    name = "tinydb_core",
    srcs = [
        "core.cc",
    ],
    hdrs = [
        "core.h",
    ],
)

cc_library(
    name = "tinydb_repl",
    srcs = [
        "repl.cc",
        "statement.cc",
    ],
    hdrs = [
        "repl.h",
        "statement.h",
    ],
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
