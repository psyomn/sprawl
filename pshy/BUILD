cc_library(
    name = "libpshy",
    hdrs = [
        "utils.h", 
        "shell.h",
    ],
    srcs = [
        "utils.cc", 
        "shell.cc",
    ],
)

cc_binary(
    name = "pshy",
    srcs = [
        "main.cc",
    ],
    deps = [
        "//pshy:libpshy"
    ]
)
