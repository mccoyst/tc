/** @file tc.h
* General-purpose declarations for tc.
* @author Steve McCoy
*/

/** Return word's token value, if it's a keyword
* -1 otherwise
*/
extern int keyval(const char *word);

extern bool dump_before, /**< flag that signals the AST should be dumped before analysis */
	dump_after,	/**< flag that signals the AST should be dumped after analysis */
	dump_table,	/**< flag that signals the symbol table should be dumped when changed */
  dump_classes, /**< signals class table printing */
  optimizing, /**< true iff optimization has been selected */
  warning; /**< true iff warnings have been enabled */
extern int yydebug;	/**< flag that signals debug parsing message should be printed */
const unsigned int LargestInt = 2147483648u;	/**< the largest negative int value in T */
