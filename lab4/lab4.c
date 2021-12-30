#include <stdio.h>
#include <stdlib.h>

#define MAX_VALUE 1000

typedef struct _Stack {
    int data[MAX_VALUE];
    int top;
} _Stack;

void PushElement(_Stack *stack, int value) {
    if (stack->top < MAX_VALUE) {
        stack->data[stack->top] = value;
        stack->top++;
    }
}

int PopElement(_Stack *stack) {
    if ((stack->top) > 0) {
        stack->top--;
        return stack->data[stack->top];
    }
    return 0;
}

int IsEmptyStack(const _Stack *Stack) { return ((Stack->top) == 0); }

int IsOperator(char symbol) {
    return (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/');
}

int IsOperand(char symbol) { return (symbol >= '0' && symbol <= '9'); }

int DeterminePrecedenceOfOperators(char symbol) {
    switch (symbol) {
        case '/':
        case '*':
            return 3;

        case '+':
        case '-':
            return 2;

        case '(':
        case ')':
            return 1;

        default:
            return -1;
    }
}

int CalculateMathExpression(_Stack *StackOfNumbers, char symbol) {
    if (symbol == '(') return 1;

    int a, b;
    if (!IsEmptyStack(StackOfNumbers))
        a = PopElement(StackOfNumbers);
    else
        return 1;
    if (!IsEmptyStack(StackOfNumbers))
        b = PopElement(StackOfNumbers);
    else
        return 1;

    if (symbol == '+')
        PushElement(StackOfNumbers, b + a);
    else if (symbol == '-')
        PushElement(StackOfNumbers, b - a);
    else if (symbol == '*')
        PushElement(StackOfNumbers, b * a);
    else if (symbol == '/') {
        if (a != 0)
            PushElement(StackOfNumbers, b / a);
        else return 2;
    }
    return 0;
}

int ConvertToPolishNotation(_Stack *StackOfNumbers, _Stack *StackOfOperators,
                            char *readInputString) {
    int symbol, j, i = 0;
    int newNumber, numberOfClosingBrackets = 0, numberOpeningAndClosingBrackets = 0;
    while ((symbol = getchar()) != '\n') {
        if (IsOperand(symbol)) {
            readInputString[i] = symbol;
            ++i;
            while (IsOperand(symbol = getchar())) {
                readInputString[i] = symbol;
                ++i;
            }
            newNumber = atoi(readInputString);
            numberOfClosingBrackets = 1;
            PushElement(StackOfNumbers, newNumber);
            for (j = 0; j < i; ++j) readInputString[j] = 0;
            i = 0;
        }
        if (symbol == '(') {
            numberOfClosingBrackets = 0;
            PushElement(StackOfOperators, symbol);
            ++numberOpeningAndClosingBrackets;
            continue;
        } else if (symbol == ')') {
            --numberOpeningAndClosingBrackets;
            if (((numberOfClosingBrackets != 0) &&
                 (numberOpeningAndClosingBrackets >= 0))) {
                while (StackOfOperators->data[StackOfOperators->top - 1] != '(') {
                    int errorWhileCalculating = CalculateMathExpression(
                        StackOfNumbers, PopElement(StackOfOperators));
                    if (errorWhileCalculating) return errorWhileCalculating;
                }
                PopElement(StackOfOperators);
            } else
                return 1;
        } else if (IsOperator(symbol)) {
            while ((!IsEmptyStack(StackOfOperators)) &&
                   (DeterminePrecedenceOfOperators(
                        StackOfOperators->data[StackOfOperators->top - 1]) >=
                    DeterminePrecedenceOfOperators(symbol))) {
                int errorWhileCalculating = CalculateMathExpression(
                    StackOfNumbers, PopElement(StackOfOperators));
                if (errorWhileCalculating) return errorWhileCalculating;
            }
            PushElement(StackOfOperators, symbol);
            continue;
        } else if ((symbol != '\n') && (symbol != EOF)) return 1;
        else if (symbol == '\n') break;
    }
    if (!IsEmptyStack(StackOfNumbers)) {
        while (!IsEmptyStack(StackOfOperators)) {
            int errorWhileCalculating = CalculateMathExpression(
                StackOfNumbers, PopElement(StackOfOperators));
            if (errorWhileCalculating) return errorWhileCalculating;
        }
    } else return 1;
    return 0;
}

int main(void) {
    _Stack StackOfNumbers;
    _Stack StackOfOperators;
    StackOfNumbers.top = 0;
    StackOfOperators.top = 0;
    char readInputString[MAX_VALUE] = {0};
    int errorWhileCalculating =
        ConvertToPolishNotation(&StackOfNumbers, &StackOfOperators, readInputString);

    if (errorWhileCalculating == 1 || StackOfNumbers.top <= -1)
        printf("syntax error");
    else if (errorWhileCalculating == 2)
        printf("division by zero");
    else
        printf("%d", StackOfNumbers.data[StackOfNumbers.top - 1]);
}
