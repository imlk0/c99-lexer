#ifndef TOKEN_MATCHER_H
#define TOKEN_MATCHER_H

#include <map>

typedef int (*matcher_func)(char *);

enum token_type {
    UNKNOWN,
    KEYWORD, // 保留字
    IDENTIFIER,
    INTEGER_CONSTANT,
    FLOATING_CONSTANT,
    CHARACTER_CONSTANTS,
    STRING_LITERALS,       // 字符串字面量
    PUNCTUATOR,            // 符号
    // PREPROCESSING_NUMBERS, // pp-number
    LINE_COMMENT,          // 行注释
    BLOCK_COMMENT,         // 块注释
    SPACES,                // 空白
};

struct matcher_type {
    token_type type;
    matcher_func func;
};

struct lexeme_type {
    token_type type;
    int start;
    int length;
    matcher_type *matcher;
};

struct match_result_type {
    char *src_str;
    int length;
    bool error;
    std::map<int, lexeme_type> offset2lexeme;
    std::map<int, int> offset2length;
};


int match_keyword(char *);
int match_identifier(char *);
int match_integer_constant(char *);
int match_floating_constant(char *);
int match_character_constants(char *);
int match_string_literals(char *);
int match_punctuator(char *);
// int match_preprocessing_numbers(char *);
int match_line_comment(char *);
int match_block_comment(char *);
int match_spaces(char *);
void match(char *, match_result_type &);


#endif