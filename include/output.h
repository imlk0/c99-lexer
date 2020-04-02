#ifndef OUTPUT_H
#define OUTPUT_H

#include <lexer.h>

void print_match_results(match_result_type &match_result);
void print_match_results_in_color(match_result_type &match_result);
void print_errors_in_match_results(match_result_type &match_result);
void print_statistics_info(match_result_type &match_result);

#endif