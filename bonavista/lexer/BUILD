package(default_visibility = ["//visibility:public"])

cc_library(
    name = "lexer",
    hdrs = ["lexer.h"],
    srcs = ["lexer.cpp"],
    deps = [
        ":token",
        "//bonavista/util:error",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@com_google_googletest//:gtest",
    ],
)

cc_test(
    name = "lexer_test",
    srcs = ["lexer_test.cpp"],
    deps = [
        ":lexer",
        "//bonavista/util:status_macros",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "lexer_test_fixture",
    hdrs = ["lexer_test_fixture.h"],
)

cc_library(
    name = "token",
    hdrs = ["token.h"],
    srcs = ["token.cpp"],
    deps = ["@com_google_absl//absl/strings"],
)

cc_test(
    name = "token_test",
    srcs = ["token_test.cpp"],
    deps = [
        ":token",
        ":token_test_macros",
        "@com_google_googletest//:gtest_main"
    ],
)

cc_library(
    name = "token_test_macros",
    hdrs = ["token_test_macros.h"],
    testonly = 1,
)

cc_library(
    name = "token_stream",
    hdrs = ["token_stream.h"],
    srcs = ["token_stream.cpp"],
    deps = [
        ":lexer",
        "//bonavista/util:status_macros",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
    ],
)

cc_test(
    name = "token_stream_test",
    srcs = ["token_stream_test.cpp"],
    deps = [
      ":lexer",
      ":token_stream",
      ":token_test_macros",
      "@com_google_googletest//:gtest_main"
    ],
)
