load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")

new_git_repository(
  name = "googletest",
  build_file = "@//:gtest.BUILD",
  remote = "https://github.com/google/googletest",
  commit = "ec44c6c1675c25b9827aacd08c02433cccde7780",
  shallow_since = "1468516538 -0400",
  # tag = "release-1.8.0",
)

new_git_repository(
  name = "pngpp",
  # todo -- maybe rename gtest.BUILd to something else
  build_file = "@//:gtest.BUILD",
  remote = "https://github.com/sampotter/pngpp",
  commit = "37911052319bb68e5971b08c951ae5ef96ae245c",
  # shallow_since = "1468516538 -0400",
  # tag = "release-1.8.0",
)
