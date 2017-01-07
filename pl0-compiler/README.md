### 構文解析器/コンパイラの作成方法
以下のようにして構文解析器/コンパイラを作成できます。
````console
$ cd pl0-compiler
$ make
いろいろ画面に出力される
$
````

#### 1. PL/0'用構文解析器
* 第20回授業(後期第05回) p. 257のPL/0'構文解析器のプログラム No.1
  * [字句解析用サブルーチン(lex版)](pl0-scanner.l)
  * [トークン定義ファイル](../scanner/tokentable.h)
  * [LL(1)構文解析器本体 No.1](pl0-ll-parser01.c)
  * [エラー処理関数など](misc.c)
  * [エラー処理関数などのヘッダーファイル](misc.h)
  * [PL/0'プログラム00(関数あり)](../sample/test00.pl0)
  * [PL/0'プログラム01(関数なし)](../sample/test01.pl0)

* 第24回授業(後期第09回)までの説明を実装したPL/0'用構文解析器 No.2(pl0-compilerディレクトリ下)
  * [LL(1)構文解析器本体 No.2](pl0-ll-parser02.c)
    * 内部に簡易な記号表を含んでいる

* 第25回授業(後期第10回)の説明を実装した完全な PL/0'用構文解析器 No.3(pl-compilerディレクトリ下)
  * [LL(1)構文解析器本体 No.3](pl0-ll-parser03.c)
  * [完全な記号表 No.1 プログラム](symbol_table01.c)
  * [完全な記号表 No.1 ヘッダーファイル](symbol_table01.h)

* 実行方法と実行結果(pl0-ll-parser01)
````console
$ ./pl0-ll-parser01 ../sample/test00.pl0
Enter Program
Enter Block
Enter ConstDeclList
....
Enter StatementList_dash
Exit  StatementList_dash
Exit  StatementList
エラー: 12 行目 '('付近: endがない
$
$ ./pl0-ll-parser01 ../sample/test01.pl0
Enter Program
Enter Block
Enter ConstDeclList
Exit  ConstDeclList
...
Exit  StatementList_dash
Exit  StatementList_dash
Exit  StatementList
Exit  Statement
Exit  Block
Exit  Program
構文解析は全て成功
$
````

#### 2. PL/0'用コンパイラ

* 第26回授業(後期第11回)までの説明を実装した PL/0' 用コンパイラ(関数非対応)
  * [LL(1)コンパイラ本体 No.1](pl0-ll-compiler01.c)
  * [記号表 No.2 プログラム](symbol_table02.c)
  * [記号表 No.2 ヘッダーファイル](symbol_table02.h)
  * [コード生成プログラム](codegen.c)
  * [コード生成プログラム ヘッダーファイル](codegen.h)
  * [エラー処理関数など](misc.c)
  * [エラー処理関数など ヘッダーファイル](misc.h)

* 第27回授業(後期第12回)までの説明を実装した PL/0' 用コンパイラ(関数対応)
  * [LL(1)コンパイラ本体 No.2](pl0-ll-compiler02.c)
  * [記号表 No.2 プログラム](symbol_table02.c)
  * [記号表 No.2 ヘッダーファイル](symbol_table02.h)
  * [コード生成プログラム](codegen.c)
  * [コード生成プログラム ヘッダーファイル](codegen.h)
  * [エラー処理関数など](misc.c)
  * [エラー処理関数など ヘッダーファイル](misc.h)

* コマンドオプションなど
  * コンパイル(コンパイル結果は標準出力)
  ```console
  $ ./pl0-ll-compiler02 test16.pl0
  JMP 64
  PUSH FP
  LOAD FP,SP
  PUSHUP
  LOAD A,#(FP+3)
  ...
  $
  ```

  * コンパイル(コンパイル結果は標準出力。番地がつく)
  ```console
  $ ./pl0-ll-compiler02 -n test16.pl0
     1 JMP 64
     2 PUSH FP
     3 LOAD FP,SP
     4 PUSHUP
     5 LOAD A,#(FP+3)
  ...
  $
  ```

  * コンパイル(コンパイル結果を test16.out に保存)
  ```console
  $ ../pl0-compiler/pl0-ll-compiler02 -o test16.out test16.pl0
  $
  ```

  * 使用法表示
   ```console
   $ ./pl0-ll-compiler02 -h
   ./pl0-ll-compiler02 [-h] [-n] [-o output_file] source_file
   $
   ```