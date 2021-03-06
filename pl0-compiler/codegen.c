/* codegen.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"
#include "symbol_table02.h"

#define MAX_OP_LINE  20 /* アセンブリコード一行の最大長 */
#define MAX_CODE    800 /* アセンブリコードの最大行数   */

int code_ptr = 0;

typedef struct {
  char op_line[MAX_OP_LINE];
  int address; /* JMP/JPC 用 */
} Asm_Code;

static Asm_Code code[MAX_CODE];

/* コード生成関数 */
int gencode_no_arg(Opr o); /* 引数なし */
int gencode_arg_ST(Opr o, int ptr); /* 引数として記号表番号 */
int gencode_arg_V(Opr o, int value); /* 引数として値そのもの */
int gencode_arg_V_ST(Opr o, int value, int ptr); /* 引数として値と記号表番号 */

int next_code(); /* 次の命令が入る番地を求める */
void list_code(FILE *fp, int n_flag); /* コードの出力 */
void backpatch(int code_lineno); /* バックパッチ用 */
  
/* 以下は本ファイルでのみ使用 */
int add_code(char *opline);
int add_code_val(char *fmt, int value);
int add_code_sharpval(char *fmt, int value);
int add_code_addr(char *opr, int address);

void list_code(FILE *fp, int n_flag){ /* リストを出力 */
  int i = 1;
  while (i <= code_ptr) {
    if (code[i].address < 0) 
      if (!n_flag)
	fprintf(fp,"%s\n", code[i].op_line);
      else
	fprintf(fp,"%4d %s\n", i, code[i].op_line);
    else
      if (!n_flag)
	fprintf(fp,"%s%d\n", code[i].op_line, code[i].address);
      else
	fprintf(fp,"%4d %s%d\n", i, code[i].op_line, code[i].address);
    i++;
  }
}

int gencode_no_arg(Opr o) {
  int cpt;
  if (o == wrl) {
    return add_code("PRINTLN");
  }

  if (o == wrt) {
    cpt = add_code("POP A");
    cpt = add_code("PRINT A");
    return cpt;
  }

  if (o == end) {
    return add_code("END");
  }

  if (o == pushup) {
    return add_code("PUSHUP");
  }

  if (o == enterf) {
    cpt = add_code("PUSH FP");
    cpt = add_code("LOAD FP,SP");
    return cpt;
  }

  if (o == leavef) {
    cpt = add_code("POP C");
    cpt = add_code("LOAD SP,FP");
    cpt = add_code("POP FP");
    return cpt;
  }

  if (o == odd) {
    add_code("POP A");
  } else { /* スタックから2つPOPするもの */
    add_code("POP B");
    add_code("POP A");
  }
  switch (o) {
  case pls:
    add_code("PLUS");
    break;
  case min:
    add_code("MINUS");
    break;
  case mul:
    add_code("MULTI");
    break;
  case divi:
    add_code("DIV");
    break;
  case odd:
    add_code("CMPODD");
    break;
  case eq:
    add_code("CMPEQ");
    break;
  case neq:
    add_code("CMPNOTEQ");
    break;
  case lt:
    add_code("CMPLT");
    break;
  case gt:
    add_code("CMPGT");
    break;
  case le:
    add_code("CMPLE");
    break;
  case ge:
    add_code("CMPGE");
    break;
  default:
    break;
  }
  return add_code("PUSH C");
}

int gencode_arg_V_ST(Opr o, int value, int ptr) {
  int cpt = 0;
  switch(o) {
  case str:
    cpt = add_code_val("LOAD A,%d", value);
    cpt = add_code_sharpval("STORE A,#(%s)", get_symbol_address(ptr));
    break;
  default:
    break;
  }
  return cpt;
}

int gencode_arg_V(Opr o, int value) {
  int cpt = 0;
  switch(o) {
  case lod:
    cpt = add_code_val("LOAD A,%d", value);
    cpt = add_code("PUSH A");
    break;
  case jmp:
    cpt = add_code_addr("JMP ", value);
    break;
  case jpc:
    cpt = add_code("POP C");
    cpt = add_code_addr("JPC ", value);
    break;
  case call:
    cpt = add_code_addr("CALL ", value);
    cpt = add_code("PUSH C");
    break;
  case ret:
    cpt = add_code_addr("RET ", value);
    break;
  default:
    break;
  }
  return cpt;
}


int gencode_arg_ST(Opr o, int ptr) {
  int cpt = 0;
  switch(o) {
  case str:
    cpt = add_code("POP A");
    cpt = add_code_sharpval("STORE A,#(%s)", get_symbol_address(ptr));
    break;
  case lod:
    cpt = add_code_sharpval("LOAD A,#(%s)", get_symbol_address(ptr));
    cpt = add_code("PUSH A");
    break;
  default:
    break;
  }
  return cpt;
}

int add_code(char *opline) {
  code_ptr++;
  strcpy(code[code_ptr].op_line, opline);
  code[code_ptr].address = -1; /* addressは使用しない */
  return code_ptr;
}

int add_code_val(char *fmt, int value) {
  code_ptr++;
  sprintf(code[code_ptr].op_line, fmt, value);
  code[code_ptr].address = -1; /* addressは使用しない */
  return code_ptr;
}

int add_code_sharpval(char *fmt, int value) {
  char str_val[15];
  code_ptr++;

  /* value はヒープアドレスかFPからの相対位置のはず */
  if (value < 800) {
    sprintf(str_val, "FP%+d", value);
  } else {
    sprintf(str_val, "%d", value);
  }
  sprintf(code[code_ptr].op_line, fmt, str_val);
  code[code_ptr].address = -1; /* addressは使用しない */
  return code_ptr;
}

int add_code_addr(char *opr, int address) {
  code_ptr++;
  strcpy(code[code_ptr].op_line, opr);
  code[code_ptr].address = address; 
  return code_ptr;
}

void backpatch(int code_lineno) { /* 現在のコード行の次の行を入れる */
  code[code_lineno].address = code_ptr+1;
}

int next_code() {
  return code_ptr+1;
}
