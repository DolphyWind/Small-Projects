import random
import sys

class BooleanExpressionGenerator:
    def __init__(self, num_variables):
        """
        Initialize with a specific number of variables.
        Variables will be named X1, X2, ..., Xn.
        """
        self.variables = [f"X{i+1}" for i in range(num_variables)]
        self.operators_binary = ["AND", "OR"]
        self.operator_unary = "NOT"

    def generate(self, max_depth=3, probability_terminal=0.3):
        """
        Generates a random boolean expression string.
        
        :param max_depth: How deep the nested parentheses can go.
        :param probability_terminal: Chance to stop recursion early and pick a variable 
                                     before hitting max_depth (0.0 to 1.0).
        """
        return self._build_tree(0, max_depth, probability_terminal)

    def _build_tree(self, current_depth, max_depth, prob_terminal):
        # 1. Base Case: If we reached max depth, we MUST return a variable
        # OR if random chance hits the terminal probability, return a variable
        if current_depth >= max_depth or random.random() < prob_terminal:
            return random.choice(self.variables)

        # 2. Recursive Step: Choose an operator type
        # 0 = Unary (NOT), 1 = Binary (AND/OR)
        # We give Binary a higher weight usually to make expressions interesting
        op_type = random.choice(['unary', 'binary', 'binary']) 

        if op_type == 'unary':
            # Generate: NOT (expression)
            inner_expr = self._build_tree(current_depth + 1, max_depth, prob_terminal)
            
            # If the inner expression is complex (has spaces), wrap in parens for clarity
            if " " in inner_expr:
                return f"{self.operator_unary} ({inner_expr})"
            return f"{self.operator_unary} {inner_expr}"

        else: # binary
            # Generate: (expression AND/OR expression)
            op = random.choice(self.operators_binary)
            left = self._build_tree(current_depth + 1, max_depth, prob_terminal)
            right = self._build_tree(current_depth + 1, max_depth, prob_terminal)
            return f"({left} {op} {right})"

# --- Usage Example ---

if __name__ == "__main__":
    if sys.argv.__len__() < 3:
        print(f"Usage: {sys.argv[0]} [num_variables] [max_depth]")
        exit(1)
    num_variables: int = int(sys.argv[1])
    max_depth: int = int(sys.argv[2])
    generator = BooleanExpressionGenerator(num_variables=num_variables)

    print(generator.generate(max_depth=max_depth, probability_terminal=0.1))
