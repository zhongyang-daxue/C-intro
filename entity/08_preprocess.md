# プリプロセス

Cのプリプロセッサとはコンパイルをする前にソースコードに対して行うべき前処理を行ってくれるプログラムだ。プリプロセッサが行う処理のことをプリプロセスという。Cでは`#`から始まる命令文がプリプロセッサ指令と解釈される。何度か出てきた`#include`もプリプロセッサ指令の一つだ。

プリプロセッサ指令は`#`で始まり、改行で終わる。プリプロセッサ指令を終わらせたくないが改行したい場合は改行文字の前にバックスラッシュ`\`を書くと次の行も同じプリプロセッサ指令として解釈される。

## #include

この先最も見るプリプロセッサ指令だ。教えていないのに何度か勝手に使ってしまったが、これがないと使えないものが多すぎるので許してほしい。この指令は書かれた位置にファイルを挿入するのと同じ動作をする。

例えば以下のような`sample.h`と`sample.c`があるとする。

```c
// sample.h
int f();
```

```c
// sample.c
#include "sample.h"

int f() {return 0;}
```

sample.cのプリプロセスが処理されれば以下のような翻訳単位になる。

```c
int f();

int f() {return 0;}
```

`#include`指令は直後に`<`, `>`もしくは`"`, `"`で囲んでヘッダファイル名を書く。`<>`で囲んだ場合はコンパイラが知っているインクルードディレクトリからヘッダファイルを検索する。二重引用符で囲んだ場合はプリプロセッサ指令が書かれたソースファイルのディレクトリからヘッダファイルを検索する。尚、相対パスでヘッダファイルを指定する。

例えば以下のようなファイル構成だとする。
```
current_dir
    ├─source.c
    └─include
        └─target_header.h
```

この場合source.cからtarget_header.hを`#include`するには以下のようにする。

```c
#include "include/target_header.h"
```

## #define

`#define`は直後に指定された文字列をそのあとに続くソースコードに置き換える命令だ。例えば`#define SAMPLE`なんて書けば`SAMPLE`というトークンはすべて虚無に置き換わる。これはプログラムの意味などは全く関係なしにソースコード上の文字列を置換するように動作する。そのため、`#define`で置き換えたい文字列は予期しないものと置き換わることを防ぐため、一般にすべて大文字で定義する。~~Windws.hを許すな。~~

### オブジェクト形式マクロ

例で出したように文字列を置き換える。どんな文字列でもそのまま置き換えてしまうため非常に危険だ。例えば以下のように使うことができる。

```c
#include <stdio.h>

#define MY_NAME "ouchiminh"

int main(void) {
    puts(MY_NAME);
}

```

### 関数形式マクロ

関数形式マクロであろうがオブジェクト形式マクロであろうがそのまま置き換えられるため非常に危険だ。関数形式マクロは引数を取ることができる。ここで悪名高い`min`マクロと`max`マクロを見てみよう。

```c
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))
```

小文字のマクロは定義するだけで嫌われ者になれる魔法だ。

関数形式マクロには注意すべき点がある。例えば次のソースコードを見てほしい。

```c
#define PLUS(a, b)          a + b

int main(void) {
    int answer = PLUS(1, 4) * 2;
}
```

`answer`に格納される数値は`10`になると思いきや、`9`だ。

>これはプログラムの意味などは全く関係なしにソースコード上の文字列を置換するように動作する。

すなわち先ほどのソースコードはこのようになる。

```c
#define PLUS(a, b)          a + b

int main(void) {
    int answer = 1 + 4 * 2;
}
```

`min`マクロと`max`マクロが括弧だらけなのはこういう理由だ。それぞれの引数とマクロ全体を括弧で囲めば問題は無くなる。

## 条件付きコンパイル

### #if

直後に指定された値が0でなければ`#elif`, `#else`, `#endif`のいずれかまでに続くソースコードがコンパイルされる。0であればそのソースコードはコンパイルされない。なお値はコンパイル前に定まっている必要がある。

### #elif

直前の`#if`, `#elif`の結果が0だった場合に評価される。動作は`#if`とおなじ。

### #else

直前の`#if`, `#elif`の結果が0だった場合、そこから`#endif`までのソースコードがコンパイルされる。

### #endif

`#if`から始まる一連の条件分岐の終了地点を示す。

### #ifdef

`#if defined`の短縮形。続く単語がマクロで定義されている場合条件は真となる。

### #ifndef

`#if !defined`の短縮形。続く単語がマクロで定義されていない場合条件は真となる。

----

例

```c
#define COMPILE_FLAG 1

#if COMPILE_FLAG
int main(void) { puts("1!"); }
#elif COMPILE_FLAG == 2
int main(void) { puts("2!"); }
#endif

```

## #pragma

プラグマ命令はコンパイラ独自の命令だ。そのためここでは扱わない。詳しくは使っているコンパイラの制作者に尋ねてほしい。

だが例外的に`#pragma once`だけは大手コンパイラのほとんどにサポートされているので解説する。`#pragma once`命令はヘッダファイルの多重インクルードを防止するプリプロセッサ命令だ。
