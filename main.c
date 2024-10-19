#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "dsa.h"

typedef enum TYPE {
  NUMBER,
  OPERATOR
} TYPE;

typedef struct Token {
  TYPE type;
  union {
    double value;
    char operator;
  };
} Token;

int isOperator(char c) {
  return (c == '+' || c == '-' || c == '*' || c == '/' || c =='^' || c == '(' || c == ')');
}

int operatorPrec(char operator) {
  int precedence;
  switch (operator) {
    case '^':
      precedence = 3;
      break;
    case '*':
      precedence = 2;
      break;
    case '/':
      precedence = 2;
      break;
    case '+':
      precedence = 1;
      break;
    case '-':
      precedence = 1;
      break;
  }
  return precedence;
}

double operate(double num1, double num2, char op) {
  double res;
  switch (op) {
    case '^':
      res = pow(num1, num2);
      break;
    case '*':
      res = num1*num2;
      break;
    case '/':
      res = num1/num2;
      break;
    case '+':
      res = num1 + num2;
      break;
    case '-':
      res = num1 - num2;
      break;
  }
  return res;
};

typedef enum {
  TOKEN_INITIAL,
  TOKEN_NUMERIC,
  TOKEN_OPERATOR
} TokenFSM;

c_queue* tokenize(char* expr) {
  c_queue *tokens = queueInit();
  char *c = expr;

  TokenFSM currentState = TOKEN_INITIAL;
  TokenFSM nextState = currentState;
  Token *token;
  while (*c) {
    switch (currentState) {
      case TOKEN_INITIAL:
        token = (Token*) malloc(sizeof(Token));
        if (*c == ' ') {
          c++;
          free(token);
        } else if ('0' <= *c && *c <= '9') {
          nextState = TOKEN_NUMERIC;
          token->type = NUMBER;
          token->value = 0;
        } else if (isOperator(*c)) {
          nextState = TOKEN_OPERATOR;
        } else {
          fprintf(stderr, "Invalid input: %c\n", *c);
          exit(EXIT_FAILURE);
        }
        break;
      case TOKEN_NUMERIC:
        if ('0' <= *c && *c <= '9') {
          token->value = token->value*10 + (*c - '0');
          c++;
        } else {
          enqueue(tokens, token);
          nextState = TOKEN_INITIAL;
        }
        break;
      case TOKEN_OPERATOR:
        token->type = OPERATOR;
        token->operator = *c;
        enqueue(tokens, token);
        nextState = TOKEN_INITIAL;
        c++;
        break;
    }
    currentState = nextState;
  }

  if (token->type == NUMBER) {
    enqueue(tokens, token);
  }

  return tokens;
}

double calc(char* expr) {
  c_stack *operators = stackInit(), *res = stackInit();
  c_queue *rpn = queueInit(), *tokens;
  tokens = tokenize(expr);

  while (!queueIsEmpty(tokens)) {
    Token *cur = (Token*) queuePeek(tokens);
    dequeue(tokens);
    if (cur->type == NUMBER) {
      enqueue(rpn, cur);
    } else {
      Token *topOperator;
      if (cur->operator == '(') {
        stackPush(operators, cur);
      } else if (cur->operator == ')') {
        while (!stackIsEmpty(operators) && (topOperator = (Token*)stackPeek(operators))->operator != '(') {
          stackPop(operators);
          enqueue(rpn, topOperator);
        }
        stackPop(operators);
        continue;
      } else if (stackIsEmpty(operators)
          || operatorPrec(cur->operator) >= operatorPrec((topOperator = (Token*)stackPeek(operators))->operator)) {
        stackPush(operators, cur);
      } else {
        while (!stackIsEmpty(operators) && operatorPrec(cur->operator) <= operatorPrec(topOperator->operator)) {
          stackPop(operators);
          enqueue(rpn, topOperator);
          if (!stackIsEmpty(operators)) {
            topOperator = (Token*) stackPeek(operators);
          }
        }
        stackPush(operators, cur);
      }
      topOperator = (Token*) stackPeek(operators);
    }
  }

  while (!stackIsEmpty(operators)) {
    Token *cur = (Token*) stackPeek(operators);
    stackPop(operators);
    enqueue(rpn, cur);
  }

  while (!queueIsEmpty(rpn)) {
    Token *cur = queuePeek(rpn);
    dequeue(rpn);
    if (cur->type == NUMBER) {
      stackPush(res, cur);
    } else {
      Token *num2 = stackPeek(res); stackPop(res);
      Token *num1 = stackPeek(res); stackPop(res);
      Token *answer = (Token*) malloc(sizeof(Token));
      answer->value =  operate(num1->value, num2->value, cur->operator);
      stackPush(res, answer);
    }
  }

  if (stackIsEmpty(res)) {
    fprintf(stderr, "Did not evaluate correctly.\n");
    exit(EXIT_FAILURE);
  }

  double result = ((Token*)stackPeek(res))->value;
  return result;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: ./calc [expression]\n");
    return 0;
  }
  char *expression = argv[1];
  printf("%f\n", calc(expression));
  return 0;
}
