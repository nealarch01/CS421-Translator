
// compile command g++ -std=c++11 scanner.cpp

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

/* Look for all **'s and complete them */
//=====================================================
// File scanner.cpp written by: Group Number: 17
//=====================================================

ifstream fin;  // global stream for reading from the input file
ofstream tfout; // global output stream for writing into translated.txt

/*
includes characters: a,e,i,o,u
*/
bool vowel(char ch) {
    switch (ch) {
        case ('a'):
        case ('e'):
        case ('i'):
        case ('o'):
        case ('u'):
        case ('I'):
        case ('E'):
            return true;
    }
    return false;
}

/*
includes characters: b,g,m,k,h,p,r
*/
bool consonant_pair(char ch) {
    // NOTE: n, c, s, t are special cases and therefore were not included
    switch (ch) {
        case ('b'):
        case ('g'):
        case ('m'):
        case ('k'):
        case ('h'):
        case ('p'):
        case ('r'):
            return true;
    }
    return false;
}

/*
includes characters: d,w,z,y,j
*/
bool consonant_nonpair(char ch) {
    switch (ch) {
        case ('d'):
        case ('w'):
        case ('z'):
        case ('y'):
        case ('j'):
            return true;
    }
    return false;
}

// WORD DFA
// Done by: Neal and Gabriel
// RE: [[consonant] vowel^+]^*
// (consonant_pair | [consonant_pair | n] | [t | t s | s | s h | c h]) [vowel]^+
// (([consonant_pair] | [consonant_nonpair] | [t] | [t s] | [c] | [c h] | [s h]
// [vowel]^+ ) | (n) ([consonant_nonpair] | [consonant_pair] | [t | t s | s | s
// h | c h] [vowel]^+)) consonant_pair = b | g | m | k | h | p | r
// consonant_nonpair = d | w | z | y | j
bool word(string s) {
    enum dfaState {  // 0-7 in current order
        q0,          // 0
        qSA,         // 1
        qY,          // 2
        qT,          // 3
        qC,          // 4
        qS,          // 5
        q0qY,        // 6
        q0q1,        // 7
        invalid = -1
    };
    // using enum instead of string literals for safety
    dfaState state = q0;

    // using for loop iteration
    for (string::size_type i = 0; i < s.length(); i++) {
        char currentChar = s[i];
        // initial state (q0)
        if (state == q0) {
            if (vowel(currentChar) == true) {
                state = q0q1;

            } else if (consonant_nonpair(currentChar)) {
                state = qSA;
            }

            else if (consonant_pair(currentChar) == true) {
                state = qY;
            }

            // special pair consonant
            else if (currentChar == 'n') {
                state = qY;
            }

            else if (currentChar == 't') {
                state = qT;
            }

            else if (currentChar == 'y') {
                state = qY;
            }

            else if (currentChar == 'c') {
                state = qC;
            }

            else if (currentChar == 's') {
                state = qS;
            }

            else {
                state = invalid;
            }
        }
        // end of q0

        // qSA
        else if (state == qSA) {
            if (vowel(currentChar) == true) {
                state = q0q1;
            } else {
                state = invalid;
            }
        }
        // end of qSA

        // qY
        else if (state == qY) {
            if (vowel(currentChar) == true) {
                state = q0q1;
            }

            else if (currentChar == 'y') {
                state = qSA;
            }

            else {
                state = invalid;
            }
        }
        // end of qY

        // qT
        else if (state == qT) {
            if (vowel(currentChar) == true) {
                state = q0q1;
            }

            else if (currentChar == 's') {
                state = qSA;
            }

            else {
                state = invalid;
            }
        }
        // end of qT

        // qC
        else if (state == qC) {
            if (currentChar == 'h') {
                state = qSA;
            } else {
                state = invalid;
            }
        }

        // qS
        else if (state == qS) {
            if (vowel(currentChar) == true) {
                state = q0q1;
            }

            else if (currentChar == 'h') {
                state = qSA;
            }

            else {
                state = invalid;
            }
        }
        // end of qS

        // q0qY
        else if (state == q0qY) {
            if (vowel(currentChar)) {
                state = q0q1;
            } else if (consonant_nonpair(currentChar)) {
                state = qSA;
            }

            else if (consonant_pair(currentChar)) {
                state = qY;
            }

            else if (currentChar == 't') {
                state = qT;
            }

            else if (currentChar == 'c') {
                state = qC;
            }

            else if (currentChar == 's') {
                state = qS;
            }

            else if (currentChar == 'n') {
                // do nothing; n loops back to q0qY
            }

            else {
                state = invalid;
            }
        }
        // end of q0qY

        // q0q1
        else if (state == q0q1) {
            if (vowel(currentChar)) {
                state = q0q1;
            }

            else if (consonant_nonpair(currentChar)) {
                state = qSA;
            }

            else if (consonant_pair(currentChar)) {
                state = qY;
            }

            else if (currentChar == 't') {
                state = qT;
            }

            else if (currentChar == 'c') {
                state = qC;
            }

            else if (currentChar == 's') {
                state = qS;
            }

            else if (currentChar == 'n') {
                state = q0qY;
            }

            else {
                state = invalid;
            }
        }
        // end of q0q1
        // invalid state
        else {
            return false;
        }
    }  // end of for loop

    // check if currentState final states
    if (state == q0qY || state == q0q1) {
        return true;
    }
    return false;  // we have not reached one of the end states, return false
}

// PERIOD DFA
// Done by: Damien
// RE: [.]
bool period(string s) {  // complete this **
    int charpos = 0;

    enum dfaState {
        qI,  // initial state
        qF,  // final state
        invalid = -1
    };

    dfaState state = qI;  // initialize to the first state
    for (int i = 0; i < s.length(); i++) {
        if (state == qI) {
            if (s[i] == '.') {
                state = qF;
            } else {
                state = invalid;
            }
        }
        // if there's more to the string, and we're already in the final state,
        // then return false because we cannot leave qF or go back to itself
        else {
            state = invalid;
            return false;
        }
    }

    if (state == qF) {
        return true;
    }
    return false;
}

// ------ Three  Tables -------------------------------------

// TABLES Done by: Damien and Neal

// ** Update the tokentype to be WORD1, WORD2, PERIOD, ERROR, EOFM, etc.
enum tokentype {
    WORD1,
    WORD2,
    PERIOD,
    VERB,
    VERBNEG,
    VERBPAST,
    VERBPASTNEG,
    IS,
    WAS,
    OBJECT,
    SUBJECT,
    DESTINATION,
    PRONOUN,
    CONNECTOR,
    PUNCTUATION,
    ERROR,
    EOFM
};

// ** For the display names of tokens - must be in the same order as the
// tokentype.
string tokenName[30] = {"WORD1",   "WORD2",     "PERIOD",      "VERB",
                        "VERBNEG", "VERBPAST",  "VERBPASTNEG", "IS",
                        "WAS",     "OBJECT",    "SUBJECT",     "DESTINATION",
                        "PRONOUN", "CONNECTOR", "PUNCTUATION", "ERROR",
                        "EOFM"};

// ** Need the reservedwords table to be set up here.
// ** Do not require any file input for this. Hard code the table.
// ** a.out should work without any additional files.

// for the map
struct token_value {
    int type;  // key doesn't exist when type = -1

    // constructor to assign type, if none provided (map access of a
    // non-reserved word using [] operator, create a token_value object with
    // type -1)
    token_value(int token = -1) { type = token; }
};

unordered_map<string, token_value> reservedWordsMap = {
    {"masu", VERB},
    {"masen", VERBNEG},
    {"mashita", VERBPAST},
    {"masendeshita", VERBPASTNEG},
    {"desu", IS},
    {"deshita", WAS},
    {"o", OBJECT},
    {"wa", SUBJECT},
    {"ni", DESTINATION},
    {"watashi", PRONOUN},
    {"anata", PRONOUN},
    {"kare", PRONOUN},
    {"kanojo", PRONOUN},
    {"sore", PRONOUN},
    {"mata", CONNECTOR},
    {"soshite", CONNECTOR},
    {"shikashi", CONNECTOR},
    {"dakara", CONNECTOR}};

// ------------ Scanner and Driver -----------------------



// Scanner processes only one word each time it is called
// Gives back the token type and the word itself
// ** Done by: Neal and Gabriel
void scanner(tokentype& tt, string& w) {
    // ** Grab the next word from the file via fin
    // 1. If it is eofm, return right now.
    string inputRead;  // variable to store read word (read as in past tense)
    fin >> inputRead;  // read stream into word

    w = inputRead;  ////

    if (inputRead == "eofm") {
        tt = EOFM;
        return;
    }

    // step 2
    if (word(inputRead)) {
        // do reserved word lookup
        // first look up if word exists
        if (reservedWordsMap[inputRead].type !=
            -1) {  // if the word is reserved
            tt = tokentype(reservedWordsMap[inputRead]
                               .type);  // type cast int into enum type
        } else {                        // word is not reserved
            // determine whether word is word1 or word2 or neither
            char lastChar = inputRead[inputRead.length() - 1];
            switch (lastChar) {
                case ('a'):
                case ('e'):
                case ('i'):
                case ('o'):
                case ('u'):
                case ('n'):  // edge case; if word is valid and ends with n, has
                             // to be word1
                    tt = WORD1;
                    break;
                case ('I'):
                case ('E'):
                    tt = WORD2;
                    break;
            }
        }
    } else if (period(inputRead) == true) {
        tt = PERIOD;
    } else {
        cout << "Lexical error: " << w << " is not a valid token" << endl;
        tt = ERROR;
    }

    /*  **
    2. Call the token functions (word and period)
       one after another (if-then-else).
       Generate a lexical error message if both DFAs failed.
       Let the tokentype be ERROR in that case.

    3. If it was a word,
       check against the reservedwords list.
       If not reserved, tokentype is WORD1 or WORD2
       decided based on the last character.

    4. Return the token type & string  (pass by reference)
    */

}  // the end of scanner


/* INSTRUCTION:  Complete all ** parts.
   You may use any method to connect this file to scanner.cpp
   that you had written.
  e.g. You can copy scanner.cpp here by:
          cp ../ScannerFiles/scanner.cpp .
       and then append the two files into one:
          cat scanner.cpp parser.cpp > myparser.cpp
*/

//=================================================
// File parser.cpp written by Group Number: 17
//=================================================

// ----- Four Utility Functions and Globals -----------------------------------

// global token; stores last token returned by scanner
tokentype saved_token;
// flag for determining if there is a token saved
bool token_available;
// word being read in by the scanner
string saved_lexeme = "";

// ** Declare Lexicon (i.e. dictionary) that will hold the content of
// lexicon.txt Make sure it is easy and fast to look up the translation. Do not
// change the format or content of lexicon.txt
// Done by: Neal 
unordered_map<string, string> lexicon_dict = {}; // Initialize as empty
string saved_enWord = ""; // Initialize as empty string



// ** Need syntaxerror1 and syntaxerror2 functions (each takes 2 args)
//    to display syntax error messages as specified by me.

// Type of error: Generates a syntax error when there is mismatch between the
// token type and the current saved string Done by: Gabriel
void syntaxerror1(string& saved_lexeme, tokentype& token_type) {
    cout << "SYNTAX ERROR: expected " << tokenName[token_type] << " but found "
         << saved_lexeme << endl;
    exit(1);
}
// Type of error: Syntax error is generated when trying to match a nonterminal
// with a string
//                that is not accepted by the parser
// Done by: Gabriel
void syntaxerror2(string& saved_lexeme, string nonterm) {
    cout << "SYNTAX ERROR: unexpected " << saved_lexeme << " found in "
         << nonterm << endl;
    exit(1);
}

// ** Need the updated match and next_token with 2 global vars
// saved_token and saved_lexeme

// Purpose: Function calls scanner to read next available token, then compares
// token type
//          Displays syntax error before terminating
// Done by: Gabriel
tokentype next_token() {
    if (!token_available) {
        scanner(saved_token, saved_lexeme);
        cout << "Scanner called using word: " << saved_lexeme << endl;
        token_available = true;
    }
    return saved_token;
}

// Purpose: Calls next_token and compares to passed in token type
// Done by: Gabriel
bool match(tokentype expected) {
    if (next_token() != expected) {
        syntaxerror1(saved_lexeme, expected);
    } else {
        token_available = false;
        return true;
    }
}

// ** Additions to parser.cpp here:
//    getEword() - using the current saved_lexeme, look up the English word
//                 in Lexicon if it is there -- save the result
//                 in saved_E_word
//    gen(line_type) - using the line type,
//                     sends a line of an IR to translated.txt
//                     (saved_E_word or saved_token is used)

// Done by: Neal
void getEWord() {
  saved_enWord = lexicon_dict[saved_lexeme]; // Note: map returns an empty string if the word was not found
}

// Done by: Neal
void gen(tokentype line_type) {
  // 10 <tense> := VERBPAST  | VERBPASTNEG | VERB | VERBNEG
  // If statement checks if line_type is tense
  if (line_type == VERBPAST || line_type == VERBPASTNEG || line_type == VERB || line_type == VERBNEG) {
    tfout << "TENSE: " << tokenName[line_type] << "\n\n";
  } else {
    tfout << tokenName[line_type] << ": " << saved_enWord << "\n";
  }
}

// ----- RDP functions - one per non-term -------------------

// ** Make each non-terminal into a function here
// ** Be sure to put the corresponding grammar rule above each function
// ** Be sure to put the name of the programmer above each function

/*
1 <story> ::= <s> { <s> }  // stay in the loop as long as a possible start
                           // of <s> is the next_token  (note it can be
CONNECTOR or WORD1 or PRONOUN) 2 <s> ::=  [CONNECTOR] <noun> SUBJECT  <verb>
<tense> PERIOD 3 <s> ::=  [CONNECTOR] <noun> SUBJECT  <noun>  <be>      PERIOD
4 <s> ::=  [CONNECTOR] <noun> SUBJECT  <noun>  DESTINATION  <verb> <tense>
PERIOD 5 <s> ::=  [CONNECTOR] <noun> SUBJECT  <noun>  OBJECT   <verb> <tense>
PERIOD 6 <s> ::=  [CONNECTOR] <noun> SUBJECT  <noun>  OBJECT   <noun>
DESTINATION

7 <noun> ::= WORD1 | PRONOUN
8 <verb> ::= WORD2
9 <be> ::=   IS | WAS
10 <tense> := VERBPAST  | VERBPASTNEG | VERB | VERBNEG
*/

/**** completed left hand factoring, use these nonterminals along side noun,
verb, be, and tense <s> ::= [CONNECTOR]<noun> SUBJECT <afterSubject>
  <afterSubject>::= <verb><tense>PERIOD | <noun> <afterNoun>
          <afterNoun>::= <be>PERIOD | DESTINATION <verb><tense>PERIOD |
                                                          | OBJECT <afterObject>
                  <afterObject>::= <noun> DESTINATION <afterVerb> | <afterVerb>
                            <afterVerb> ::= <verb> <tense> PERIOD
*/

// Grammar: <noun> ::= WORD1 | PRONOUN
// Done by: Neal
void noun() {
    cout << "processing <noun>\n";
    switch (next_token()) {
        case WORD1:
            match(WORD1);
            cout << "matched WORD1\n";
            break;
        case PRONOUN:
            match(PRONOUN);
            cout << "matched PRONOUN\n";
            break;
        default:
            syntaxerror2(saved_lexeme, "noun");
            break;
    }
}

// Grammar: <verb> ::= WORD2
// Done by: Neal
void verb() {
    switch (next_token()) {
        case WORD2:
            match(WORD2);
            cout << "matched WORD2\n";
            break;
        default:
            syntaxerror2(saved_lexeme, "verb");
    }
}

// Grammar: <be> ::=   IS | WAS
// Done by: Gabriel
void be() {
    cout << "processing <be>\n";
    switch (next_token()) {
        case IS:
            match(IS);
            cout << "matched IS\n";
            break;
        case WAS:
            match(WAS);
            cout << "matched WAS\n";
            break;
        default:
            syntaxerror2(saved_lexeme, "be");
    }
}

// Grammar: <tense> := VERBPAST  | VERBPASTNEG | VERB | VERBNEG
// Done by: Gabriel
void tense() {
    switch (next_token()) {
        case VERBPAST:
            match(VERBPAST);
            cout << "matched VERBPAST\n";
            break;
        case VERBPASTNEG:
            match(VERBPASTNEG);
            cout << "matched VERBPASTNEG\n";
            break;
        case VERB:
            match(VERB);
            cout << "matched VERB\n";
            break;
        case VERBNEG:
            match(VERBNEG);
            cout << "matched VERBNEG\n";
            break;
        default:
            syntaxerror2(saved_lexeme, "tense");
    }
}

/**
 * utility function for <verb><tense>PERIOD phrase
 * Done by: Gabriel
 */
void verbTensePeriod() {
    int isVerb = 0;
    bool hasPeriod = false;
    while (!hasPeriod) {
        if (isVerb == 0) {
            cout << "processing <verb>\n";
        } else if (isVerb == 1) {
            cout << "processing <tense>\n";
        }
        switch (next_token()) {
            case WORD2:  // all <verb> non-terms should go to word2
                verb();
                break;

            case VERBPAST:
                tense();
                break;
            case VERBPASTNEG:
                tense();
                break;
            case VERB:
                tense();
                break;
            case VERBNEG:
                tense();
                break;

            case PERIOD:
                match(PERIOD);
                cout << "matched PERIOD\n";
                hasPeriod = true;
                break;

            default:
                syntaxerror2(saved_lexeme, "afterSubject");
                break;
        }  // end switch
        isVerb++;
    }  // end while
    // cout << "\nexiting verbTensePeriod()\n\n";
}
/**** completed left hand factoring, use these nonterminals along side noun,
verb, be, and tense <s> ::= [CONNECTOR]<noun> SUBJECT <afterSubject>
  <afterSubject>::= <verb><tense>PERIOD | <noun> <afterNoun>
          <afterNoun>::= <be>PERIOD | DESTINATION <verb><tense>PERIOD |
                                                          | OBJECT <afterObject>
                  <afterObject>::= <noun> DESTINATION <verb><tense>PERIOD
                       |<verb><tense>PERIOD
*/

// Grammar: <afterObject>::= <noun> DESTINATION <verb><tense>PERIOD |
//<verb><tense>PERIOD
// Done by: Gabriel
void afterObject() {
    cout << "processing <afterObject>\n";
    bool isValid = false;

    while (!isValid) {
        switch (next_token()) {
            //<noun> DESTINATION <verb><tense>PERIOD cases
            case WORD1:
                noun();
                break;
            case PRONOUN:
                noun();
                break;
            case DESTINATION:
                match(DESTINATION);
                verbTensePeriod();
                isValid = true;
                break;

            //<verb><tense>PERIOD case
            case WORD2:
                verbTensePeriod();
                isValid = true;
                break;

            default:
                syntaxerror2(saved_lexeme, "afterObject");
                break;
        }  // end switch
    }      // end while
           //  cout << "\nexiting afterObject()\n\n";
}

// Grammar: <afterNoun>::= <be>PERIOD | DESTINATION <verb><tense>PERIOD |
//			  		          | OBJECT <afterObject>
// Done by: Gabriel
void afterNoun() {
    cout << "processing <afterNoun>\n";
    bool isValid = false;
    while (!isValid) {
        switch (next_token()) {
            //<be> PERIOD cases
            case IS:
                be();
                break;
            case WAS:
                be();
                break;
            case PERIOD:
                match(PERIOD);
                cout << "matched PERIOD\n";
                isValid = true;
                break;

            // DESTINATION <verb><tense>PERIOD case
            case DESTINATION:
                match(DESTINATION);
                cout << "matched DESTINATION\n";
                verbTensePeriod();
                isValid = true;
                break;

            // OBJECT <afterObject> case
            case OBJECT:
                match(OBJECT);
                cout << "matched OBJECT\n";
                afterObject();
                isValid = true;
                break;

            default:
                syntaxerror2(saved_lexeme, "afterNoun");
                break;
        }
    }  // end while
       // cout << "\nexiting afterNoun()\n\n";
}

// Grammar: <afterSubject>::= <verb><tense>PERIOD | <noun> <afterNoun>
// Done by: Gabriel
void afterSubject() {
    cout << "processing <afterSubject>\n";
    bool isValid = false;
    while (!isValid) {
        switch (next_token()) {
                //<noun> <afterNoun> cases
            case WORD1:
                noun();
                afterNoun();
                isValid = true;
                break;
            case PRONOUN:
                noun();
                afterNoun();
                isValid = true;
                break;

                //<verb><tense>PERIOD cases
            case WORD2:  // all <verb> non-terms should go to word2
                verbTensePeriod();
                isValid = true;
                break;

            default:
                syntaxerror2(saved_lexeme, "afterSubject");
                break;
        }  // end switch
    }      // end while
           // cout << "\nexiting afterSubject()\n\n";
}

/* Grammar: <s> ::= [CONNECTOR] <noun> SUBJECT  <verb>   <tense> PERIOD
                      | [CONNECTOR] <noun> SUBJECT  <noun>  <be>      PERIOD
                      | [CONNECTOR] <noun> SUBJECT  <noun>  DESTINATION  <verb>
   <tense> PERIOD | [CONNECTOR] <noun> SUBJECT  <noun>  OBJECT   <verb> <tense>
   PERIOD | [CONNECTOR] <noun> SUBJECT  <noun>  OBJECT   <noun> DESTINATION
*/
// Grammar: <s> ::= [CONNECTOR]<noun> SUBJECT <afterSubject>
// Done by: Gabriel, Neal
void s() {
    cout << "processing <s>\n";
    if (next_token() == CONNECTOR) {
        match(CONNECTOR);
        cout << "matched CONNECTOR\n";
    }
    noun();  // unconditionally process noun
    next_token();
    match(SUBJECT);
    cout << "matched SUBJECT\n";
    afterSubject();
}

// Grammar: <story> ::= <s> { <s> }
// Done by: Neal
void story() {
    cout << "Group #17: Neal, Damien, Gabriel\n\n";
    // Since this is the entry point, do an infinite loop to keep parsing until
    // over
    cout << "Processing <story>\n\n";
    s();  // Grammar says process one <s> first
    while (1) {
        if (next_token() == EOFM) {
            cout << "\nSuccessfully parsed <story>\n";
            return;
        }
        s();  // repeatable when in curly braces
    }
}

/* INSTRUCTION:  copy your parser.cpp here
      cp ../ParserFiles/parser.cpp .
   Then, insert or append its contents into this file and edit.
   Complete all ** parts.
*/

//=================================================
// File translator.cpp written by Group Number: 17
//=================================================



// ----- Changes to the parser.cpp content ---------------------

// ** Comment update: Be sure to put the corresponding grammar
//    rule with semantic routine calls
//    above each non-terminal function

// ** Each non-terminal function should be calling
//    getEword and/or gen now.

// ---------------- Driver ---------------------------

// The final test driver to start the translator
// Done by:  Neal
int main() {
    
    ifstream lfin; // file stream for lexicon.txt
    lfin.open("lexicon.txt"); //** opens the lexicon.txt file and reads it into Lexicon

    string japanese_word;
    string english_word;
    while (lfin >> japanese_word) {
      lfin >> english_word;
      lexicon_dict.insert({japanese_word, english_word});
    }
    
    lfin.close(); //** closes lexicon.txt

    //** opens the output file translated.txt
    tfout.open("translated.txt");

    string filename;
    cout << "Enter the input file name: ";
    cin >> filename;
    fin.open(filename.c_str());

    //** calls the <story> to start parsing
    story();
    //** closes the input file
    fin.close();
    //** closes traslated.txt
    tfout.close();

}  // end
//** require no other input files!
//** syntax error EC requires producing errors.txt of error messages
//** tracing On/Off EC requires sending a flag to trace message output functions
