#ifndef _NODE_DOT_H_
#define _NODE_DOT_H_

/*Steve McCoy
*/

#include <string>

/** Just to save typing. */
typedef unsigned int uint;

/** Just messin' around */
namespace Severity{
  /** Error report severity level. */
  enum Severity{
    Warning,    /**< Non-severe and compilable */
    Problem,    /**< Severe and non-compilable, nothrow */
    Error,      /**< Severe and non-compilable, throws SyntaxError */
    NLevels     /**< Number of servity levels */
  };
}

/** Represents a syntax error.
*/
struct SyntaxError{};

/** All Abstract Syntax Tree classes are Nodes.
* All Nodes have an associated line number.
*/
class Node{
  uint lineno;
  static uint nerrors;
public:
  /** Sets Node's line to the line number on which it was parsed and sets status to true. */
  Node();
  /** AST Nodes should delete any subtrees that they own,
  * just in case some resource other than memory needs managed.
  */
  virtual ~Node();
  /** AST Nodes should dump any subtrees in addition to themselves.
  * The dump goes to standard error.
  */
  virtual void dump() const = 0;
  /** AST Nodes should analyze any subtrees in addition to themselves.
  * The dump goes to standard error.
  */
  virtual Node *analyze() = 0;
  /** AST Nodes should encode any subtrees in addition to themselves.
  * The dump goes to standard output.
  */
  virtual void encode() const = 0;
  /** Returns the line number of the source code where the Node was parsed. */
  uint line() const;
  /** Prints error message to standard error with line number.
  * Throws SyntaxError if s = Severity::Error.
  */
  void error(const char *msg, Severity::Severity s = Severity::Error) throw(SyntaxError);
  /** Prints error message to standard error with line number.
  * Throws SyntaxError if s = Severity::Error.
  */
  void error(const std::string& msg, Severity::Severity s = Severity::Error) throw(SyntaxError);
  /** Returns the global error count. */
  static uint errors();
};

#endif
