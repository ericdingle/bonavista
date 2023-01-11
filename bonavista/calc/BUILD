cc_library(
    name = "calc_executer",
    hdrs = ["calc_executer.h"],
    srcs = ["calc_executer.cpp"],
    deps = [
        ":calc_lexer",
        "//bonavista/executer",
        "//bonavista/util:error",
    ],
)

cc_test(
    name = "calc_executer_test",
    srcs = ["calc_executer_test.cpp"],
    deps = [
        ":calc_executer",
        ":calc_parser",
        "//bonavista/executer:executer_test_fixture",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "calc_lexer",
    hdrs = ["calc_lexer.h"],
    srcs = ["calc_lexer.cpp"],
    deps = [
        "//bonavista/lexer",
        "//bonavista/util:status_macros",
    ],
)

cc_test(
    name = "calc_lexer_test",
    srcs = ["calc_lexer_test.cpp"],
    deps = [
        ":calc_lexer",
        "//bonavista/lexer:lexer_test_fixture",
        "//bonavista/lexer:token_test_macros",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "calc_parser",
    hdrs = ["calc_parser.h"],
    srcs = ["calc_parser.cpp"],
    deps = [
        ":calc_lexer",
        "//bonavista/parser",
        "//bonavista/util:status_macros",
    ],
)

cc_test(
    name = "calc_parser_test",
    srcs = ["calc_parser_test.cpp"],
    deps = [
        ":calc_lexer",
        ":calc_parser",
        "//bonavista/lexer:token_test_macros",
        "//bonavista/parser:parser_test_fixture",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    deps = [
        ":calc_executer",
        ":calc_lexer",
        ":calc_parser",
    ],
)
