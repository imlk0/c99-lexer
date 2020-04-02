#include <iostream>
#include <map>
#include <lexer.h>

using namespace std;

map<token_type, char *> token_type_to_string = {
    {UNKNOWN, "UNKNOWN"},
    {KEYWORD, "KEYWORD"},
    {IDENTIFIER, "IDENTIFIER"},
    {INTEGER_CONSTANT, "INTEGER_CONSTANT"},
    {FLOATING_CONSTANT, "FLOATING_CONSTANT"},
    {CHARACTER_CONSTANTS, "CHARACTER_CONSTANTS"},
    {STRING_LITERALS, "STRING_LITERALS"},
    {PUNCTUATOR, "PUNCTUATOR"},
    // {PREPROCESSING_NUMBERS, "PREPROCESSING_NUMBERS"},
    {LINE_COMMENT, "LINE_COMMENT"},
    {BLOCK_COMMENT, "BLOCK_COMMENT"},
    {SPACES, "SPACES"},
};

map<token_type, char *> token_type_to_color = {
    {UNKNOWN, "\e[41;5;1m"},
    {KEYWORD, "\e[44m"},
    {IDENTIFIER, "\e[45m"},
    {INTEGER_CONSTANT, "\e[103m"},
    {FLOATING_CONSTANT, "\e[103;30m"},
    {CHARACTER_CONSTANTS, "\e[43m"},
    {STRING_LITERALS, "\e[43;30m"},
    {PUNCTUATOR, "\e[100m"},
    // {PREPROCESSING_NUMBERS, "\e[46m"},
    {LINE_COMMENT, "\e[42m"},
    {BLOCK_COMMENT, "\e[42m"},
    {SPACES, "\e[m"},
};

void print_match_results(match_result_type &match_result) {
    for (auto &it : match_result.offset2lexeme) {
        auto &lexeme = it.second;
        printf("[%3d : %3d]\t<%s>\t%.*s\n", lexeme.start,
               (lexeme.start + lexeme.length),
               token_type_to_string[lexeme.type], lexeme.length,
               match_result.src_str + lexeme.start);
    }
    printf("\n");
}

void print_match_results_in_color(match_result_type &match_result) {
    for (auto &it : match_result.offset2lexeme) {
        auto &lexeme = it.second;
        auto color = token_type_to_color[lexeme.type];
        auto str_len = lexeme.length;
        auto str_start = match_result.src_str + lexeme.start;
        printf("%s%.*s\e[m", color, str_len, str_start);
    }
    printf("\n");
}

void print_errors_in_match_results(match_result_type &match_result) {
    int err_count = 0;
    for (auto &it : match_result.offset2lexeme) {
        auto &lexeme = it.second;
        if (lexeme.type == UNKNOWN) {
            if (err_count) {
                printf("\n");
            }
            err_count++;
            auto line = match_result.offset2length.upper_bound(lexeme.start);
            int line_num =
                distance(match_result.offset2length.begin(), line);
            line--;
            printf("[error] #%d\tat row:%d column:%d\n", err_count, line_num,
                   lexeme.start - line->first + 1);
            printf("%.*s", line->second, match_result.src_str + line->first);
            printf("%*s^\n", lexeme.start - line->first, " ");
        }
    }
    if (err_count) {
        printf("%d errors in total.\n", err_count);
    }
}

void print_statistics_info(match_result_type &match_result) {
    // 行数
    auto line_count = match_result.offset2length.size();
    // 字符总数
    auto char_count = match_result.length;
    // 各类单词的数目
    map<token_type, int> token_count;
    for (auto &it : match_result.offset2lexeme) {
        auto &lexeme = it.second;
        token_count[lexeme.type]++;
    }

    printf("lines of code: %d\n", line_count);
    printf("characters of code: %d\n", char_count);
    printf("count of each typeof token:\n");
    printf("------------------------------\n");
    for (auto &it : token_type_to_string) {
        printf("<%s>\t | %d\n", it.second, token_count[it.first]);
    }
    printf("------------------------------\n");
}