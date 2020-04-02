#include <cerrno>
#include <cstring>
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <iterator>
#include <list>
#include <output.h>
#include <streambuf>
#include <string>
#include <lexer.h>
using namespace std;

/**
 * Print the usage of this program.
 */
void print_usage() {
    printf("Usage: lexical-analyzer [-l] [-c] [-c] [-h] [FILE]\n\n");
    printf("-l, --lexemes\tprint all lexemes matched.\n");
    printf("-c, --color\tprint input strings in color mode.\n");
    printf("-s, --statistics\tprint statistics message.\n");
    printf("-h, --help\tprint this help message.\n");
    printf("FILE\t\tsource code file to analysis. stdin will be used to read "
           "if FILE was not specified.\n");
}

bool opt_color = false;
bool opt_statistics = false;
bool opt_lexemes = false;
const char *opt_src_file = NULL;

/**
 * Function to parse the options
 */
void parse_option(int argc, char const *argv[]) {
    // parse options
    static struct option long_options[] = {
        {"color", no_argument, NULL, 'c'},
        {"statistics", no_argument, NULL, 's'},
        {"lexemes", no_argument, NULL, 'l'},
        {"help", no_argument, NULL, 'h'},
        {0, 0, 0, 0}};
    opterr = 0;
    int opt;
    while ((opt = getopt_long(argc, (char *const *)argv, "cslh", long_options,
                              NULL)) != -1) {
        switch (opt) {
        case 'c':
            opt_color = 1;
            break;
        case 's':
            opt_statistics = 1;
            break;
        case 'l':
            opt_lexemes = 1;
            break;
        case 'h':
            print_usage();
            exit(-1);
        case '?':
            printf("unknown arg: %s\n\n", argv[optind - 1]);
            print_usage();
            exit(-1);
        }
    }

    if (optind < argc - 1) {
        printf("only process one file at a time.\n\n");
        print_usage();
        exit(-1);
    }
    opt_src_file = argv[optind];
}

char *read_src_str_from_file(const char *file_path) {
    std::ifstream in;
    in.open(file_path);
    if (in.fail()) {
        return NULL;
    }
    in.seekg(0, std::ios::end);
    auto len = in.tellg();
    in.seekg(0, std::ios::beg);
    char *src_str = new char[len + 1];
    in.read(src_str, len);
    src_str[len] = '\0';
    in.close();
    return src_str;
}

char *read_src_str_from_stdin() {
    std::cin >> std::noskipws;
    std::istream_iterator<char> it(std::cin);
    std::istream_iterator<char> end;
    std::string str(it, end);
    auto len = str.length();
    char *src_str = new char[len + 1];
    src_str[len] = '\0';
    strcpy(src_str, str.c_str());
    return src_str;
}

int main(int argc, const char **argv) {
    parse_option(argc, argv);
    char *src_str = NULL;
    if (!opt_src_file) {
        src_str = read_src_str_from_stdin();
    } else {
        src_str = read_src_str_from_file(opt_src_file);
    }
    if (!src_str) {
        printf("failed to read: %s", strerror(errno));
        return errno;
    }
    match_result_type match_result;
    match(src_str, match_result);
    print_errors_in_match_results(match_result);
    if (opt_lexemes) {
        print_match_results(match_result);
    }
    if (opt_color) {
        print_match_results_in_color(match_result);
    }
    if (opt_statistics) {
        print_statistics_info(match_result);
    }

    return match_result.error;
}
