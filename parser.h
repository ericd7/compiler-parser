/**************************************************************************
File name: parser.h
Compiler: MS Visual Studio 2010
Author: Svillen Ranev, modified by Eric Dodds, 040701142 
Course: CST8152 – Compilers, Lab Section: 402
Assignment: 4 - Parser
Date: December 8th 2013
Professor: Sv. Ranev
Purpose: Declares constants and functions
**************************************************************************/
#ifndef PARSER_H_
#define PARSER_H_

#include "buffer.h"
#include "stable.h"
#include "token.h"

#define NO_ATTR		0

#define ELSE		0
#define IF			1
#define INPUT		2
#define OUTPUT		3
#define PLATYPUS	4
#define REPEAT		5
#define THEN		6
#define USING		7
#endif

void parser(Buffer * in_buf);
void match(int pr_token_code, int pr_token_attribute);
void syn_eh(int sync_token_code);
void syn_printe();
void gen_incode(char string[]);
void program(void);
void opt_statements(void);
void variable_list(void);
void statements(void);
void statements_prime(void);
void statement(void);
void assignment_statement(void);
void assignment_expression(void);
void selection_statement(void);
void opt_statements(void);
void iteration_statement(void);
void input_statement(void);
void variable_list(void);
void variable_identifier(void);
void variable_list_prime(void);
void output_statement(void);
void opt_variable_list(void);
void arithmetic_expression(void);
void unary_arithmetic_expression(void);
void additive_arithmetic_expression(void);
void additive_arithmetic_expression_prime(void);
void multiplicative_arithmetic_expression(void);
void multiplicative_arithmetic_expression_prime(void);
void primary_arithmetic_expression(void);
void string_expression(void);
void string_expression_prime(void);
void primary_string_expression(void);
void conditional_expression(void);
void logical_or_expression(void);
void logical_or_expression_prime(void);
void logical_and_expression(void);
void logical_and_expression_prime(void);
void relational_expression(void);
void relational_expression_op(void);
void primary_a_relational_expression(void);
void primary_s_relational_expression(void);
void opt_variable_list(void);

