package(default_visibility = ["//visibility:public"])

cc_library(
    name = "executer",
    hdrs = ["executer.h"],
    srcs = ["executer.cpp"],
    deps = [
        "//bonavista/parser",
        "//bonavista/parser:node",
        "//bonavista/util:error",
        "//bonavista/util:status_macros",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
    ],
)

cc_test(
    name = "executer_test",
    srcs = ["executer_test.cpp"],
    deps = [
        ":executer",
        ":executer_test_fixture",
        "//bonavista/util:status_macros",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "executer_test_fixture",
    hdrs = ["executer_test_fixture.h"],
    deps = ["//bonavista/lexer:token_stream"],
)
