cc_library(
    name = "json_lexer",
    hdrs = ["json_lexer.h"],
    srcs = ["json_lexer.cpp"],
    deps = [
        "//lexer",
        "//util:status_macros",
    ],
)

cc_test(
    name = "json_lexer_test",
    srcs = ["json_lexer_test.cpp"],
    deps = [
        ":json_lexer",
        "//lexer:lexer_test_fixture",
        "//lexer:token_test_macros",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "json_parser",
    hdrs = ["json_parser.h"],
    srcs = ["json_parser.cpp"],
    deps = [
        ":json_lexer",
        "//parser",
    ],
)

cc_test(
    name = "json_parser_test",
    srcs = ["json_parser_test.cpp"],
    deps = [
        ":json_parser",
        "//lexer:token_test_macros",
        "//parser:parser_test_fixture",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    deps = [
        ":json_lexer",
        ":json_parser",
    ],
)
