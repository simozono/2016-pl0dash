### 構文解析器の作成方法
以下のようにして構文解析器を作成できます。
````console
$ cd pl0-compiler
$ make
いろいろ画面に出力される
$
````

#### 1. PL/0'用構文解析器 No.1
* 第20回授業(後期第05回) p. 257のPL/0'構文解析器のプログラム
  * [字句解析用サブルーチン(lex版)](pl0-scanner.l)
  * [トークン定義ファイル](../scanner/tokentable.h)
  * [LL(1)構文解析器本体](pl0-ll-parser01.c)
  * [エラー処理関数など](misc.c)
  * [エラー処理関数などのヘッダーファイル](misc.h)
  * [PL/0'プログラム00(関数あり)](../sample/test00.pl0)
  * [PL/0'プログラム01(関数なし)](../sample/test01.pl0)
* 実行方法と実行結果
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
$
````