#include "postfix.h"
#include <string.h>  // cần cho hàm strncmp

int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

int precedence(char op) {
    switch(op) {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
        case '^': return 3;
        default: return 0;
    }
}

void printTokens(Token *output) {
    int i = 0;
    printf("Output Tokens: ");
    while (output[i].type != OPERATOR || output[i].value.operator != 'E') {
        if (output[i].type == OPERAND) {
            printf("%.2f ", output[i].value.operand);
        } else if (output[i].type == OPERATOR) {
            printf("%c ", output[i].value.operator);
        } else if (output[i].type == VARIABLE) {
            printf("x ");
        }
        i++;
    }
    printf("\n");
}
Token *infixToPostfix(char* myFunction){
    state_t current_state = S_START;
    Token *output = (Token *)malloc(MAX * sizeof(Token));
    int outputIndex = 0;
    char stack[MAX];
    int stackTop = -1;

    while (1){
        switch (current_state){
            case S_START:
                if (isdigit(*myFunction) || *myFunction == '.' || *myFunction == 'x') {
                    current_state = S_OPERAND;
                }
                else if (strncmp(myFunction, "sin", 3) == 0) {
                    // Nhận diện hàm sin, đẩy token 's' vào ngăn xếp
                    stack[++stackTop] = 's';
                    myFunction += 3; // bỏ qua "sin"
                    // Sau "sin" cần có dấu mở ngoặc
                    if (*myFunction == '(') {
                        current_state = S_OPEN;
                    } else {
                        current_state = S_ERROR;
                    }
                }
                else if (*myFunction == '(') {
                    current_state = S_OPEN;
                }
                else if (*myFunction == 0) {
                    current_state = S_END;
                }
                else {
                    current_state = S_ERROR;
                }
                break;

            case S_OPERAND: {
                float operand = 0.0;
                int decimal_flag = 0;
                float decimal_divisor = 1.0;
                if (isdigit(*myFunction) || *myFunction == '.'){
                    while (isdigit(*myFunction) || *myFunction == '.') {
                        if (*myFunction == '.') {
                            decimal_flag = 1;
                        }
                        else {
                            if (decimal_flag == 0) {
                                operand = operand * 10 + (*myFunction - '0');
                            }
                            else {
                                decimal_divisor *= 10;
                                operand = operand + (*myFunction - '0') / decimal_divisor;
                            }
                        }
                        myFunction++;
                    }
                    output[outputIndex].type = OPERAND;
                    output[outputIndex].value.operand = operand;
                    outputIndex++;
                }
                else if (*myFunction == 'x') {
                    myFunction++;
                    output[outputIndex].type = VARIABLE;
                    output[outputIndex].value.variable = 0.0;
                    outputIndex++;                    
                }
                if (isOperator(*myFunction)) {
                    current_state = S_OPERATOR;
                }
                else if (*myFunction == ')') {
                    current_state = S_CLOSE;
                }
                else if (*myFunction == 0) {
                    current_state = S_END;
                }
                else {
                    current_state = S_ERROR;
                }
                break;
            }

            case S_OPERATOR:
                while (stackTop >= 0 && isOperator(stack[stackTop]) && 
                       precedence(stack[stackTop]) >= precedence(*myFunction)) {
                    output[outputIndex].type = OPERATOR;
                    output[outputIndex].value.operator = stack[stackTop];
                    outputIndex++;
                    stackTop--;
                }
                stack[++stackTop] = *myFunction;
                myFunction++;
                current_state = S_START;
                break;

            case S_OPEN:
                stack[++stackTop] = *myFunction;
                myFunction++;
                current_state = S_START;
                break;

            case S_CLOSE:
                while (stackTop >= 0 && stack[stackTop] != '(') {
                    output[outputIndex].type = OPERATOR;
                    output[outputIndex].value.operator = stack[stackTop];
                    outputIndex++;
                    stackTop--;
                }
                if (stackTop >= 0) 
                    stackTop--; // loại bỏ dấu '('
                
                // Kiểm tra nếu có hàm sin (ký hiệu 's') trên đỉnh ngăn xếp
                if (stackTop >= 0 && stack[stackTop] == 's') {
                    output[outputIndex].type = OPERATOR;
                    output[outputIndex].value.operator = stack[stackTop];
                    outputIndex++;
                    stackTop--;
                }
                myFunction++;
                if (isOperator(*myFunction)) {
                    current_state = S_OPERATOR;
                }
                else if (*myFunction == ')') {
                    current_state = S_CLOSE;
                }
                else if (*myFunction == 0) {
                    current_state = S_END;
                }
                else if (isdigit(*myFunction) || *myFunction == '.') {
                    current_state = S_OPERAND;
                }
                else {
                    current_state = S_ERROR;
                }
                break;

            case S_END:
                while (stackTop >= 0) {
                    output[outputIndex].type = OPERATOR;
                    output[outputIndex].value.operator = stack[stackTop];
                    outputIndex++;
                    stackTop--;
                }
                output[outputIndex].type = OPERATOR;
                output[outputIndex].value.operator = 'E';
                outputIndex++;
                return output;
                break;

            case S_ERROR:
            default:
                printf("Input function error!!!\n");
                return NULL;
        }
    }
}

float evaluatePostfix(Token *postfix, float x_value) {
    float stack[MAX];
    int stackTop = -1;
    int i = 0;
    while (postfix[i].type != OPERATOR || postfix[i].value.operator != 'E') {
        if (postfix[i].type == OPERAND) {
            stack[++stackTop] = postfix[i].value.operand;
        } else if (postfix[i].type == VARIABLE) {
            stack[++stackTop] = x_value;
        } else if (postfix[i].type == OPERATOR) {
            char op = postfix[i].value.operator;
            if (op == 's') {  // xử lý hàm sin
                float operand = stack[stackTop--];
                float result = sin(operand * M_PI / 180.0);
                stack[++stackTop] = result;
            } else {
                float operand2 = stack[stackTop--];
                float operand1 = stack[stackTop--];
                float result = 0.0;
                switch (op) {
                    case '+': result = operand1 + operand2; break;
                    case '-': result = operand1 - operand2; break;
                    case '*': result = operand1 * operand2; break;
                    case '/': result = operand1 / operand2; break;
                    case '^': result = pow(operand1, operand2); break;
                    default:
                        printf("Unknown operator\n");
                        return 0.0;
                } // kết thúc switch
                stack[++stackTop] = result;
            }
        }
        i++;
    }
    return stack[stackTop];
}
