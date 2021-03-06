/* PL/0' 用 (LA)LR構文解析器メイン
 *              2016年後期 鹿児島高専
 *              3年生 言語処理系 授業用
 *
 */

#include <stdio.h>
#include <stdlib.h>

extern int yyparse();
extern FILE *yyin;      /* 読み込むソースファイル */

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf ("ソースプログラムのファイル名のみ指定してください\n");
    exit(EXIT_FAILURE);
  }

  yyin = fopen(argv[1], "r");
  if (yyin  == NULL) {
    printf ("%s というファイルがない\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  /* 構文解析スタート */
  if (yyparse() != 0) {
	 printf("構文解析エラー\n");
	 exit(EXIT_FAILURE);
  } else {
	printf("構文解析成功\n");
  }	
}
