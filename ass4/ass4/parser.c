/**************************************************************************
File name: parser.c
Compiler: MS Visual Studio 2010
Author: Eric Dodds, 040-701-142
Course: CST 8152 – Compilers, Lab Section: 402
Assignment: 3 - Parser
Date: December 8th 2013
Professor: Sv. Ranev
Purpose: Parse tokens and check syntax
Function list: 
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
**************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "parser.h"

extern Token mlwpar_next_token(Buffer * sc_buf);
extern int line;
extern STD sym_table;
extern Buffer * str_LTBL;
extern char * kw_table[];

static Token lookahead_token;
static Buffer * sc_buf;

int synerrno = 0;
/**************************************************************************
Purpose: Start the parser and check for EOF
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void parser(Buffer * in_buf)
{
	sc_buf = in_buf;
	lookahead_token = mlwpar_next_token(sc_buf);
	program(); 
	match(SEOF_T,NO_ATTR);
	gen_incode("PLATY: Source file parsed");
}

/**************************************************************************
Purpose: Match a given token with the token being looked for
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: int pr_token_code, int pr_token_attribute
Return value: void
**************************************************************************/
void match(int pr_token_code, int pr_token_attribute)
{
	if(pr_token_code != lookahead_token.code)
	{
		syn_eh(pr_token_code);
		return;
	}
	if(pr_token_code == KW_T)
	{
		if(pr_token_attribute != lookahead_token.attribute.kwt_idx)
		{
			syn_eh(pr_token_code);
			return;
		}
	}
	if(pr_token_code == LOG_OP_T)
	{
		if(pr_token_attribute != lookahead_token.attribute.log_op)
		{
			syn_eh(pr_token_code);
			return;
		}
	}
	if(pr_token_code == ART_OP_T)
	{
		if(pr_token_attribute != lookahead_token.attribute.arr_op)
		{
			syn_eh(pr_token_code);
			return;
		}
	}
	if(pr_token_code == REL_OP_T)
	{
		if(pr_token_attribute != lookahead_token.attribute.rel_op)
		{
			syn_eh(pr_token_code);
			return;
		}
	}
	if(pr_token_code == SEOF_T)
		return;

	lookahead_token = mlwpar_next_token(sc_buf);
	if(lookahead_token.code == ERR_T)
	{
		syn_printe();
		lookahead_token = mlwpar_next_token(sc_buf);
		synerrno++;
		return;
	}
	return;
}

/**************************************************************************
Purpose: Handle errors and call print error function
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void syn_eh(int sync_token_code)
{
	syn_printe();
	synerrno++;
	while(lookahead_token.code != sync_token_code)
	{
		lookahead_token = mlwpar_next_token(sc_buf);
		if(lookahead_token.code == SEOF_T && sync_token_code == SEOF_T)
			return;
		if(lookahead_token.code == SEOF_T && sync_token_code != SEOF_T)
			exit(synerrno);
	}
	lookahead_token = mlwpar_next_token(sc_buf);
	return;
}

/**************************************************************************
Purpose: Print the appropriate token error message
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void syn_printe()
{
	Token t = lookahead_token;

	printf("PLATY: Syntax error:  Line:%3d\n", line);
	printf("*****  Token code:%3d Attribute: ", t.code);

	switch(t.code){
		case ERR_T: /* ERR_T     0   Error token */
			printf("%s\n",t.attribute.err_lex);
			break;
		
		case SEOF_T: /*SEOF_T    1   Source end-of-file token */
			printf("NA\n" );
			break;
		
		case AVID_T: /* AVID_T    2   Arithmetic Variable identifier token */
		case SVID_T :/* SVID_T    3  String Variable identifier token */
			printf("%s\n",sym_table.pstvr[t.attribute.get_int].plex);
			break;
		
		case FPL_T: /* FPL_T     4  Floating point literal token */
			printf("%5.1f\n",t.attribute.flt_value);
			break;
		
		case INL_T: /* INL_T      5   Integer literal token */
			printf("%d\n",t.attribute.get_int);
			break;
		
		case STR_T:/* STR_T     6   String literal token */
			printf("%s\n",b_get_chmemloc(str_LTBL,(short)t.attribute.get_int));
			break;
		
		case SCC_OP_T: /* 7   String concatenation operator token */
			printf("NA\n" );
			break;
		
		case ASS_OP_T:/* ASS_OP_T  8   Assignment operator token */
			printf("NA\n" );
			break;
		
		case ART_OP_T:/* ART_OP_T  9   Arithmetic operator token */
			printf("%d\n",t.attribute.get_int);
			break;
		
		case REL_OP_T: /*REL_OP_T  10   Relational operator token */ 
			printf("%d\n",t.attribute.get_int);
			break;
		
		case LOG_OP_T:/*LOG_OP_T 11  Logical operator token */
			printf("%d\n",t.attribute.get_int);
			break;
		
		case LPR_T: /*LPR_T    12  Left parenthesis token */
			printf("NA\n" );
			break;
		
		case RPR_T: /*RPR_T    13  Right parenthesis token */
			printf("NA\n" );
			break;
		
		case LBR_T: /*    14   Left brace token */
			printf("NA\n" );
			break;
		
		case RBR_T: /*    15  Right brace token */
			printf("NA\n" );
			break;
		
		case KW_T: /*     16   Keyword token */
			printf("%s\n",kw_table [t.attribute.get_int]);
			break;
		
		case COM_T: /* 17   Comma token */
			printf("NA\n");
			break;
		
		case EOS_T: /*    18  End of statement *(semi - colon) */
			printf("NA\n" );
			break; 		
		
		default:
			printf("PLATY: Scanner error: invalid token code: %d\n", t.code);
    }
}

/**************************************************************************
Purpose: Prints a given string
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: char string[]
Return value: void
**************************************************************************/
void gen_incode(char string[])
{
	printf("%s\n", string);
}

/**************************************************************************
Purpose: Check for PLATYPUS keyword
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void program(void){
	match(KW_T, PLATYPUS);
	match(LBR_T,NO_ATTR);
	opt_statements();
	match(RBR_T,NO_ATTR);
	gen_incode("PLATY: Program parsed");
}

/**************************************************************************
Purpose: Call statement functions
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void statements(void)
{
	statement();
	statements_prime();
}

/**************************************************************************
Purpose: Check for AVID, SVID and keywords
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void statements_prime(void)
{
	switch (lookahead_token.code)
	{
		case AVID_T:
		case SVID_T:	
			statement();
			statements_prime();
			break;
		case KW_T:
			if (lookahead_token.attribute.get_int != PLATYPUS
				&& lookahead_token.attribute.get_int != ELSE
				&& lookahead_token.attribute.get_int != THEN
				&& lookahead_token.attribute.get_int != REPEAT)
			{
				statement();
				statements_prime();
			}
			break;
		default:
			break;
	}
}

/**************************************************************************
Purpose: Check for AVID, SVID and keywords
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void statement(void)
{
	switch (lookahead_token.code)
	{
		case AVID_T:
			assignment_statement();
			break;
		case SVID_T:	
			assignment_statement();
			break;
		case KW_T:
			if (lookahead_token.attribute.get_int == IF)
				selection_statement();
			else if(lookahead_token.attribute.get_int == USING)
				iteration_statement();
			else if(lookahead_token.attribute.get_int == INPUT)
				input_statement();
			else if(lookahead_token.attribute.get_int == OUTPUT)
				output_statement();
			break;
		default:
			break;
	}
}

/**************************************************************************
Purpose: Call assignment expression and match EOS
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void assignment_statement(void)
{
	assignment_expression();
	match(EOS_T, NO_ATTR);
	gen_incode("PLATY: Assignment statement parsed");
}

/**************************************************************************
Purpose: Check AVID and SVID and call proper assignment functions
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void assignment_expression(void)
{
	switch(lookahead_token.code)
	{
		case AVID_T:
			match(AVID_T, NO_ATTR);
			match(ASS_OP_T, NO_ATTR);
			arithmetic_expression();
			gen_incode("PLATY: Assignment expression (arithmetic) parsed");
			break;
		case SVID_T:
			match(SVID_T, NO_ATTR);
			match(ASS_OP_T, NO_ATTR);
			string_expression();
			gen_incode("PLATY: Assignment expression (string) parsed");
			break;
		default:
			syn_printe();
			break;
	}
}

/**************************************************************************
Purpose: Matches IF statements
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void selection_statement(void)
{
	match(KW_T, IF);
	match(LPR_T, NO_ATTR);
	conditional_expression();
	match(RPR_T, NO_ATTR);
	match(KW_T, THEN);
	opt_statements();
	match(KW_T, ELSE);
	match(LBR_T, NO_ATTR);
	opt_statements();
	match(RBR_T, NO_ATTR);
	match(EOS_T, NO_ATTR);
	gen_incode("PLATY: IF statement parsed");
}

/**************************************************************************
Purpose: Check for type of optional statement
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void opt_statements(void)
{
/* FIRST set: {AVID_T,SVID_T,KW_T(but not … see above),e} */
	switch(lookahead_token.code)
	{
	case AVID_T:
	case SVID_T: 
		statements();
		break;
	case KW_T:	/* check for PLATYPUS, ELSE, THEN, REPEAT here and in statements_p()*/
		if (lookahead_token. attribute. get_int != PLATYPUS
			&& lookahead_token. attribute. get_int != ELSE
			&& lookahead_token. attribute. get_int != THEN
			&& lookahead_token. attribute. get_int != REPEAT){
			statements();
			break;
		}
	default: /*empty string – optional statements*/ ;
		gen_incode("PLATY: Opt_statements parsed");
		break;
	}
}

/**************************************************************************
Purpose: Check USING iteration statement
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void iteration_statement(void)
{
	match(KW_T, USING);
	match(LPR_T, NO_ATTR);
	assignment_expression();
	match(COM_T, NO_ATTR);
	conditional_expression();
	match(COM_T, NO_ATTR);
	assignment_expression();
	match(RPR_T, NO_ATTR);
	match(KW_T, REPEAT);
	match(LBR_T, NO_ATTR);
	opt_statements();
	match(RBR_T, NO_ATTR);
	match(EOS_T, NO_ATTR);
	gen_incode("PLATY: USING statement parsed");
}

/**************************************************************************
Purpose: Check INPUT statement
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void input_statement(void)
{
	match(KW_T, INPUT);
	match(LPR_T, NO_ATTR);
	variable_list();
	gen_incode("PLATY: Variable list parsed");
	match(RPR_T, NO_ATTR);
	match(EOS_T, NO_ATTR);
	gen_incode("PLATY: INPUT statement parsed");
}

/**************************************************************************
Purpose: Call variable list functions
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void variable_list(void)
{
	variable_identifier();
	variable_list_prime();
}

/**************************************************************************
Purpose: Check variable identifier type
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void variable_identifier(void)
{
	switch(lookahead_token.code)
	{
		case AVID_T:
			match(AVID_T, NO_ATTR);
			break; 
		case SVID_T:
			match(SVID_T, NO_ATTR);
			break;
		case STR_T:
			match(STR_T, NO_ATTR);
			break;
		default:
			syn_printe();
			break;
	}
}

/**************************************************************************
Purpose: Check variable list
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void variable_list_prime(void)
{
	switch(lookahead_token.code)
	{
		case COM_T:
			match(COM_T, NO_ATTR);
			variable_identifier();
			variable_list_prime();
			break;
		default:
			break;
	}
}

/**************************************************************************
Purpose: Check OUTPUT statement and match following items
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void output_statement(void)
{
	match(KW_T, OUTPUT);
	match(LPR_T, NO_ATTR);
	output_list();
	match(RPR_T, NO_ATTR);
	match(EOS_T, NO_ATTR);
	gen_incode("PLATY: OUTPUT statement parsed");
}

void output_list(void)
{
	switch(lookahead_token.code)
	{
	case AVID_T:
	case SVID_T:
		variable_list();
		gen_incode("PLATY: Variable list parsed");
		break;
	case STR_T:
		match(STR_T, NULL);
		gen_incode("PLATY: Output list (string literal) parsed");
		break;
	default:
		gen_incode("PLATY: Output list (empty) parsed");
		break;
	}
}
/**************************************************************************
Purpose: Check optional variable list
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void opt_variable_list(void)
{
	switch(lookahead_token.code)
	{
		case AVID_T:
		case SVID_T:
		case STR_T:
			variable_list();
			break;
		default:
			break;
	}
}

/**************************************************************************
Purpose: Check arithmetic expression
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void arithmetic_expression(void)
{
	switch(lookahead_token.code)
	{
		case FPL_T:
		case INL_T:
		case LPR_T:
		case AVID_T:
			additive_arithmetic_expression();
			break;
		case ART_OP_T:
			if(lookahead_token.attribute.arr_op == PLUS || lookahead_token.attribute.arr_op == MINUS)
				unary_arithmetic_expression();
			break;

		default:
			syn_printe();
			break;
	}
	gen_incode("PLATY: Arithmetic expression parsed");
}

/**************************************************************************
Purpose: Check unary arithmetic expressions
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void unary_arithmetic_expression(void)
{
	switch(lookahead_token.code)
	{
		case ART_OP_T:
			if(lookahead_token.attribute.arr_op == PLUS || lookahead_token.attribute.arr_op == MINUS)
			{
				match(ART_OP_T, lookahead_token.attribute.arr_op);
				primary_arithmetic_expression();
				break;
			}
			break;
		default:
			break;
	}
	gen_incode("PLATY: Unary arithmetic expression parsed");
}

/**************************************************************************
Purpose: Call additive arithmetic expression functions
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void additive_arithmetic_expression(void)
{
	multiplicative_arithmetic_expression();
	additive_arithmetic_expression_prime();
}

/**************************************************************************
Purpose: Check additive arithmetic expression
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void additive_arithmetic_expression_prime(void)
{
	switch(lookahead_token.code)
	{
		case ART_OP_T:
			if(lookahead_token.attribute.arr_op == PLUS || lookahead_token.attribute.arr_op == MINUS)
			{
				match(ART_OP_T, lookahead_token.attribute.arr_op);
				multiplicative_arithmetic_expression();
				additive_arithmetic_expression_prime();
				gen_incode("PLATY: Additive arithmetic expression parsed");
			}
			break;
		default:
			break;
	}
}

/**************************************************************************
Purpose: Call multiplicative arithmetic expression functions
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void multiplicative_arithmetic_expression(void)
{
	primary_arithmetic_expression();
	multiplicative_arithmetic_expression_prime();
}

/**************************************************************************
Purpose: Check multiplicative arithmetic expression
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void multiplicative_arithmetic_expression_prime(void)
{
	switch(lookahead_token.code)
	{
		case ART_OP_T:
			if(lookahead_token.attribute.arr_op == MULT || lookahead_token.attribute.arr_op == DIV)
			{
				match(ART_OP_T, lookahead_token.attribute.arr_op);
				primary_arithmetic_expression();
				multiplicative_arithmetic_expression_prime();						
				gen_incode("PLATY: Multiplicative arithmetic expression parsed");
			}
			break;
		default:
			break;
	}
}

/**************************************************************************
Purpose: Check primary arithmetic expression type
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void primary_arithmetic_expression(void)
{
	switch(lookahead_token.code)
	{
		case AVID_T:
			match(AVID_T, NO_ATTR);
			break;
		case FPL_T:
			match(FPL_T, NO_ATTR);
			break;
		case INL_T:
			match(INL_T, NO_ATTR);
			break;
		case LPR_T:
			match(LPR_T, NO_ATTR);
			arithmetic_expression();
			match(RPR_T, NO_ATTR);
			break;
		default:
			syn_printe();
			break;
	}
	gen_incode("PLATY: Primary arithmetic expression parsed");
}

/**************************************************************************
Purpose: Check string expression
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void string_expression(void)
{
	primary_string_expression();
	string_expression_prime();
	gen_incode("PLATY: String expression parsed");
}

/**************************************************************************
Purpose: Check string expression type
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void string_expression_prime(void)
{
	switch(lookahead_token.code)
	{
		case SCC_OP_T:
			match(SCC_OP_T, NO_ATTR);
			primary_string_expression();
			string_expression_prime();
			break;
		default:
			break;
	}
}

/**************************************************************************
Purpose: Check primary string expression type
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void primary_string_expression(void)
{
	switch(lookahead_token.code)
	{
		case SVID_T:
			match(SVID_T, NO_ATTR);
			break;
		case STR_T:
			match(STR_T, NO_ATTR);
			break;
		default:
			break;
	}
	gen_incode("PLATY: Primary string expression parsed");
}

/**************************************************************************
Purpose: Call conditional operator function
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void conditional_expression(void)
{
	logical_or_expression();
	gen_incode("PLATY: Conditional expression parsed");
}

/**************************************************************************
Purpose: Call logical or expression functions
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void logical_or_expression(void)
{
	logical_and_expression();
	logical_or_expression_prime();
}

/**************************************************************************
Purpose: Check logical or expression 
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void logical_or_expression_prime(void)
{
	switch(lookahead_token.code)
	{
		case LOG_OP_T:
			if(lookahead_token.attribute.log_op == OR)
			{
				match(LOG_OP_T, lookahead_token.attribute.log_op);
				logical_and_expression();
				logical_or_expression_prime();
				gen_incode("PLATY: Logical OR expression parsed");
			}
			break;
		default:
			break;
	}
}

/**************************************************************************
Purpose: Call logical and expression functions
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void logical_and_expression(void)
{
	relational_expression();
	logical_and_expression_prime();
}

/**************************************************************************
Purpose: Check logical and expression
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void logical_and_expression_prime(void)
{
	switch(lookahead_token.code)
	{
		case LOG_OP_T:
			if(lookahead_token.attribute.log_op == AND)
			{
				match(LOG_OP_T, lookahead_token.attribute.log_op);
				relational_expression();
				logical_and_expression_prime();
				gen_incode("PLATY: Logical AND expression parsed");
			}
			break;
		default:
			break;
	}
}

/**************************************************************************
Purpose: Check relational expressions and call appropriate functions
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void relational_expression(void)
{
	switch(lookahead_token.code)
	{
		case AVID_T:
		case INL_T:
		case FPL_T:
			primary_a_relational_expression();
			relational_expression_op();
			primary_a_relational_expression();
			break;
		case SVID_T:
		case STR_T:
			primary_s_relational_expression();
			relational_expression_op();
			primary_s_relational_expression();
			break;
		default:
			syn_printe();
			break;
	}
	gen_incode("PLATY: Relational expression parsed");
}

/**************************************************************************
Purpose: Check primary_a_relational_expression
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void primary_a_relational_expression(void)
{
	switch(lookahead_token.code)
	{
		case AVID_T:
			match(AVID_T, NO_ATTR);
			break;
		case INL_T:
			match(INL_T, NO_ATTR);
			break;
		case FPL_T:
			match(FPL_T, NO_ATTR);
			break;
		default:
			syn_printe();
			break;
	}
	gen_incode("PLATY: Primary a_relational expression parsed");
}

/**************************************************************************
Purpose: Check primary_s_relational_expression
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void primary_s_relational_expression(void)
{	
	primary_string_expression();
	gen_incode("PLATY: Primary s_relational expression parsed");
}

/**************************************************************************
Purpose: Check relational operator
Author: Eric Dodds
History/Versions: 1.0 December 8th 2013
Parameters: void
Return value: void
**************************************************************************/
void relational_expression_op(void)
{
	switch(lookahead_token.code)
	{
		case REL_OP_T:
			if(lookahead_token.attribute.rel_op == EQ)
				match(REL_OP_T, EQ);
			else if(lookahead_token.attribute.rel_op == NE)
				match(REL_OP_T, NE);
			else if(lookahead_token.attribute.rel_op == GT)
				match(REL_OP_T, GT);
			else if(lookahead_token.attribute.rel_op == LT)
				match(REL_OP_T, LT);
			break;
		default:
			syn_printe();
			break;
	}
}