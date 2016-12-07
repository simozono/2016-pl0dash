## 「言語処理系」の授業で使用したプログラムコード
ここでは、2016年度に「言語処理系」(鹿児島高専/情報工学科/3年生向け)の授業の
際に使用したプログラムコードを提示しています。(自分の忘備録を兼ねています)

### コンパイル対象言語
* PL/0'
* 簡単な文法G4

### プログラムの説明

#### 1. [字句解析器(scannerディレクトリ下)](scanner)
  * [「PL/0'」用字句解析器(lex版)](scanner/pl0-scanner.l)

#### 2. [簡単な文法G4(exp-parserディレクトリ下)](exp-parser)

第20回授業(後期第05回)p.247 で示した文法に対応する構文解析器等です。

* 第20回授業(後期第05回) p. 250～の文法G4構文解析器のプログラム
  * [字句解析用サブルーチン(lex版)](exp-parser/exp-scanner.l)
  * [トークン定義ファイル](scanner/tokentable.h)
  * [LL(1)構文解析器本体](exp-parser/exp-ll-parser01.c)
  * [サンプルファイル](sample/exp01.txt)

#### 3. PL/0'用構文解析器(pl0-compilerディレクトリ下)
* 第20回授業(後期第05回) p. 257のPL/0'構文解析器のプログラム No.1
  * [字句解析用サブルーチン(lex版)](pl0-compiler/pl0-scanner.l)
  * [トークン定義ファイル](scanner/tokentable.h)
  * [LL(1)構文解析器本体 No.1](pl0-compiler/pl0-ll-parser01.c)
  * [エラー処理関数など](pl0-compiler/misc.c)
  * [エラー処理関数などのヘッダーファイル](pl0-compiler/misc.h)
  * [PL/0' ソースファイル00(関数あり)](sample/test00.pl0)
  * [PL/0' ソースファイル01(関数なし)](sample/test01.pl0)

* 第24回授業(後期第09回)までの説明を実装したPL/0'用構文解析器 No.2(pl0-compilerディレクトリ下)
  * [LL(1)構文解析器本体 No.2](pl0-compiler/pl0-ll-parser02.c)
    * 内部に簡易な記号表を含んでいる

* 第25回授業(後期第10回)の説明を実装した完全な PL/0'用構文解析器 No.3(pl-compilerディレクトリ下)
  * [LL(1)構文解析器本体 No.3](pl0-compiler/pl0-ll-parser03.c)
  * [完全な記号表 No.1 プログラム](pl0-compiler/symbol_table01.c)
  * [完全な記号表 No.1 ヘッダーファイル](pl0-compiler/symbol_table01.h)

### 利用方法

````console
$ git clone https://github.com/simozono/2016-pl0dash
$ cd 2016-pl0dash
$ ls
README.md  exp-parser/  pl0-compiler/  sample/  scanner/
$ 
````

