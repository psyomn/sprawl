cc_library(
    name = "psyds",
    hdrs = ["bigint.h", "hashes.h", "md5.h"],
    srcs = ["bigint.cc", "hashes.cc", "md5.cc"],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "psyds_test",
    srcs = [
        "bigint_test.cc",
        "hashes_test.cc",
        "md5_test.cc",
    ],
    deps = ["//ds:psyds",
            "@googletest//:gtest_main"],
)
