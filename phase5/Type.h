#ifndef _TYPE_HEADER_
#define _TYPE_HEADER_

/** @file Type.h
* Definition of the abstract classes Type and Ref.
* @author Steve McCoy
*/

#include <iosfwd>
#include <string>
#include <list>
#include "Ident.h"

class CDecl;
class Ctor;
class Expr;
class Meth;

/** Abstract base class of all types's AST representations:
primitive, built-in, and user-defined. */
class Type{
public:
	virtual ~Type() {}
  /** Returns the standard string representation of the type's name */
  virtual std::string str() const = 0;
	/** Returns true iff this instance is the same type as t */
	virtual bool is(const Type *t) const = 0;

  /** Returns true if this type has any information missing from its
  definition. */
  virtual bool incomplete() const = 0;
  /** Sets the completeness of this type. */
  virtual void complete(bool c) = 0;
  /** Returns true iff this class has been defined. */
  virtual bool defined() const = 0;
  /** Sets the defined state of this class. */
  virtual void defined(bool d) = 0;

  /** Returns true iff this type is a subtype of t. */
  virtual bool inherits_from(const Type *t) const = 0;
  /** Returns true iff this type can be converted to t at compile-time. */
  virtual bool can_upcast(const Type *t) const = 0;
  /** Returns true iff this type can attempt to convert to t at runtime. */
  virtual bool can_downcast(const Type *t) const = 0;

  /** Prints the encoding of this type's VMT to standard output. */
  virtual void encode() const = 0;
  /** Returns the size of an instance of this type's fields in number of machine words. */
  virtual int words() const = 0;

  /** Adds a field to this class's field table. Returns true iff a field
  named @a id doesn't already exist. */
  virtual bool field(Ident id, Type *t) = 0;
  /** Returns the offset (in words) of field @a f for an instance
  of this type, if it's a member. Otherwise, returns -1. */
  virtual int field_offset(Ident f) const = 0;
  /** Sets the offsets of the fields for an instance of this type. */
  virtual void set_offsets() = 0;
  /** Returns the type of the field. */
  virtual const Type *field_type(Ident f) const = 0;

  /** Returns this class's superclass, 0 if it doesn't have one. */
  virtual const Type *super() const = 0;
  /** Sets this class's superclass. */
  virtual void super(Type *s) = 0;

  /** Prints the class and its members to standard error */
  virtual void dump() const = 0;

  /** Registers declaration of a subclass. */
  virtual void register_subclass(CDecl*) = 0;

  /** Flags that a custom destructor is defined for this class */
  virtual void define_destructor() = 0;
  /** Returns true iff a custom destructor is defined for this class */
  virtual bool has_destructor() const = 0;
  /** Name of the nearest class with a custom destructor */
  virtual std::string nearest_destructor() const = 0;

  /** Adds a constructor to this class. Returns true iff a constructor of the
  * same signature doesn't already exist */
  virtual bool define_constructor(Ctor*) = 0;
  /** Name of the nearest class with a default constructor */
  virtual std::string nearest_constructor() const = 0;
  /** Returns a constructor that matches the args, or 0 */
  virtual Ctor *matching_constructor(const std::list<Expr*> *args) const = 0;
  /** Fills list with all ctors that are applicable, given args */
  virtual void applicable_ctors(std::list<Ctor*>& l, const std::list<Expr*> *args) const {}
  /** True iff class has noarg ctor */
  virtual bool has_noarg() const = 0;
  /** Returns class's noarg */
  virtual Ctor *noarg() const = 0;

  /** Adds method if it isn't there already */
  virtual bool add_meth(Meth *m) = 0;
  /** Fills list with all methods that are applicable, given name and args */
  virtual void applicable_meths(
    std::list<Meth*>& l, Ident name, const std::list<Expr*> *args) const = 0;
  /** Returns the offset into the class's VMT for the method, or -1 if not a member */
  virtual int meth_offset(const Meth *m) const = 0;

  /** Returns the number of dimensions */
  virtual unsigned int dimensions() const { return 0; }
  virtual Type *element() const { return 0; }

	/** Returns the instance of the unknown type */
	static Type *unknown();
	/** Returns the instance of the int type representation */
	static Type *int_type();
	/** Returns the instance of the null literal's type representation */
	static Type *null();
	/** Returns the instance of Object's type representation. */
	static Type *object();
	/** Returns the Type of the user-defined class with the given name.
	If a class of the given name doesn't exist, then an undefined,incomplete
  type is added and returned */
	static Type *user(Ident name);
	/** Returns an array type formed from the element type and dimensions */
	static Type *array(Type *element, unsigned int dimensions);
	/** Prints the Array VMT to standard output */
	static void encode_array();

  /** Returns the current class being analyzed */
  static const Type *current();
  /** Sets the current class being analyzed */
  static void current(const Type*);

  /** Sentinel for making a class the "current" class */
  class Make_current{
    const Type *old;
  public:
    Make_current(const Type *new_cur);
    ~Make_current();
  };
};

inline std::ostream& operator <<(std::ostream& o, const Type& t){
  return o << t.str();
}

inline Type::Make_current::Make_current(const Type *t)
  : old(Type::current()){
  Type::current(t);
}

inline Type::Make_current::~Make_current(){
  Type::current(old);
}

#endif
