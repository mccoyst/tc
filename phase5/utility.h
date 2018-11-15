/** @file utility.h
* Miscellaneous utility classes and functions.
* @author Steve McCoy
*/

/** Deletes the pointer argument, returns 0. Useful for transform()ing
* a collection of pointers to be deleted and nulled.
*/
template<class P>
P *Delete(P *p){
	delete p;
	return 0;
}

/** Returns dynamic_cast<B*>(f(a)). Useful when transform()ing a collection
* of polymorphic types with functions that return a wider type.
*/
template<class A, class B, class F>
class Dcast{
	const F& f;
public:
	/** Forwards func to the Dcast object.
	*/
	Dcast(const F& func) : f(func) {}
	/** Returns dynamic_cast<B*>(func(a)).
	*/
	B *operator()(A *a) { return dynamic_cast<B*>(f(a)); }
};

/** Returns a dcast for when A == B.
*/
template<class A, class F>
Dcast<A,A,F> dc_fun(const F& f){
	return Dcast<A,A,F>(f);
}
