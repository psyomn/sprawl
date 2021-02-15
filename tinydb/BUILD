cc_library(
    name = "tinydb_core",
    srcs = [
        "core.cc",
        "error.cc",
        "page.cc",
        "schema.cc",
        "statement.cc",
        "table.cc",
    ],
    hdrs = [
        "core.h",
        "error.h",
        "page.h",
        "schema.h",
        "statement.h",
        "table.h",
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
