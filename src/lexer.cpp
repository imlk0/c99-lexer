#include <algorithm>
#include <cstring>
#include <iostream>
#include <lexer.h>

using namespace std;

/**
 * basic helper functions
 */
bool str_start_with(const char *str, const char *target) {
    return !strncmp(str, target, strlen(target));
}

bool str_start_with_ignore_case(const char *str, const char *target) {
    return !strncasecmp(str, target, strlen(target));
}

int match_string_in_list(char *str, const char *strlist[],
                         bool case_sensitive = true) {
    const char *matched_str = NULL;
    size_t matched_str_len = 0;
    for (auto t = strlist; *t != NULL; t++) {
        if (strlen(*t) > matched_str_len &&
            ((case_sensitive && str_start_with(str, *t)) ||
             (!case_sensitive && str_start_with_ignore_case(str, *t)))) {
            matched_str = *t;
            matched_str_len = strlen(*t);
        }
    }
    return matched_str_len;
}

int match_digit(char *str) { return ('0' <= str[0] && str[0] <= '9'); }

int match_octal_digit(char *str) {
    char ch = str[0];
    return '0' <= ch && ch <= '7';
}

int match_hexadecimal_digit(char *str) {
    char ch = str[0];
    return ('0' <= ch && ch <= '9') || ('a' <= ch && ch <= 'z') ||
           ('A' <= ch && ch <= 'Z');
}

int match_nondigit(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_';
}

int match_universal_character_name(char *str) {
    // example: \u0040;
    if (str_start_with_ignore_case(str, "\\u")) {
        if (match_hexadecimal_digit(str + 2) &&
            match_hexadecimal_digit(str + 3) &&
            match_hexadecimal_digit(str + 4) &&
            match_hexadecimal_digit(str + 5)) {

            if (match_hexadecimal_digit(str + 6) &&
                match_hexadecimal_digit(str + 7) &&
                match_hexadecimal_digit(str + 8) &&
                match_hexadecimal_digit(str + 9)) {
                return 10;
            }
            return 6;
        }
    }
    return 0;
}

int match_identifier_nondigit(char *str) {
    int m = match_nondigit(str[0]);
    m = max(m, match_universal_character_name(str));
    return m;
}

int match_digit_sequence(char *str) {
    int cur_pos = 0;
    while (str[cur_pos] && match_digit(str + cur_pos)) {
        cur_pos++;
    }
    return cur_pos;
}

int match_hexadecimal_digit_sequence(char *str) {
    int cur_pos = 0;
    while (str[cur_pos] && match_hexadecimal_digit(str + cur_pos)) {
        cur_pos++;
    }
    return cur_pos;
}

int match_exponent_part(char *str) {
    int cur_pos = 0;
    if (str[cur_pos] == 'e' || str[cur_pos] == 'E') {
        cur_pos++;
        if (str[cur_pos] == '-' || str[cur_pos] == '+') {
            cur_pos++;
        }
        int t = match_digit_sequence(str + cur_pos);
        if (t) {
            cur_pos += t;
            return cur_pos;
        }
    }
    return 0;
}

int match_binary_exponent_part(char *str) {
    int cur_pos = 0;
    if (str[cur_pos] == 'p' || str[cur_pos] == 'P') {
        cur_pos++;
        if (str[cur_pos] == '-' || str[cur_pos] == '+') {
            cur_pos++;
        }
        int t = match_digit_sequence(str + cur_pos);
        if (t) {
            cur_pos += t;
            return cur_pos;
        }
    }
    return 0;
}

int match_floating_suffix(char *str) {
    if (str[0] == 'f' || str[0] == 'l' || str[0] == 'F' || str[0] == 'L') {
        return 1;
    } else {
        return 0;
    }
}

int match_line_end(char *str) {
    if (str[0] == '\r' && str[1] == '\n') {
        return 2;
    } else if (str[0] == '\n' || str[0] == '\r') {
        return 1;
    }
    return 0;
}

int match_hexadecimal_prefix(char *str) {
    return str_start_with_ignore_case(str, "0x") ? 2 : 0;
}

int match_c_char_or_s_char(char *str, bool s_char = false) {
    const static char *simple_escape_sequence_list[] = {
        "\\'", "\\\"", "\\?", "\\\\", "\\a", "\\b",
        "\\f", "\\n",  "\\r", "\\t",  "\\v", NULL};
    int t;
    int m = 0;
    if (((!s_char && str[0] != '\'') || (s_char && str[0] != '"')) &&
        str[0] != '\n' && str[0] != '\r' && str[0] != '\\') {
        m = max(m, 1);
    }
    m = max(m, match_string_in_list(str, simple_escape_sequence_list));
    if (str[0] == '\\' && match_digit(str + 1)) {
        m = max(m, 2);
        if (match_digit(str + 2)) {
            m = max(m, 3);
            if (match_digit(str + 3)) {
                m = max(m, 4);
            }
        }
    }
    if (str[0] == '\\' && str[1] == 'x') {
        t = match_hexadecimal_digit_sequence(str + 2);
        if (t) {
            m = max(m, 2 + t);
        }
    }
    return m;
}

int match_c_char_sequence_or_s_char_sequence(char *str, bool s_char = false) {
    int cur_pos = 0;
    int t = 0;
    while (t = match_c_char_or_s_char(str + cur_pos, s_char)) {
        cur_pos += t;
    }
    return cur_pos;
}

/**
 * Functions to match various types of tokens
 */
int match_keyword(char *str) {
    const static char *keywords_list[]{
        "auto",    "enum",   "restrict", "unsigned",   "break",  "extern",
        "return",  "void",   "case",     "float",      "short",  "volatile",
        "char",    "for",    "signed",   "while",      "const",  "goto",
        "sizeof",  "_Bool",  "continue", "if",         "static", "_Complex",
        "default", "inline", "struct",   "_Imaginary", "do",     "int",
        "switch",  "double", "long",     "typedef",    "else",   "register",
        "union",   NULL};
    return match_string_in_list(str, keywords_list);
}

int match_identifier(char *str) {
    int cur_pos = 0;
    while (str[cur_pos]) {
        if (match_identifier_nondigit(str + cur_pos) ||
            (cur_pos != 0 && match_digit(str + cur_pos))) {
            cur_pos++;
        } else {
            break;
        }
    }
    return cur_pos;
}

int match_integer_constant(char *str) {
    // int decimal = 0;
    // int octal = 1;
    // int hexadecimal = 2;
    int cur_pos = 0;
    int type = 0;

    // prefix
    if (match_hexadecimal_prefix(str + cur_pos)) {
        type = 2;
        cur_pos += 2;
    } else if (str[cur_pos] == '0') {
        type = 1;
    }

    bool valid = false;

    while (str[cur_pos]) {
        char c = str[cur_pos];
        if ((type == 0 && match_digit(str + cur_pos)) ||
            (type == 1 && match_octal_digit(str + cur_pos)) ||
            (type == 2 && match_hexadecimal_digit(str + cur_pos))) {
            cur_pos++;
            valid = true;
        } else {
            break;
        }
    }
    if (!valid) {
        return 0;
    }
    // suffix
    const static char *suffix_list[] = {"u",   "l",  "ll",  "ul",
                                        "ull", "lu", "llu", NULL};
    cur_pos += match_string_in_list(str + cur_pos, suffix_list, false);
    return cur_pos;
}

int match_floating_constant(char *str) {
    int cur_pos = 0;

    // prefix
    bool is_hex = match_hexadecimal_prefix(str + cur_pos);
    if (is_hex) {
        cur_pos += 2;
    }
    int t = (!is_hex) ? match_digit_sequence(str + cur_pos)
                      : match_hexadecimal_digit_sequence(str + cur_pos);
    bool is_fractional_constant = str[cur_pos + t] == '.';
    if (is_fractional_constant) {
        int t1 = (!is_hex)
                     ? match_digit_sequence(str + cur_pos + t + 1)
                     : match_hexadecimal_digit_sequence(str + cur_pos + t + 1);
        if (t == 0 && t1 == 0) {
            return 0;
        }
        cur_pos += (t + 1 + t1);
    } else {
        if (t == 0) {
            return 0;
        }
        cur_pos += t;
    }

    t = (!is_hex) ? match_exponent_part(str + cur_pos)
                  : match_binary_exponent_part(str + cur_pos);
    if ((!is_fractional_constant || is_hex) && t == 0) {

        return 0;
    }
    cur_pos += t;

    // suffix
    cur_pos += match_floating_suffix(str + cur_pos);
    return cur_pos;
}

int match_character_constants(char *str) {
    int cur_pos = 0;
    if (str[cur_pos] == 'L') {
        cur_pos++;
    }
    if (str[cur_pos] != '\'') {
        return 0;
    }
    cur_pos++;
    int t = match_c_char_sequence_or_s_char_sequence(str + cur_pos);
    if (!t) {
        return 0;
    }
    cur_pos += t;
    if (str[cur_pos] != '\'') {
        return 0;
    }
    cur_pos++;
    return cur_pos;
}

int match_string_literals(char *str) {
    int cur_pos = 0;
    if (str[cur_pos] == 'L') {
        cur_pos++;
    }
    if (str[cur_pos] != '\"') {
        return 0;
    }
    cur_pos++;
    cur_pos += match_c_char_sequence_or_s_char_sequence(str + cur_pos, true);
    if (str[cur_pos] != '\"') {
        return 0;
    }
    cur_pos++;
    return cur_pos;
}

int match_punctuator(char *str) {
    const static char *punctuator_list[] = {
        "[",   "]",  "(",  ")",  "{",  "}",  ".",  "->",  "++",  "--",   "&",
        "*",   "+",  "-",  "~",  "!",  "/",  "%",  "<<",  ">>",  "<",    ">",
        "<=",  ">=", "==", "!=", "^",  "|",  "&&", "||",  "?",   ":",    ";",
        "...", "=",  "*=", "/=", "%=", "+=", "-=", "<<=", ">>=", "&=",   "^=",
        "|=",  ",",  "#",  "##", "<:", ":>", "<%", "%>",  "%:",  "%:%:", NULL};
    return match_string_in_list(str, punctuator_list);
}

// int match_preprocessing_numbers(char *str) {
//     int cur_pos = 0;
//     if (match_digit(str)) {
//         cur_pos++;
//     } else if (str[0] == '.' && match_digit(str + 1)) {
//         cur_pos += 2;
//     } else {
//         return 0;
//     }

//     bool valid = false;
//     while (true) {
//         int m = match_identifier_nondigit(str + cur_pos);
//         m = max(m, match_identifier_nondigit(str + cur_pos));
//         const static char *parts_of_pp_number_list[] = {"e-", "e+", "p-",
//         "p+",
//                                                         NULL};
//         m = max(m, match_string_in_list(str + cur_pos,
//         parts_of_pp_number_list,
//                                         false));
//         m = max(m, (int)(str[cur_pos] == '.'));
//         if (m) {
//             valid = true;
//             cur_pos += m;
//         } else {
//             break;
//         }
//     }
//     if (!valid) {
//         return 0;
//     }
//     return cur_pos;
// }

int match_line_comment(char *str) {
    int cur_pos = 0;
    if (str_start_with(str, "//")) {
        cur_pos += 2;
        while (str[cur_pos]) {
            if (match_line_end(str + cur_pos)) {
                return cur_pos;
            }
            cur_pos++;
        }
    }
    return 0;
}

int match_block_comment(char *str) {
    int cur_pos = 0;
    if (str_start_with(str, "/*")) {
        cur_pos += 2;
        while (str[cur_pos]) {
            if (str[cur_pos] == '*' && str[cur_pos + 1] == '/') {
                cur_pos += 2;
                return cur_pos;
            }
            cur_pos++;
        }
    }
    return 0;
}

int match_spaces(char *str) {
    int cur_pos = 0;
    char c;
    while (c = str[cur_pos]) {
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f') {
            cur_pos++;
        } else {
            break;
        }
    }
    return cur_pos;
}

void generate_offset2length(char *str, map<int, int> &result) {
    int cur_pos = 0;
    int line_start = 0;
    if (str[cur_pos]) {
        while (true) {
            int t = match_line_end(str + cur_pos);
            if (!t) {
                cur_pos++;
            } else {
                cur_pos += t;
                result[line_start] = cur_pos - line_start;
                line_start = cur_pos;
            }
            if (!str[cur_pos]) {
                result[line_start] = cur_pos - line_start;
                break;
            }
        }
    }
}

matcher_type matchers[] = {
    {.type = KEYWORD, .func = match_keyword},
    {.type = IDENTIFIER, .func = match_identifier},
    {.type = INTEGER_CONSTANT, .func = match_integer_constant},
    {.type = FLOATING_CONSTANT, .func = match_floating_constant},
    {.type = CHARACTER_CONSTANTS, .func = match_character_constants},
    {.type = STRING_LITERALS, .func = match_string_literals},
    {.type = PUNCTUATOR, .func = match_punctuator},
    // {.type = PREPROCESSING_NUMBERS, .func = match_preprocessing_numbers},
    {.type = LINE_COMMENT, .func = match_line_comment},
    {.type = BLOCK_COMMENT, .func = match_block_comment},
    {.type = SPACES, .func = match_spaces},
};

/**
 * The main part of the lexer. It will try all the sub-functions defined in
 * matchers and find the best match. Then try the next position in the string
 * until the end
 */
void match(char *str, match_result_type &match_result) {
    match_result.src_str = str;
    match_result.length = strlen(str);
    generate_offset2length(str, match_result.offset2length);

    int cur_pos = 0;
    bool error = false;
    int error_start = 0;
    while (true) {
        int max_len = 0;
        matcher_type *best_matcher = NULL;
        for (auto &matcher : matchers) { // try each sub-functions
            int r = matcher.func(str + cur_pos);
            if (r > max_len) {
                best_matcher = &matcher;
                max_len = r;
            }
        }

        if (error && (best_matcher || !str[cur_pos])) {
            lexeme_type r = {.type = UNKNOWN,
                             .start = error_start,
                             .length = cur_pos - error_start,
                             .matcher = NULL};
            match_result.offset2lexeme[r.start] = r;
            error = false;
        }

        if (!str[cur_pos]) {
            break;
        }

        if (best_matcher) {
            lexeme_type r = {.type = best_matcher->type,
                             .start = cur_pos,
                             .length = max_len,
                             .matcher = best_matcher};
            match_result.offset2lexeme[r.start] = r;
            cur_pos += max_len;
        } else {
            if (!error) {
                error = true;
                error_start = cur_pos;
            }
            cur_pos++;
        }
    }
}