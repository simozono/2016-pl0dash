%{
  #include <stdio.h>
  #include <stdlib.h>
  extern char *yytext;
  extern int yylex(void);
  int yyerror(char *s);
%}

%token T_NUMBER
%token T_LPAR T_RPAR T_PLUS T_MINUS
%token T_MULTI T_DIVIDE
%token T_WHITESPC T_ERROR T_EOF

%start expression

%%
expression:	expression T_PLUS term
			{ printf("POP B\n");
			  printf("POP A\n");
			  printf("PLUS\n");
			  printf("PUSH C\n");
			}
		| term
		;

term:		term T_MULTI factor
			{ printf("POP B\n");
			  printf("POP A\n");
			  printf("MULTI\n");
			  printf("PUSH C\n");
                        }
		| factor
		;

factor:		T_LPAR expression T_RPAR
		| T_NUMBER
			{ printf("LOAD A, %s\n", yytext);
			  printf("PUSH A\n");
                        }
		;
%%
int yyerror(char *error_message) {
  printf("エラー: %s\n", error_message);
  return(EXIT_FAILURE); /* 終了ステータスと同じものを返しておく */
}
