#include "mpc.h"

#include <stdio.h>
#include <stdlib.h>

#include <editline/readline.h>
#include <editline/history.h>

/* Declare a buffer for user input of size 2048 */
int main(int argc, char** argv){

  mpc_parser_t* Number    = mpc_new("number");
  mpc_parser_t* Operator  = mpc_new("operator");
  mpc_parser_t* Expr      = mpc_new("expr");
  mpc_parser_t* Lispy     = mpc_new("lispy");

  mpca_lang(MPCA_LANG_DEFAULT,
	    "                                                      \
              number     : /-?[0-9]+/ ;                            \
              operator   : '+' | '-' | '*' | '/' ;                 \
              expr       : <number> | '(' <operator> <expr>+ ')' ; \
              lispy      : /^/ <operator> <expr>+ /$/ ;            \
            ",
	    Number, Operator, Expr, Lispy);

  /* Print version and exit information */
  puts("Lispy Version 0.0.0.0.1");
  puts("Press Ctrl+c to Exit\n");

  /* In a never-ending loop */
  while (1) {

    /* Output our prompt */
    char* input = readline("lispy> ");
    /* Add input to history */
    add_history(input);

    /* Echo input back to user */
    printf("Your input: %s\n", input);

    /* Parse Input */
    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Lispy, &r)) {
      /* On Success Print the AST */
      mpc_ast_print(r.output);
      mpc_ast_delete(r.output);
    } else {
      /* Otherwise Print the Error */
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    };

    /* Free retrieved input */
    free(input);

  };

  /* Undefine and Delete our Parsers */
  mpc_cleanup(4, Number, Operator, Expr, Lispy);

  return 0;
};
