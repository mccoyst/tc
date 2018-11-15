#ifndef _IDENTIFIER_HEADER_
#define _IDENTIFIER_HEADER_

/** @file Ident.h
* Definition of the Ident class.
* @author Steve McCoy
*/

#include <string>
#include <iosfwd>

/** An identifer string. A pool is used so that identical identifiers can share
* the same string, instead of copies.
*/
class Ident{
	const std::string *id;
public:
	/** Copy constructor */
	Ident(const Ident& id);
	/** If id is in the pool, it won't be copied. */
	Ident(const std::string& id);
	/** If id is in the pool, it's deleted. */
	Ident(std::string *id);
	/** Destructor. @todo Decide if it should deleted the referenced string
	* if this is the last reference.
	*/
	~Ident();

	/** Copy assignment. */
	Ident& operator =(const Ident& id);
	/** Assignment. If id is in the pool, it won't be copied. */
	Ident& operator =(const std::string& id);

  /** The string representation of the identifier. */
  const std::string& str() const;

	/** True iff the identifers are the same. */
	friend bool operator ==(const Ident& l, const Ident& r);
	/** True iff the identifers are not the same. */
	friend bool operator !=(const Ident& l, const Ident& r);
	/** True iff l is lexicographically before r. */
	friend bool operator <(const Ident& l, const Ident& r);
	/** True iff l is lexicographically after r. */
	friend bool operator >(const Ident& l, const Ident& r);
	/** True iff l and r satisfy == or < . */
	friend bool operator <=(const Ident& l, const Ident& r);
	/** True iff l and r satisfy == or > . */
	friend bool operator >=(const Ident& l, const Ident& r);

	/** Sends the string represented by id to os. */
	friend std::ostream& operator <<(std::ostream& os, const Ident& id);
	/** Sends a string (delimited as with std::string) from is to id. If that string
	* is in the pool, it will simply be discarded.
	*/
	friend std::istream& operator >>(std::istream& is, Ident& id);
};

#endif
