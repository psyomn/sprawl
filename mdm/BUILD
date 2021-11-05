cc_library(
    name = "mdm",
    srcs = [
      "error.cc",
      "lexer.cc",
      "module.cc",
      "parser.cc",
      "state.cc",
      "state_machine.cc",
      "symbol_table.cc",
      "token.cc",
    ],
    hdrs = [
      "error.h",
      "lexer.h",
      "module.h",
      "parser.h",
      "state.h",
      "state_machine.h",
      "symbol_table.h",
      "token.h",
      "tree.h",
    ],
    visibility = ["//visibility:__pkg__"],
)

cc_binary(
    name = "mdmc",
    srcs = ["main.cc"],
    deps = [":mdm"],
)

filegroup(
    name = "test-data",
    srcs = [
        "data/just-words.md",
        "data/header-and-text.md",
        "data/code-blocks.md",
        "data/lists.md",
    ]
)

cc_test(
    name = "test",
    srcs = [
        "lexer_test.cc",
        "parser_test.cc",
    ],
    deps = [
      ":mdm",
      "@googletest//:gtest_main",
    ],
    data = [
      ":test-data",
    ]
)

