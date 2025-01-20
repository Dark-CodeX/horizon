/**
 * @file token_type.hh
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#ifndef HORIZON_TOKEN_TYPE_TOKEN_TYPE_HH
#define HORIZON_TOKEN_TYPE_TOKEN_TYPE_HH

namespace horizon
{
    enum class token_type : unsigned char // max 256 tokens
    {
        TOKEN_IDENTIFIER,                          // Represents any identifier
        TOKEN_CHAR_LITERAL,                        // Represents any single-byte character
        TOKEN_STRING_LITERAL,                      // Represents any string
        TOKEN_INTEGER_LITERAL,                     // Represents any integer in any format
        TOKEN_DECIMAL_LITERAL,                     // Represents any floating point integer in any format
        TOKEN_ARITHMETIC_ADD,                      // Represents addition operator (+)
        TOKEN_ARITHMETIC_SUBSTRACT,                // Represents subtraction operator (-)
        TOKEN_ARITHMETIC_MULTIPLY,                 // Represents multiplication operator (*)
        TOKEN_ARITHMETIC_POWER,                    // Represents exponentiation operator (**)
        TOKEN_ARITHMETIC_DIVIDE,                   // Represents division operator (/)
        TOKEN_ARITHMETIC_MODULUS,                  // Represents modulus operator (%)
        TOKEN_RELATIONAL_EQUAL_TO,                 // Represents equal-to operator (==)
        TOKEN_RELATIONAL_NOT_EQUAL_TO,             // Represents not-equal-to operator (!=)
        TOKEN_RELATIONAL_GREATER_THAN,             // Represents greater-than operator (>)
        TOKEN_RELATIONAL_LESS_THAN,                // Represents less-than operator (<)
        TOKEN_RELATIONAL_GREATER_THAN_OR_EQUAL_TO, // Represents greater-than-or-equal-to operator (>=)
        TOKEN_RELATIONAL_LESS_THAN_OR_EQUAL_TO,    // Represents less-than-or-equal-to operator (<=)
        TOKEN_LOGICAL_NOT,                         // Represents logical NOT operator (!)
        TOKEN_LOGICAL_AND,                         // Represents logical AND operator (&&)
        TOKEN_LOGICAL_OR,                          // Represents logical OR operator (||)
        TOKEN_BITWISE_NOT,                         // Represents bitwise NOT operator (~)
        TOKEN_BITWISE_AND,                         // Represents bitwise AND operator (&)
        TOKEN_BITWISE_OR,                          // Represents bitwise OR operator (|)
        TOKEN_BITWISE_XOR,                         // Represents bitwise XOR operator (^)
        TOKEN_BITWISE_LEFT_SHIFT,                  // Represents left shift operator (<<)
        TOKEN_BITWISE_RIGHT_SHIFT,                 // Represents right shift operator (>>)
        TOKEN_ASSIGN,                              // Represents simple assignment operator (=)
        TOKEN_ASSIGN_ADD,                          // Represents addition assignment operator (+=)
        TOKEN_ASSIGN_SUBSTRACT,                    // Represents subtraction assignment operator (-=)
        TOKEN_ASSIGN_MULTIPLY,                     // Represents multiplication assignment operator (*=)
        TOKEN_ASSIGN_POWER,                        // Represents multiplication assignment operator (**=)
        TOKEN_ASSIGN_DIVIDE,                       // Represents division assignment operator (/=)
        TOKEN_ASSIGN_MODULUS,                      // Represents modulus assignment operator (%=)
        TOKEN_ASSIGN_BITWISE_AND,                  // Represents bitwise AND assignment operator (&=)
        TOKEN_ASSIGN_BITWISE_OR,                   // Represents bitwise OR assignment operator (|=)
        TOKEN_ASSIGN_BITWISE_XOR,                  // Represents bitwise XOR assignment operator (^=)
        TOKEN_ASSIGN_LEFT_SHIFT,                   // Represents left shift assignment operator (<<=)
        TOKEN_ASSIGN_RIGHT_SHIFT,                  // Represents right shift assignment operator (>>=)
        TOKEN_INCREMENT,                           // Represents increment operator (++)
        TOKEN_DECREMENT,                           // Represents decrement operator (--)
        TOKEN_QUESTION,                            // Represents the ternary conditional operator (?)
        TOKEN_SEMICOLON,                           // Represents semicolon (;)
        TOKEN_COLON,                               // Represents colon (:)
        TOKEN_MEMEBER_ACCESS,                      // Represents double-colon (:)
        TOKEN_COMMA,                               // Represents comma (,)
        TOKEN_DOT,                                 // Represents dot (.)
        TOKEN_RIGHT_PAREN,                         // Represents right parenthesis )
        TOKEN_LEFT_PAREN,                          // Represents left parenthesis (
        TOKEN_RIGHT_BRACE,                         // Represents right brace }
        TOKEN_LEFT_BRACE,                          // Represents left brace {
        TOKEN_RIGHT_BRACKET,                       // Represents right bracket ]
        TOKEN_LEFT_BRACKET,                        // Represents left bracket [
        TOKEN_KEYWORD,                             // Represents pre-defined keywords
        TOKEN_PRIMARY_TYPE,                        // Represents primary data types
        TOKEN_END_OF_FILE                          // Represents EOF of the opened file
    };
}

#endif