/** @file scanner.h
* Declarations of interest to the scanner, lexdbg and lexrelease
* @author Steve McCoy
*/

/** Identifier token, also covers keywords */
extern int id_tok();
/** int literal token */
extern int int_tok();
/** one-character operator token */
extern int op1_tok();
/** two-character operator token */
extern int op2_tok();
/** token error */
extern int bad_tok();
