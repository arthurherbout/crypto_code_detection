/*

Copyright 2012 William Hart. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of
      conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list
      of conditions and the following disclaimer in the documentation and/or other materials
      provided with the distribution.

THIS SOFTWARE IS PROVIDED BY William Hart ``AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL William Hart OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <stdio.h>
#include "parser.h"
#include "types.h"
#include "backend.h"

extern jmp_buf exc;

int main(void)
{
   ast_t * a;
   input_t * in = new_input();
   int jval;

   ast_init();
   sym_tab_init();
   types_init();

   printf("Welcome to Cesium v0.3\n\n");
   printf("> ");

   combinator_t * stmt = new_combinator();
   combinator_t * exp = new_combinator();
   combinator_t * paren = new_combinator();
   combinator_t * base = new_combinator();

   seq(paren, T_LIST,
          match("("),
          exp,
          match(")"),
       NULL);

   multi(base, T_NONE, 
          capture(T_INT, integer()),
          paren,
       NULL);

   expr(exp, base);

   expr_insert(exp, 0, T_ADD, EXPR_INFIX, ASSOC_LEFT, match("+"));
   expr_altern(exp, 0, T_SUB, match("-"));

   expr_insert(exp, 1, T_MUL, EXPR_INFIX, ASSOC_LEFT, match("*"));
   expr_altern(exp, 1, T_DIV, match("/"));
   expr_altern(exp, 1, T_REM, match("%"));

   seq(stmt, T_NONE,
          exp,
          match(";"),
       NULL);

   while (1)
   {
      if (!(jval = setjmp(exc)))
      {
         a = parse(in, stmt);
         if (!a) break;
      } else
      {
         while (read1(in) != '\n') ;
      }
      
      printf("\n> ");
      in->start = 0;
      in->length = 0;
   }

   printf("\n");

   return 0;

}
