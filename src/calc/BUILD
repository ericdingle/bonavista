cc_library(
    name = "calc_executer",
    hdrs = ["calc_executer.h"],
    srcs = ["calc_executer.cpp"],
    deps = [
        ":calc_lexer",
        "//executer",
        "//util:error",
    ],
)

cc_test(
    name = "calc_executer_test",
    srcs = ["calc_executer_test.cpp"],
    deps = [
        ":calc_executer",
        ":calc_parser",
        "//executer:executer_test_fixture",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "calc_lexer",
    hdrs = ["calc_lexer.h"],
    srcs = ["calc_lexer.cpp"],
    deps = [
        "//lexer",
        "//util:status_macros",
    ],
)

cc_test(
    name = "calc_lexer_test",
    srcs = ["calc_lexer_test.cpp"],
    deps = [
        ":calc_lexer",
        "//lexer:lexer_test_fixture",
        "//lexer:token_test_macros",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "calc_parser",
    hdrs = ["calc_parser.h"],
    srcs = ["calc_parser.cpp"],
    deps = [
        ":calc_lexer",
        "//parser",
        "//util:status_macros",
    ],
)

cc_test(
    name = "calc_parser_test",
    srcs = ["calc_parser_test.cpp"],
    deps = [
        ":calc_lexer",
        ":calc_parser",
        "//lexer:token_test_macros",
        "//parser:parser_test_fixture",
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
