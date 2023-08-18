#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Structure of a node in the syntax tree
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

// Helper function to tokenize the formula
char *nextToken(char **formula) {
    char *token = strtok(*formula, " ");
    *formula = NULL;
    return token;
}

// Recursive function to build syntax tree from postfix notation
Node *buildSyntaxTree(char *postfix) {
    Node *stack[100];
    int top = -1;

    char *token = nextToken(&postfix);
    while (token != NULL) {
        if (token[0] == '&' || token[0] == '|') {
            Node *right = stack[top--];
            Node *left = stack[top--];
            Node *operatorNode = createNode(token[0]);
            operatorNode->left = left;
            operatorNode->right = right;
            stack[++top] = operatorNode;
        } else if (token[0] == '~') {
            Node *operand = stack[top--];
            Node *notNode = createNode('~');
            notNode->left = operand;
            stack[++top] = notNode;
        } else {
            stack[++top] = createNode(token[0]);
        }
        token = nextToken(&postfix);
    }
    return stack[0];
}

// Print the syntax tree (for debugging)
void printTree(Node *root) {
    if (root != NULL) {
        printTree(root->left);
        printf("%c ", root->value);
        printTree(root->right);
    }
}

// Helper function to negate a node
Node *negateNode(Node *node) {
    Node *notNode = createNode('~');
    notNode->left = node;
    return notNode;
}

// Helper function to apply De Morgan's law (A | (B & C)) = (A | B) & (A | C)
Node *applyDeMorgans(Node *node) {
    if (node->value == '|') {
        Node *leftAnd = createNode('&');
        Node *rightAnd = createNode('&');
        leftAnd->left = node->left;
        leftAnd->right = node->right;
        rightAnd->left = node->left;
        rightAnd->right = node->right;

        node->value = '&';
        node->left = leftAnd;
        node->right = rightAnd;
    }
    return node;
}

// Helper function to distribute AND over OR
Node *distributeANDOverOR(Node *node) {
    if (node->value == '&') {
        if (node->left->value == '|') {
            node->left = applyDeMorgans(node->left);
            node = applyDeMorgans(node);
        } else if (node->right->value == '|') {
            node->right = applyDeMorgans(node->right);
            node = applyDeMorgans(node);
        }
    }
    return node;
}

// Convert the syntax tree to CNF
void convertToCNF(Node *root) {
    if (root == NULL) {
        return;
    }

    // Recursively convert subtrees
    convertToCNF(root->left);
    convertToCNF(root->right);

    // Apply CNF conversion rules
    if (root->value == '~') {
        if (root->left->value == '~') {
            root = root->left->left;
        } else if (root->left->value == '&') {
            root->value = '|';
            root->left = negateNode(root->left->left);
            root->right = negateNode(root->left->right);
        } else if (root->left->value == '|') {
            root->value = '&';
            root->left = negateNode(root->left->left);
            root->right = negateNode(root->left->right);
        }
    } else if (root->value == '&') {
        root = distributeANDOverOR(root);
    } else if (root->value == '|') {
        root = distributeANDOverOR(root);
    }
}

// Helper function to print the CNF result
void printCNFResult(Node *root) {
    if (root == NULL) {
        return;
    }

    if (root->value == '&') {
        printCNFResult(root->left);
        printf(" & ");
        printCNFResult(root->right);
    } else if (root->value == '|') {
        printf("(");
        printCNFResult(root->left);
        printf(" | ");
        printCNFResult(root->right);
        printf(")");
    } else if (root->value == '~') {
        printf("~");
        printCNFResult(root->left);
    } else {
        printf("%c", root->value);
    }
}

int main() {
    char postfix_formula[100];
    printf("Enter postfix logical formula: ");
    fgets(postfix_formula, sizeof(postfix_formula), stdin);
    postfix_formula[strcspn(postfix_formula, "\n")] = '\0'; // Remove newline if present

    Node *syntax_tree = buildSyntaxTree(postfix_formula);

    printf("Syntax tree: ");
    printTree(syntax_tree);

    convertToCNF(syntax_tree);

    printf("CNF result: ");
    printCNFResult(syntax_tree);

    return 0;
}
