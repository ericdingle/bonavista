package(default_visibility = ["//visibility:public"])

cc_library(
    name = "node",
    hdrs = ["node.h"],
    srcs = ["node.cpp"],
    deps = ["//lexer:token"],
)

cc_library(
    name = "parser",
    hdrs = ["parser.h"],
    srcs = ["parser.cpp"],
    deps = [
        ":node",
        "//lexer:token",
        "//lexer:token_stream",
        "@com_google_absl//absl/status:statusor",
    ],
)

cc_test(
    name = "parser_test",
    srcs = ["parser_test.cpp"],
    deps = [
        ":parser",
        ":parser_test_fixture",
        "//lexer:token_test_macros",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "parser_test_fixture",
    hdrs = ["parser_test_fixture.h"],
    testonly = 1,
)
