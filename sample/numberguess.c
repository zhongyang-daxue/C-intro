// numberguess.c

#include <stdio.h>

// search number in range of [lower, upper).
int guess(const int lower, const int upper) {
    if(upper - lower <= 1) return lower;
    const int m = (upper + lower) / 2;
    char answer;                // ユーザーの回答
    printf_s(u8"is the number greater than or equal to %d? y/n\n", m);
    scanf_s("%c", &answer);

    // 改行文字の読み捨て
    while (getchar() != '\n');

    return answer == 'y'        // 答えがイエスなら
        ? guess(m, upper)       // [m + 1, upper)でもう一度
        : guess(lower, m);      // 答えがノーなら[lower, m)でもう一度
}

int main(void) {
    puts(u8"choose a number between 0 and 99.");
    printf_s(u8"the answer is %d!\n", guess(0, 100));
}