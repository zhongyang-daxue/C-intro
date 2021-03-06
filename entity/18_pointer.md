# アドレス

[変数のページ](06_variable.md)でも話したように変数はメモリに置かれる (メモリに置かれないこともあるが厄介なのでここでは考えない)。ポインタはメモリのどこに置かれたのかを示す値を格納する変数だ。その値のことをアドレスという。日本語にすると住所だ。まさしくオブジェクトの居場所を指している的確な用語だ。変数のアドレスを取るには`&`演算子を用いる。書式付き出力関数でポインタの変換指定子は`%p`だ。試しに適当な変数のアドレスを出力してみよう。

```c
#include <stdio.h>

int main(void) {
    int a;
    printf_s("%p\n", &a);
}
```

出力された場所に変数`a`がある。

# ポインタ

ポインタは格納されているアドレスの位置にあるオブジェクトを参照することができる。ポインタ型は型名に`*`を付けて表現する。ポインタの定義は以下のようになる。

```c
型名* 変数名;
```

また、ポインタを通してそのオブジェクトにアクセスすることができる。そのことを参照はがし (*dereference*) といい、ポインタに`*`を付けて記述する。当然だが、ポインタが指しているオブジェクトを書き換えると、ポインタが指しているオブジェクトが書き換わる。

```c
#include <stdio.h>

int main(void) {
    int a = 1;
    int* p = &a;            // pはintへのポインタ aのアドレスとpに代入

    printf_s("%d\n", *p);   // pが指しているオブジェクトを出力

    *p = 2;                 // pの参照先を2に書き換えると
    printf_s("%d\n", a);    // aの値も2になる
}
// output
// 1
// 2

```

ポインタは整数型にキャストできる。ポインタを整数にキャストする際、ポインタの値の範囲が変換先の整数型が表すことができる値の範囲より大きい場合、未定義動作となる。ポインタを整数にキャストする際には`stdint.h`で宣言されている`intptr_t`もしくは`uintptr_t`を使用すれば安全に動作する。`intptr_t`と`uintptr_t`はどんな有効なポインタからも変換可能だ。そして、ポインタに変換しなおせばその結果は変換前と等しい。

```c
#include <stdint.h>
#include <assert.h>
int main(void) {
    int a;
    uintptr_t integer;
    int* ptr;
    integer = (uintptr_t)&a;
    ptr = &a;
    assert((int*)integer == ptr);
}
```

ポインタは関数の引数にして、計算結果を書き込んでもらうことも出来る。その場合関数の戻り値はエラーを報告するためなど、ほかの目的に使用することができる。オブジェクトのアドレスを渡すことを参照渡しという (逆にオブジェクトをそのまま渡すことを値渡しといい、オブジェクトはコピーされて渡される)。

```c
// sampleディレクトリのoverflow_check.cに同等のプログラムがある。
#include <limits.h>
// a+bを計算して桁あふれを検出する。
// 桁あふれが起こった場合-1を返す。
int add(unsigned int* result_buffer, unsigned int a, unsigned int b) {
    return b <= UINT_MAX - a
        ? *result_buffer = a + b, 0
        : -1;
}
```

また、ポインタはコピーするのが億劫な巨大なオブジェクトを関数に渡すための手段としても用いることができる。組み込み型ではそのような型は見かけないが、構造体のサイズを大きくするのは非常に簡単だ。関数に渡されるオブジェクトは全てコピーして渡されるため、構造体をコピーするより構造体のアドレスをコピーして渡すほうがスタックの消費量や処理にかかる時間を押さえることができる場合が多い。

## voidポインタ

`void`へのポインタは如何なる型のポインタからも、もしくは如何なる型のポインタへも変換可能なポインタだ。また`void`へのポインタは、文字型へのポインタと同じ表現及び同じ境界調整要求を持たなければならない。

## constポインタ

ポインタが指すオブジェクトが書き換えられないことを保証したいときはポインタに`const`を付ける。今更だが`const`は何もポインタに対してだけ用いるものではない。他の型に対しても書き換えたくない場合は`const`を指定して読み取り専用の変数のように扱うことができる。

ただしポインタは`const`を付ける位置によって読み取り専用になるものが変わるので注意が必要だ。

```c
int a = 0;
int b = 1;
int* p0 = &a;
const int* p1 = &a;         // int const*でもよい ポインタの指すオブジェクトがconst
int* const p2 = &a;         // ポインタ自体がconst (参照する場所を変更できない)
const int* const p3 = &a;   // int const * constでもよい ポインタ自体とポインタの指すオブジェクトがconst

p0 = &b;                    // 合法
p1 = &b;                    // 合法
p2 = &b;                    // error! p2自体は読み取り専用
p3 = &b;                    // error! p3自体は読み取り専用

*p0 = 2;                    // 合法
*p1 = 2;                    // error! p1が指す先は読み取り専用
*p2 = 2;                    // 合法
*p3 = 2;                    // error! p3が指す先は読み取り専用

p0 = p1;                    // error! p1のconst属性をはがすことはできない
p0 = p2;                    // 合法
p0 = p3;                    // error! same as above
p1 = p0;                    // 合法
p1 = p2;                    // 合法
p1 = p3;                    // 合法
p2 = p0;                    // error!
p2 = p1;                    // error!
p2 = p3;                    // error!
p3 = p0;                    // error!
p3 = p1;                    // error!
p3 = p2;                    // error!
```

## ヌルポインタ

ポインタにはヌルポインタという無効値を持たせることができる。ヌルポインタとは`0`の値を持つ整数定数式、またはそのような式を`void*`にキャストした式だ。その式はマクロで`NULL`として定義されている。

```c
#define NULL ((void*)0)
// もしくは
#define NULL 0
```

ヌルポインタのような無効なポインタに対して参照はがしすると未定義動作となる。

## ダングリングポインタ

最も厄介なものにダングリングポインタというものがある。その名の通りポインタの指している場所が不正なメモリ領域になってしまい、宙ぶらりんになっているポインタのことだ。では早速不正なポインタを作ろう。その扱いの厄介さと比べて作るのは非常に容易であるため、バグが発生しやすい。C++という言語ではC以上にダングリングポインタを発生させやすくするラムダ式の参照キャプチャ機能があるが、ここではその話をすることはない。

```c
int* f() {
    int a = 0;
    return &a;
}   // aの寿命は尽きた。

int main(void) {
    int* dangling_pointer = f();
    printf_s("%d\n", *dangling_pointer);    // 未定義動作
}
```

[変数の項目](06_variable.md)で話したように、変数には寿命がある。その寿命が尽きた変数があった場所には何があるか分からない。関数`f`の中の変数`a`は`return`文の後その寿命を迎える。`dangling_pointer`にアクセスすることはとてもいけないことだ。

# エイリアシング

複数の変数がメモリ上の同じ位置を参照していることをエイリアシングという。人間は愚かであるがゆえに、エイリアスになるべきでない変数をエイリアスにしてしまうことがある。

```c
long a;
void foo(double* p) {
    a = 0;
    *p = 3.14;
    bar(a);
}

foo((double*)&a);
```

上記のコードはとても正気の沙汰ではないが、浮動小数点数の実装を見たいといった場合にはもしかしたらこんなおかしなコードを書くことがあるかもしれない。

問題なのはオプティマイザが`bar(a)`を`bar(0)`というように最適化してしまいかねないということだ。なぜなら、`int`と`double`は互換型ではないから、`*p`は`a`のエイリアスになり得ないためだ。事前に`a`には`0`が代入されているために、`a`のエイリアスになり得ない`*p`に何が代入されようと`bar()`の実引数は`0`以外にならないとオプティマイザは考えている。

合法的にエイリアスになれる二つのオブジェクトは以下のいずれかの条件を満たすものだ。

- 互換型か、`signed`、`unsigned`、`const`、`volatile`の組み合わせのみが異なる型同士
- `struct`または`union`とそれが内部に含む型同士
- 文字型 (例外的に`char*`、`signed char*`と`unsigned char*`だけはメモリ上のすべてのエイリアスになり得る)
  
以上の条件を満たさないエイリアスを作ることは発見しにくいバグを生むだろう。
