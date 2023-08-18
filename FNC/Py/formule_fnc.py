import re

class Node:
    def __init__(self, value):
        self.value = value
        self.left = None
        self.right = None

class Operator(Node):
    def __init__(self, value, left=None, right=None):
        super().__init__(value)
        self.left = left
        self.right = right

AND = Operator('&')
OR = Operator('|')
NOT = Operator('~')

OPERATOR_PRIORITY = {
    '~': 3,
    '&': 2,
    '|': 1,
}

def transform_to_cnf(formula):
    tokens = tokenize(formula)
    syntax_tree = build_syntax_tree(tokens)
    convert_to_cnf(syntax_tree)
    cnf_formula = tree_to_string(syntax_tree)
    return cnf_formula

def tokenize(formula):
    pattern = r"([&|~()])|\b\w+\b"
    tokens = re.findall(pattern, formula)
    tokens = [token for token in tokens if token.strip()]
    return tokens

def build_syntax_tree(tokens):
    stack = []
    
    for token in tokens:
        if token == '(':
            stack.append(token)
        elif token in [AND.value, OR.value, NOT.value]:
            operator = Operator(token)
            while stack and stack[-1] in [AND.value, OR.value, NOT.value] and OPERATOR_PRIORITY[stack[-1]] >= OPERATOR_PRIORITY[operator.value]:
                operator.left = stack.pop()
            stack.append(operator)
        elif token == ')':
            while stack and stack[-1] != '(':
                node = stack.pop()
                if stack and stack[-1] == NOT.value:
                    node = NOT(value='', left=node)
                    stack.pop()
                if stack and stack[-1] in [AND.value, OR.value]:
                    node.right = stack.pop()
                stack.append(node)
            stack.pop()
        else:
            stack.append(Node(token))
    
    return stack[0]

def convert_to_cnf(node):
    if isinstance(node, Operator):
        if node.value == OR.value:
            distribute_and_over_or(node)
        elif node.value == AND.value:
            convert_to_cnf(node.left)
            convert_to_cnf(node.right)
        elif node.value == NOT.value:
            convert_to_cnf(node.left)

def distribute_and_over_or(node):
    left_is_and = isinstance(node.left, Operator) and node.left.value == AND.value
    right_is_and = isinstance(node.right, Operator) and node.right.value == AND.value

    if left_is_and and right_is_and:
        new_left = Operator('|')
        new_left.left = Operator('&', left=node.left.left, right=node.right.left)
        new_left.right = Operator('&', left=node.left.right, right=node.right.left)
        node.value = '&'
        node.left = new_left
        node.right = Operator('&', left=node.left.right, right=node.right.right)
    elif left_is_and:
        new_left = Operator('|')
        new_left.left = Operator('&', left=node.left.left, right=node.right)
        new_left.right = Operator('&', left=node.left.right, right=node.right)
        node.value = '&'
        node.left = new_left
    elif right_is_and:
        new_right = Operator('|')
        new_right.left = Operator('&', left=node.left, right=node.right.left)
        new_right.right = Operator('&', left=node.left, right=node.right.right)
        node.value = '&'
        node.right = new_right

def tree_to_string(node):
    if isinstance(node, Operator):
        if node.value == AND.value:
            left_str = tree_to_string(node.left)
            right_str = tree_to_string(node.right)
            return f"({left_str} & {right_str})"
        elif node.value == OR.value:
            left_str = tree_to_string(node.left)
            right_str = tree_to_string(node.right)
            return f"({left_str} | {right_str})"
        elif node.value == NOT.value:
            return f"~{tree_to_string(node.left)}"
    else:
        return node

if __name__ == "__main__":
    logical_formula = input("Enter a logical formula: ")
    cnf_formula = transform_to_cnf(logical_formula)
    
    print("Conjunctive Normal Form:", cnf_formula)
