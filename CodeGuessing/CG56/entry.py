from math import inf
from sys import argv

class Parser:
  def __init__(self, string):
    self.string = string
    self.position = 0

  def consume(self):
    x = self.current()
    self.position += 1
    return x

  def current(self):
    return self.string[self.position] if self.position < len(self.string) else None

  def rewind(self, n=1):
    self.position -= n


class Phi:
  def __str__(self):
    return "[]"

PHI = Phi()


# Grammar:
# regex       := alternative
# alternative := branch { "|" branch }
# branch      := { atom { "*" } }
# atom        := regex_char | "\\" any_char | "[]" | "(" alternative ")"


def parse_regex(string):
  parser = Parser(string)
  thing = parse_alternative(parser)
  assert parser.current() is None
  return thing

def parse_alternative(parser):
  branches = [parse_branch(parser)]
  while parser.current() == "|":
    parser.consume()
    branches.append(parse_branch(parser))
  return branches

def parse_branch(parser):
  atoms = []
  atom = None
  while (atom := parse_atom(parser)) is not None:
    if parser.current() == "*":
      if parser.current() == "*":
        parser.consume()
      kleene = True
    else: kleene = False
    atoms.append((atom, kleene))
  return atoms

def parse_atom(parser):
  if not parser.current() or parser.current() not in "()[]|*\\":
    return parser.consume()
  match parser.consume():
    case "\\":
      return parser.consume()
    case "[":
      assert parser.consume() == "]"
      return PHI
    case "(":
      subexpr = parse_alternative(parser)
      assert parser.consume() == ")"
      return subexpr
    case other:
      assert other in ")|"
      parser.rewind()
      return None


def find_alt(expr):
  expr = list(filter(lambda x: x is not PHI, map(find_branch, expr)))
  if not expr: return PHI
  return min(expr, key=len)

def find_branch(branch):
  branch = list(map(lambda x: find_atom(x[0]), filter(lambda x: not x[1],
                                                      branch)))
  if PHI in branch: return PHI
  return "".join(branch)

def find_atom(atom):
  if isinstance(atom, list): return find_alt(atom)
  return atom


def entry(string):
  expression = parse_regex(string)
  return find_alt(expression)


if __name__ == "__main__":
  with open(argv[1], 'r') as f:
        print("Parsed string: \"", entry(f.read()[:-1]), "\"", sep='')
