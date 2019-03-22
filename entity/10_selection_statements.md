# 選択文

選択文とは制御式の値に応じていくつかの文の中から一つを選択するための構文である。

## if文

```
if (式) 文
if (式) 文 else 文
```

`if`文の制御式は、スカラ型をもたなければならない。式の値が0と比較して等しくない場合、最初の副文を実行する。`else`が後に続く場合、直前の`if`の式の値が0と等しい場合、2番目の副文を実行する。`else`は、その`else`の前で最も近い位置にある`if`と結びつく。

例

```c
int a;
scanf("%d", &a);
if(!a) {
    puts("a is 0");
} else {
    if(a == 1){
        puts("a is 1");
    } else puts("a is not 0");
}
```

余談だが、選択文はブロックであるため、副文が一つの文しか含まない場合波括弧`{}`を省略できる。すると先のコードは以下のように書きかえることができる。

```c
int a;
scanf("%d", &a);
if(!a) puts("a is 0");
else
    if(a == 1) puts("a is 1");
    else puts("a is not 0");    // このelseは1行上のifと結びついている。
```

実際には`else if`という構文は無いが、インデントを調整すればあたかもRubyなどのelifと同じように記述することができる。

```c
int a;
scanf("%d", &a);
if(!a) puts("a is 0");
else if(a == 1) puts("a is 1");
else puts("a is not 0");    // このelseは1行上のifと結びついている。
```

## switch文

## オマケ:三項条件演算子