from sympy import symbols, Not, And, Or, to_cnf

def transform_to_cnf(formula):
    variables = symbols(formula)
    parsed_formula = eval(formula.replace('&', 'And').replace('|', 'Or').replace('~', 'Not '))
    cnf_formula = to_cnf(parsed_formula)
    return cnf_formula

if __name__ == "__main__":
    logical_formula = input("Enter a logical formula: ")
    cnf_formula = transform_to_cnf(logical_formula)
    
    print("Conjunctive Normal Form:", cnf_formula)
