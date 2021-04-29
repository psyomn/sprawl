cc_library(
    name = "libpshy",
    hdrs = [
        "dynbuf.h",
        "utils.h",
        "shell.h",
        "tokenizer.h",
    ],
    srcs = [
        "dynbuf.c",
        "utils.c",
        "shell.c",
        "tokenizer.c",
    ],
    visibility = [
        "//visibility:public"
    ]
)

cc_binary(
    name = "pshy",
    srcs = [
        "main.c",
    ],
    deps = [
        "//pshy:libpshy"
    ]
)
