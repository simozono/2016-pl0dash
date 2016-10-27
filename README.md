## 「言語処理系」の授業で使用したプログラムコード
ここでは、2016年度に「言語処理系」(鹿児島高専/情報工学科/3年生向け)の授業の
際に使用したプログラムコードを提示しています。(自分の忘備録を兼ねています)

### コンパイル対象言語
*  PL/0'
* 簡単な文法G4

### プログラムの説明

#### 1. [字句解析器](scanner)
  * [「PL/0'」用字句解析器(lex版)](scanner/pl0-scanner.l)

#### 2. [簡単な文法G4](exp-parser)

第20回授業(後期第05回)p.247 で示した文法に対応する構文解析器等です。

* 第20回授業(後期第05回) p. 250～の文法G4構文解析器のプログラム
  * [字句解析用サブルーチン(lex版)](exp-parser/exp-scanner.l)
  * [トークン定義ファイル](scanner/tokentable.h)
  * [LL(1)構文解析器本体](exp-parser/exp-ll-parser01.c)
  * [サンプルファイル](sample/exp01.txt)
  
#### 3. [PL/0'用構文解析器 No.1]
* 第20回授業(後期第05回) p. 257のPL/0'構文解析器のプログラム
  * [字句解析用サブルーチン(lex版)](pl0-compiler/pl0-scanner.l)
  * [トークン定義ファイル](scanner/tokentable.h)
  * [LL(1)構文解析器本体](pl0-compiler/pl0-ll-parser01.c)
  * [エラー処理関数など](pl0-compiler/misc.c)
  * [エラー処理関数などのヘッダーファイル](pl0-compiler/misc.h)
  * [PL/0' ソースファイル00(関数あり)](sample/test00.pl0)
  * [PL/0' ソースファイル01(関数なし)](sample/test01.pl0)

