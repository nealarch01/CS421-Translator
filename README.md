# A Small Japanese to English translator
- A final project for CS 421 Theory of Computing
- Uses CS concepts such as finite state machines, recursive descent parsing, left factoring grammar, etc. 
## Three parts of the program
1. Scanner:
    - Created using a Deterministic Finite Automata that accepts/rejects a string
    - Determines the token type of a valid word using an unordered map

2. Parser:
    - Implemented a recursive descent parser (bottom-up parser)
    - Each non-terminal has a corresponding function

3. Translator:
    - Semantic routines were added to the LHR factored grammar
    - Translated outputs are then stored in ```translated.txt```