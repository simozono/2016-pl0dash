/* PL/0' 用 LL(1)再帰下降型構文解析器 No.02
 *              2016年後期 鹿児島高専
 *              3年生 言語処理系 授業用
 *   * 構文解析しか行っていない
 *   * ループを使わず再帰のみでやっている
 *   * 変数のIDと関数のIDの分岐を一応やっている
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "misc.h"
#include "../scanner/tokentable.h"

extern FILE *yyin;   /* 読み込むソースファイル */
extern int yylex();  /* lex の字句解析 */
extern int line_no;  /* 行番号 */
extern char *yytext; /* lex よりレクシムが入る */

int getToken(void);  /* トークンを取得する関数 */

/* 非終端記号に対応した関数 */
void parse_Program(void);
void parse_Block(void);
void parse_ConstDeclList(void);
void parse_ConstDecl(void);
void parse_ConstIdList(void);
void parse_ConstIdList_dash(void);
void parse_VarDeclList(void);
void parse_VarDecl(void);
void parse_VarIdList(void);
void parse_VarIdList_dash(void);
void parse_FuncDeclList(void);
void parse_FuncDecl(void);
void parse_FuncDeclIdList(void);
void parse_FuncDeclIdList_dash(void);
void parse_Statement(void);
void parse_StatementList(void);
void parse_StatementList_dash(void);
void parse_Condition(void);
void parse_Expression(void);
void parse_Expression_dash(void);
void parse_Term(void);
void parse_Term_dash(void);
void parse_Factor(void);
void parse_FuncArgList(void);
void parse_FuncArgList_dash(void);

int nextToken; /* 次のトークンが入る変数 */

int getToken(void) { /* トークンを取得する関数 */
  int token = yylex();
  /* yylex()が0を返す時がEOFのようだ */
  if (token == 0) token = T_EOF;
  return token;
}

/* ***** 記号表関係ここから ***** */

#define MAX_ID_NAME     31  /* ID の最大値 */
#define MAX_TABLE_SIZE 200  /* 記号表の大きさ */

typedef enum { /* 記号表に登録する名前の種別 */
  const_id,  /* 定数 */
  var_id,    /* 変数 */
  func_id,   /* 関数 */
  param_id   /* 関数の仮引数 */
} id_type;

struct table_entry { /* 記号表に登録する要素 */
  id_type type;               /* 種別 */
  char id_name[MAX_ID_NAME];  /* 名前 */
  int line_no;                /* 宣言されたソースコード上の行数 */
};

static struct table_entry symbol_table[MAX_TABLE_SIZE]; /* 記号表 */
static int symbol_table_ptr = 0 ; /* 記号表の現在位置を示すポインタ */

int search_table(char *id_name) { /* 記号表の検索 */
  int i = symbol_table_ptr;
  strcpy(symbol_table[0].id_name, id_name);  /* 番兵 */
  while(strcmp(id_name, symbol_table[i].id_name)) { 
    i--;
  }
  return i; /* 0 だったら登録されていない。1以上は記号表の位置 */
}

struct table_entry get_table(int ptr) {
  return symbol_table[ptr];
}

int add_table(id_type type, char *id_name, int line_no) {
  if (search_table(id_name) > 0) {
     pl0_error(id_name, line_no, "既に登録されています");
    exit(EXIT_FAILURE);
  }

  symbol_table_ptr++;

  if (symbol_table_ptr >= MAX_TABLE_SIZE) {
     pl0_error(id_name, line_no, "名前の登録数が多すぎます");
  }  else {
     symbol_table[symbol_table_ptr].type = type;
     strcpy(symbol_table[symbol_table_ptr].id_name, id_name);
     symbol_table[symbol_table_ptr].line_no = line_no;
  }

  return symbol_table_ptr;
}

/* ***** 記号表関係ここまで ***** */

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
  nextToken = getToken();
  parse_Program();
  if (nextToken != T_EOF) pl0_error("",line_no, "EOFでない");

  /* 正常終了 */
  printf("構文解析は全て成功\n");
  exit(EXIT_SUCCESS);
}

void parse_Program() {
  /* <Program> -> <Block> T_PRIOD */ 
  printf("Enter Program\n");
  parse_Block();
  if (nextToken != T_PERIOD) pl0_error(yytext, line_no, "ピリオドでない");
  nextToken = getToken();
  printf("Exit  Program\n");
}

void parse_Block() {
  /* <Block> -> <ConstDeclList> <VarDeclList> <FuncDeclList> <Statement> */
  printf("Enter Block\n");
  parse_ConstDeclList();
  parse_VarDeclList();
  parse_FuncDeclList();
  parse_Statement();
  printf("Exit  Block\n");
}

void parse_ConstDeclList() {
  /* <ConstDeclList> -> <ConstDecl> <ConstDeclList> | ε */
  printf("Enter ConstDeclList\n");
  if (nextToken == T_CONST) { /* First(<ConstDecl>) */
    parse_ConstDecl();
    parse_ConstDeclList();
  }
  printf("Exit  ConstDeclList\n");
}

void parse_ConstDecl() {
  /* <ConstDecl> -> T_CONST <ConstIdList> T_SEMIC */
  printf("Enter ConstDecl\n");
  /* T_CONST では何もしない。次のトークンを読む */
  nextToken = getToken();
  parse_ConstIdList();
  if (nextToken != T_SEMIC) pl0_error(yytext, line_no, ";でない");
  nextToken = getToken();
  printf("Exit  ConstDecl\n");
}

void parse_ConstIdList() {
  /* <ConstIdList> -> T_ID T_EQ T_NUMBER <ConstIdList_dash> */
  char const_name[MAX_ID_NAME];
  printf("Enter ConstIdList\n");
  if (nextToken != T_ID) pl0_error(yytext, line_no, "定数名でない");
  strcpy(const_name, yytext);
  nextToken = getToken();
  if (nextToken != T_EQ) pl0_error(yytext, line_no, "=でない");
  nextToken = getToken();
  if (nextToken != T_NUMBER) pl0_error(yytext, line_no, "数でない");
  /* 定数名の登録および値の設定をここで行う */
  add_table(const_id, const_name, line_no);
  nextToken = getToken();
  parse_ConstIdList_dash();
  printf("Exit  ConstIdList\n");
}

void parse_ConstIdList_dash() {
  /* <ConstIdList_dash> -> T_COMMA T_ID T_EQ T_NUMBER <ConstIdList_dash> | ε */
  char const_name[MAX_ID_NAME];
  printf("Enter ConstIdList_dash\n");
  if (nextToken == T_COMMA) {
    nextToken = getToken();
    if (nextToken != T_ID) pl0_error(yytext, line_no, "定数名でない");
    strcpy(const_name, yytext);
    nextToken = getToken();
    if (nextToken != T_EQ) pl0_error(yytext, line_no, "=でない");
    nextToken = getToken();
    if (nextToken != T_NUMBER) pl0_error(yytext, line_no, "数でない");
    /* 定数名の登録および値の設定をここで行う */
    add_table(const_id, const_name, line_no);
    nextToken = getToken();
    parse_ConstIdList_dash();
  }
  printf("Exit  ConstIdList_dash\n");
}

void parse_VarDeclList() {
  /* <VarDeclList> -> <VarDecl> <VarDeclList> | ε */
  printf("Enter VarDeclList\n");
  if (nextToken == T_VAR) { /* First(<VarDecl>) */
    parse_VarDecl();
    parse_VarDeclList();
  }
  printf("Exit  VarDeclList\n");
}

void parse_VarDecl() {
  /* <VarDec> -> T_VAR <VarIdList> T_SEMIC */
  printf("Enter VarDecl\n");
  /* T_VAR では何もしない。次のトークンを読む */
  nextToken = getToken();
  parse_VarIdList();
  if (nextToken != T_SEMIC) pl0_error(yytext, line_no, ";でない");
  nextToken = getToken();
  printf("Exit  VarDecl\n");
}

void parse_VarIdList() {
  /* <VarIdList_dash> -> T_ID <VarIdList_dash> */
  char var_name[MAX_ID_NAME];
  printf("Enter VarIdList\n");
  if (nextToken != T_ID) pl0_error(yytext, line_no, "変数名でない");
  /* 変数名の登録をここで行う */
  strcpy(var_name, yytext);
  add_table(var_id, var_name, line_no);
  nextToken = getToken();
  parse_VarIdList_dash();
  printf("Exit  VarIdList\n");
}

void parse_VarIdList_dash() {
  /* <VarIdList_dash> -> T_COMMA T_ID <VarIdList_dash> | ε */
  char var_name[MAX_ID_NAME];
  printf("Enter VarIdList_dash\n");
  if (nextToken == T_COMMA) {
    nextToken = getToken();
    if (nextToken != T_ID) pl0_error(yytext, line_no, "変数名でない");
    /* 変数名の登録をここで行う */
    strcpy(var_name, yytext);
    add_table(var_id, var_name, line_no);
    nextToken = getToken();
    parse_VarIdList_dash();
  }
  printf("Exit  VarIdList_dash\n");
}

void parse_FuncDeclList() {
  /* <FuncDeclList> -> <FuncDecl> <FuncDeclList> | ε */
  printf("Enter VarDeclList\n");
  if (nextToken == T_FUNC) { /* First(<FuncDecl>) */
    parse_FuncDecl();
    parse_FuncDeclList();
  }
  printf("Exit  VarDeclList\n");
}

void parse_FuncDecl() {
  /* <FuncDecl> -> T_FUNC T_ID T_LPAR <FuncDeclIdList> T_RPAR <Block> T_SEMIC */
  char func_name[MAX_ID_NAME];
  printf("Enter FuncDecl\n");
  /* T_FUNC では何もしない。次のトークンを読む */
  nextToken = getToken();
  if (nextToken != T_ID) pl0_error(yytext, line_no, "関数名でない");
  strcpy(func_name, yytext);
  add_table(func_id, func_name, line_no);
  nextToken = getToken();
  if (nextToken != T_LPAR) pl0_error(yytext, line_no, "(でない");
  nextToken = getToken();
  parse_FuncDeclIdList();
  if (nextToken != T_RPAR) pl0_error(yytext, line_no, ")でない");
  nextToken = getToken();
  parse_Block();
  if (nextToken != T_SEMIC) pl0_error(yytext, line_no, ";でない");
  nextToken = getToken();
  printf("Exit  FuncDecl\n");
}

void parse_FuncDeclIdList() {
  /* <FuncDeclIdList> -> T_ID <FuncDeclIdList_dash> | ε */
  char param_name[MAX_ID_NAME];
  printf("Enter FuncDeclIdList\n");
  if (nextToken == T_ID) {
    strcpy(param_name, yytext);
    add_table(param_id, param_name, line_no);
    nextToken = getToken();
    parse_FuncDeclIdList_dash();
  }
  printf("Exit  FuncDeclIdList\n");
}

void parse_FuncDeclIdList_dash() {
  /* <FuncDeclIdList_dash> -> T_COMMA T_ID <FuncDeclIdList_dash> | ε */
  char param_name[MAX_ID_NAME];
  printf("Enter FuncDeclIdList_dash\n");
  if (nextToken == T_COMMA) {
    nextToken = getToken();
    if (nextToken != T_ID) pl0_error(yytext, line_no, "仮引数名でない");
    strcpy(param_name, yytext);
    add_table(param_id, param_name, line_no);
    nextToken = getToken();
    parse_FuncDeclIdList_dash();
  }
  printf("Exit  FuncDeclIdList_dash\n");
}

void parse_Statement() {
  printf("Enter Statement\n");
  if (nextToken == T_ID) { /* 代入文 */
    nextToken = getToken();
    if (nextToken != T_COLEQ) pl0_error(yytext, line_no, ":=がない");
    nextToken = getToken();
    parse_Expression();
  } else if (nextToken == T_BEGIN) { /* begn ～ end */
    nextToken = getToken();
    parse_StatementList();
    if (nextToken != T_END) pl0_error(yytext, line_no, "endがない");
    nextToken = getToken();
  } else if (nextToken == T_IF) { /* if then */
    nextToken = getToken();
    parse_Condition();
    if (nextToken != T_THEN) pl0_error(yytext, line_no, "thenがない");
    nextToken = getToken();
    parse_Statement();
  } else if (nextToken == T_WHILE)  { /* while do */
    nextToken = getToken();
    parse_Condition();
    if (nextToken != T_DO) pl0_error(yytext, line_no, "doがない");
    nextToken = getToken();
    parse_Statement();
  } else if (nextToken == T_RETURN) { /* return */
    nextToken = getToken();
    parse_Expression();
  } else if (nextToken == T_WRITE) { /* write */
    nextToken = getToken();
    parse_Expression();
  } else if (nextToken ==  T_WRITELN) { /* writeln */
    /* writeln の処理 */
    nextToken = getToken();
  }
  printf("Exit  Statement\n");
}

void parse_StatementList() {
  /* <StatementList> -> <Statement> <StatementList_dash> */
  printf("Enter StatementList\n");
  parse_Statement();
  parse_StatementList_dash();
  printf("Exit  StatementList\n");
}

void parse_StatementList_dash() {
  /* <StatementList_dash> -> T_SEMIC <Statement> <StatementList_dash> | ε */
  printf("Enter StatementList_dash\n");
  if (nextToken == T_SEMIC) {
    nextToken = getToken();
    parse_Statement();
    parse_StatementList_dash();
  }
  printf("Exit  StatementList_dash\n");
}

void parse_Condition() {
  printf("Enter Condition\n");
  int operator ; /* T_EQ や T_GT を一時格納 */
  if (nextToken == T_ODD) {
    nextToken = getToken();
    parse_Expression();
    /* ここで T_ODDの処理 */
  } else {
    parse_Expression();
    if (nextToken == T_EQ || nextToken == T_NOTEQ
        || nextToken == T_LT || nextToken == T_GT
        || nextToken == T_LE || nextToken == T_GE) {
      operator = nextToken;
    } else {
      pl0_error(yytext, line_no, "比較演算子がない");
    }
    nextToken = getToken();
    parse_Expression();
    /* ここで operator 処理 */
  }
  printf("Exit  Condition\n");
}

void parse_Expression() {
  printf("Enter Expression\n");
  if (nextToken == T_PLUS) {
    nextToken = getToken();
    parse_Term();
    /* ここで 0 Term + をスタックにつむ */
    parse_Expression_dash();
  } else if (nextToken == T_MINUS) {
    nextToken = getToken();
    parse_Term();
    /* ここで 0 Term - をスタックにつむ */
    parse_Expression_dash();
  } else {
    parse_Term();
    parse_Expression_dash();
  }
  printf("Exit  Expression\n");
}

void parse_Expression_dash() {
  printf("Enter Expression_dash\n");
  if (nextToken == T_PLUS) {
    nextToken = getToken();
    parse_Term();
    /* ここで + の処理 */
    parse_Expression_dash();
  } else if (nextToken == T_MINUS) {
    nextToken = getToken();
    parse_Term();
    /* ここで - の処理 */
    parse_Expression_dash();
  } else {
  }
  printf("Exit  Expression_dash\n");
}

void parse_Term() {
  printf("Enter Term\n");
  parse_Factor();
  parse_Term_dash();
  printf("Exit  Term\n");
}

void parse_Term_dash() {
  printf("Enter Term_dash\n");
  if (nextToken == T_MULTI) {
    nextToken = getToken();
    parse_Factor();
    /* ここで * の処理 */
    parse_Term_dash();
  } else if (nextToken == T_DIVIDE) {
    nextToken = getToken();
    parse_Factor();
    /* ここで / の処理 */
    parse_Term_dash();
  }
  printf("Exit  Term_dash\n");
}

void parse_Factor() {
  int t_ptr ;
  struct table_entry t_ent;
  char id_name[MAX_ID_NAME];
  
  printf("Enter Factor\n");
  if (nextToken == T_ID) { 
    /* 右辺値変数 or 関数呼び出しの判断をしなければならない */
    strcpy(id_name, yytext);
    t_ptr = search_table(id_name); /* T_ID を検索 */
    if (t_ptr == 0) pl0_error(id_name, line_no, "その変数/定数/関数はない");
    t_ent = get_table(t_ptr);

    if (t_ent.type == func_id) { /* T_IDが関数名の場合 */
      nextToken = getToken();
      if (nextToken != T_LPAR) {
        pl0_error("", line_no, "( がない");
      } else {
	nextToken = getToken();
	parse_FuncArgList();
	if (nextToken != T_RPAR) pl0_error("", line_no, ") がない");
      }
    } else { /* T_IDが関数名以外の場合 */
    }
    nextToken = getToken();
  } else if (nextToken == T_NUMBER) { 
    /* ここで数字の処理 */
    nextToken = getToken();
  } else if (nextToken == T_LPAR) {
    nextToken = getToken();
    parse_Expression();
    if (nextToken != T_RPAR) pl0_error(yytext, line_no, ")がない");
    nextToken = getToken();
  } else {
    pl0_error(yytext, line_no, "式がおかしい");
  }
  printf("Exit  Factor\n");
}

void parse_FuncArgList() {
  printf("Enter FuncArgList\n");
  if (nextToken == T_PLUS || nextToken == T_MINUS
      || nextToken == T_ID || nextToken == T_NUMBER
      || nextToken == T_LPAR) { /* First(<Expression>)に含まれるもの */
    parse_Expression();
    parse_FuncArgList_dash();
  }
  printf("Exit  FuncArgList\n");
}

void parse_FuncArgList_dash() {
  printf("Enter FuncArgList_dash\n");
  if (nextToken == T_COMMA) {
    nextToken = getToken();
    parse_Expression();
    parse_FuncArgList_dash();
  }
  printf("Exit  FuncArgList_dash\n");
}
