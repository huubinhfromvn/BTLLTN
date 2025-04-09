#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include "postfix.h"
#include "findroot.h"

float best_result = 0.0;

void findrootNewton(Token *postfix)
{
    best_result = newtonRaphson(postfix);
    printf("Newton Raphson tim duoc nghiem la: %f\n", best_result);
}

void findrootSecant(Token *postfix, float a, float b)
{
    best_result = secantMethod(postfix, a, b);
    printf("Secant tim duoc nghiem la: %f\n", best_result);
}
int main()
{
    struct timespec start, end;
    Token *output;
    char str[MAX];
    int method_choice;
    printf("Nhap bieu thuc: ");
    fgets(str, MAX, stdin);
    str[strcspn(str, "\n")] = 0;
    output = infixToPostfix(str);
    if (output != NULL)
    {
        printTokens(output);

        // Chọn phương pháp tìm nghiệm
        printf("Chon phuong phap tim nghiem:\n");
        printf("1. Newton-Raphson\n");
        printf("2. Secant\n");
        printf("Chon (1 hoac 2): ");
        scanf("%d", &method_choice);

        clock_gettime(CLOCK_MONOTONIC, &start);

        // Tùy chọn phương pháp tìm nghiệm
        switch (method_choice)
        {
        case 1:
            findrootNewton(output);
            break;
        case 2:
            float a, b;
            printf("Nhap hai gia tri a va b: ");
            scanf("%f %f", &a, &b);
            findrootSecant(output, a, b);
            break;
        default:
            printf("Lua chon khong hop le.\n");
            return 1;
        }

        clock_gettime(CLOCK_MONOTONIC, &end);
        double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        printf("Thoi gian tim nghiem: %f giay\n\n", elapsed);

        printf("Ket qua %.2f la: %.3f\n", best_result, fabs(evaluatePostfix(output, best_result)));
    }
    return 0;
}
