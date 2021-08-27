#!/usr/bin/env bash
bazel coverage -s \
  --instrument_test_targets \
  --experimental_cc_coverage \
  --combined_report=lcov \
  --coverage_report_generator=@bazel_tools//tools/test/CoverageOutputGenerator/java/com/google/devtools/coverageoutputgenerator:Main \
  //...

find ./bazel-out/ -iname "coverage.dat" -print0 | xargs -0 genhtml -o coverage/