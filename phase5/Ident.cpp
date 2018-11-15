/*Steve McCoy
*/

#include "Ident.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <set>
using namespace std;

namespace{
	/* should go in utility.h */
	template<class PtrTp>
	struct ptr_less : public binary_function<PtrTp, PtrTp, bool>{
		bool operator ()(const PtrTp l, const PtrTp r){
			return *l < *r;
		}
	};

	typedef set<const string*, ptr_less<const string*> > IdSet;

	IdSet& idents(){
		static IdSet _;
		return _;
	}
}

void print_idents(){
	cerr << "There are " << idents().size() << " identifiers in the program\n";
}

Ident::Ident(const Ident& i)
	: id(i.id){
}

Ident::Ident(const std::string& s){
	*this = s;
}

Ident::Ident(std::string *s){
  IdSet::const_iterator it = idents().find(s);
  if(it != idents().end()){
    id = *it;
    delete s;
  }else id = *(idents().insert(s).first);
}

Ident::~Ident(){
}

Ident& Ident::operator =(const Ident& i){
	id = i.id;
	return *this;
}

Ident& Ident::operator =(const std::string& s){
	IdSet::const_iterator it = idents().find(&s);
	if(it != idents().end()) id = *it;
	else id = *(idents().insert(new string(s)).first); // first is a set iterator
	return *this;
}

const std::string& Ident::str() const{
  return *id;
}

bool operator ==(const Ident& l, const Ident& r){
	return *l.id == *r.id;
}
bool operator !=(const Ident& l, const Ident& r){
	return *l.id != *r.id;
}
bool operator <(const Ident& l, const Ident& r){
	return *l.id < *r.id;
}
bool operator >(const Ident& l, const Ident& r){
	return *l.id > *r.id;
}
bool operator <=(const Ident& l, const Ident& r){
	return *l.id <= *r.id;
}
bool operator >=(const Ident& l, const Ident& r){
	return *l.id >= *r.id;
}

std::ostream& operator <<(std::ostream& os, const Ident& id){
	return os << *id.id;
}
std::istream& operator >>(std::istream& is, Ident& id){
	string s;
	is >> s;
	id = s;
	return is;
}
