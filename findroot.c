/* findroot.c */
#include "findroot.h"
#include <stdio.h>
#include <math.h>

// Hàm tính đạo hàm xấp xỉ
float derivative(Token *postfix, float x) {
    float h = 1e-4;
    return (evaluatePostfix(postfix, x + h) - evaluatePostfix(postfix, x)) / h;
}

// Phương pháp Newton-Raphson
float newtonRaphson(Token *postfix) {
    float x = 1.0; // Giá trị khởi tạo
    int d=0;
    for (int i = 0; i < MAX_ITER; i++) {
        float fx = evaluatePostfix(postfix, x);
        float dfx = derivative(postfix, x);
        if (fabs(dfx) < EPSILON) {
            printf("Dao ham gan bang 0, khong the tiep tuc\n");
            printf("Nghiem la: %f\n", x);
            printf("So lan lap: %d\n", i+1);
            return x;
        }
        float x_new = x - fx / dfx;
        // printf("Iteration %d: x = %f, x_new = %f, fabs(x_new - x) = %e\n", i + 1, x, x_new, fabs(x_new - x));
        if (fabs(x_new - x) < 1e-6){
            printf("fabs(x_new - x) la: %f\n", fabs(x_new - x));
            printf("Khoang cach giua x va xnew be hon 1e-6\n");
            printf("So lan lap: %d\n", i+1);
            return x_new;
        } 
        x = x_new;
        d++;
    }
    x=-1;
    for (int i = 0; i < MAX_ITER; i++) {
        float fx = evaluatePostfix(postfix, x);
        float dfx = derivative(postfix, x);
        if (fabs(dfx) < EPSILON) {
            printf("Dao ham gan bang 0, khong the tiep tuc\n");
            printf("Nghiem la: %f\n", x);
            printf("So lan lap: %d\n", i+1);
            return x;
        }
        float x_new = x - fx / dfx;
        // printf("Iteration %d: x = %f, x_new = %f, fabs(x_new - x) = %e\n", i + 1, x, x_new, fabs(x_new - x));
        if (fabs(x_new - x) < 1e-6){
            printf("fabs(x_new - x) la: %f\n", fabs(x_new - x));
            printf("Khoang cach giua x va xnew be hon 1e-6\n");
            printf("So lan lap: %d\n", i+1);
            return x_new;
        } 
        x = x_new;
        d++;
    }
    printf("So lan lap: %d\n", d);
    return x;
}

// Phương pháp Secant
float secantMethod(Token *postfix, float a, float b) {
    float x0 = a, x1 = b;
    for (int i = 0; i < MAX_ITER; i++) {
        float f0 = evaluatePostfix(postfix, x0);
        float f1 = evaluatePostfix(postfix, x1);
        if (fabs(f1 - f0) < EPSILON){
            printf("Khoang cach giua f1 va f0 be hon 1e-4\n");
            printf("So lan lap: %d\n", i+1);
            return x1;
        }
        float x_new = x1 - f1 * (x1 - x0) / (f1 - f0);
        printf("Lan Lap %d: x0 = %f, x1 = %f, x_new = %f\n", i + 1, x0, x1, x_new);
        if (fabs(x_new - x1) < EPSILON){
            printf("Khoang cach giua x va xnew be hon 1e-4\n");
            return x_new;
        } 
        x0 = x1;
        x1 = x_new;
    }
    return x1;
}
