# Calculate-FIRST-
This program reads in a language and can determine the useful and useless  symbols, as well as  calculate all the first of all  non-terminals 

Input Format:
    The following context-free grammar specifies the input format:
       Rule-list → Rule Rule-list | Rule
        Id-list → ID Id-list | ID 
        Rule  → ID ARROW Right-hand-side HASH
        Right-hand-side → Id-list | 
        
The tokens used in the above grammar description are defined by the following regular expressions:

    ID = letter (letter + digit)*
    HASH = #
    DOUBLEHASH = ##
    ARROW = ->

Task 1
Task one simply outputs the list of non-terminals followed by the list of terminals in the order in which they appear in the
grammar rules.
  Example: For the input grammar
    decl -> idList colon ID #
    idList -> ID idList1 #
    idList1 -> #
    idList1 -> COMMA ID idList1 #
    ##
    
the expected output for task 1 is:
decl idList idList1 colon ID COMMA

Task 2: Eliminating Useless Symbols

    5.2.1 Useless Symbols: Definition
      The following is the definition for useless and useful symbols.
      Definition: Symbol A is useful if there is a derivation starting from S of a string of w of terminals, possibly empty (w ∈ T ∗),
      in which A appears:
          S ∗⇒ . . . ⇒ xAy ⇒ . . ∗⇒ w
    A symbol is useless if it is not useful.
        The algorithm for determining useless symbols is given in a separate presentation provided with the project documents.
        
    5.2.2 Task Requirements
      Determine useless symbols in the grammar and remove rules that contain useless symbols. Then output each of the remaining
      rules on a single line in the following format:
        <LHS> -> <RHS>
            Where <LHS> should be replaced by the left-hand side of the grammar rule and <RHS> should be replaced by the right-hand
       side of the grammar rule. If the grammar rule is of the form A → , use # to represent the epsilon. Note that this is different
       from the input format.
           The grammar rules should be printed in the same relative order that they originally had. So, if Rule1 and Rule2 are not
       removed after the elimination of useless symbols, and Rule1 appears before Rule2 in the original grammar, then Rule1 should be
       printed before Rule2 in the output.

   Example 1: Given the following input grammar :
      decl -> idList colon ID #
      idList -> ID idList1 #
      idList1 -> #
      idList1 -> COMMA ID idList1 #
      ##
      
  the expected output for task 2 is:
      decl -> idList colon ID
      idList -> ID idList1
      idList1 -> #
      idList1 -> COMMA ID idList1


Task 3: Calculate FIRST Sets
  Compute the FIRST sets of all non-terminals. Then, for each of the non-terminals of the input grammar, in the order that it 
  appears in the grammar, output one line in the following format:
      FIRST(<symbol>) = { <set_items> }
      where <symbol> should be replaced by the non-terminal name and <set_items> should be replaced by a comma-separated list of
      elements of the set ordered in the following manner.
          • If  belongs to the set, represent it as #.
          • If  belongs to the set, it should be listed before any other element of the set.
          • All other elements of the set should be sorted in the order in which they appear in the grammar.

      Example: Given the input grammar:
        decl -> idList colon ID #
        idList -> ID idList1 #
        idList1 -> #
        idList1 -> COMMA ID idList1 #
        ##
        
the expected output for task 3 is:
    FIRST(decl) = { ID }
    FIRST(idList) = { ID }
    FIRST(idList1) = { #, COMMA }
