#include <string.h>
#include "misc.h"
#include "symbol_table01.h"

static struct table_entry symbol_table[MAX_TABLE_SIZE]; /* 記号表 */
static int symbol_table_ptr = 0 ; /* 記号表の現在位置を示すポインタ */

int search_table(char *id_name) { /* 記号表の検索 */
  int i = symbol_table_ptr;
  while(strcmp(id_name, symbol_table[i].id_name) && i > 0) { 
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

int reg_const_in_tbl(char *id_name, int line_no, int t_num_value) {
  int t_ptr ;
  t_ptr = add_table(const_id, id_name, line_no);
  return t_ptr;
}

int reg_var_in_tbl(char *id_name, int line_no) {
  int t_ptr ;
  t_ptr = add_table(var_id, id_name, line_no);
  return t_ptr;
}

int reg_func_in_tbl(char *id_name, int line_no) {
  int t_ptr ;
  t_ptr = add_table(func_id, id_name, line_no);
  symbol_table[t_ptr].data.func.n_params = 0;
  return t_ptr;
}

int reg_param_in_tbl(char *id_name, int line_no, int func_ptr) {
  int t_ptr ;
  t_ptr = add_table(param_id, id_name, line_no);
  symbol_table[func_ptr].data.func.n_params++;
  return t_ptr;
}
