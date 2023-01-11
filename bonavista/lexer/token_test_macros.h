#ifndef LEXER_TOKEN_TEST_MACROS_H_
#define LEXER_TOKEN_TEST_MACROS_H_

#define EXPECT_TOKEN(expr, t, v, l, c)              \
  do {                                              \
    const Token _token = (expr);                    \
    EXPECT_EQ(t, _token.type());                    \
    EXPECT_EQ(std::string_view(v), _token.value()); \
    EXPECT_EQ(l, _token.line());                    \
    EXPECT_EQ(c, _token.column());                  \
  } while (false)

#endif  // LEXER_TOKEN_TEST_MACROS_H_
