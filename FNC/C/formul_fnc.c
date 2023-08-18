#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Node {
    char value;
    struct Node *left;
    struct Node *right;
} Node;

Node *createNode(char value) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->value = value;
    newNode->left = newNode->right = NULL;
    return newNode;
}

Node *transformToCNF(char *formula) {
    Node *stack[100];
    int top = -1;

    for (int i = 0; formula[i] != '\0'; i++) {
        if (formula[i] == ' ') {
            continue; // Skip spaces
        } else {
            Node *operandNode = createNode(formula[i]);
            stack[++top] = operandNode;
        }
    }

    return stack[0];
}

void printCNFResult(Node *root) {
    if (root != NULL) {
        printf("%c", root->value);
    }
}

int main() {
    char formula[100];
    printf("Enter a logical variable: ");
    fgets(formula, sizeof(formula), stdin);
    formula[strcspn(formula, "\n")] = '\0'; // Remove newline if present

    Node *root = transformToCNF(formula);

    printf("CNF result: ");
    printCNFResult(root);
    printf("\n");

    return 0;
}
