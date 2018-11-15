/*Steve McCoy
* T runtime system
*/

#include <cstdlib>
#include <algorithm>
#include <iostream>
using namespace std;

typedef unsigned int uint;
enum{ Base_VMT, Length, Element_VMT, Dimensions, First_elem };

extern "C"
{
	void _TLANGUAGE_RUNTIME_outint(int i){
		cout << i << '\n';
	}

  inline void _TRUNTIME_error(const char *msg, uint line){
    cout << "ERROR: " << msg << " (line " << line << ")\n";
    exit(1);
  }

	void _TLANGUAGE_RUNTIME_ERROR_divzero(unsigned int n){
		_TRUNTIME_error("Divide by zero",n);
	}

	void _TRUNTIME_cast(unsigned int n, const int *base, const int *derived){
    const int *c = derived;
    if(!c) return;
    while(c){
      if(c == base) return;
      c = reinterpret_cast<const int*>(*c);
    }
    _TRUNTIME_error("Invalid cast", n);
	}

	extern const int *array_vmt;

  void *_TRUNTIME_new(uint length, uint n){
    void *obj = calloc(length, 4);
    if(!obj) _TRUNTIME_error("Out of memory", n);
    return obj;
  }

  void _TRUNTIME_check_null(unsigned int n, int *obj){
    if(!obj) _TRUNTIME_error("Null reference",n);
  }

  void _TRUNTIME_rargs(int n){
    int *args = 1+&n;
    std::reverse(args,args+n);
  }

  uint _TRUNTIME_new_array(uint line, uint elem_vmt, uint dims, uint nexprs){
    int *expr = reinterpret_cast<int*>(&nexprs + nexprs);
    struct{
      uint line, evmt; // wrap recursion in a struct, since these 2 never change
      uint operator()(uint dims, uint nexprs, int *expr){
        int length = *expr;
        if(length < 0) _TRUNTIME_error("Negative array size", line);
        uint *arr = reinterpret_cast<uint*>(_TRUNTIME_new(4 + length, line));
        arr[Base_VMT] = reinterpret_cast<uint>(array_vmt);
        arr[Length] = length;
        arr[Element_VMT] = evmt;
        arr[Dimensions] = dims;
        if(nexprs > 1) for(uint *sa = arr+First_elem; sa != (arr+First_elem+length); ++sa)
          *sa = (*this)(dims-1,nexprs-1,expr-1);
        return reinterpret_cast<uint>(arr);
      }
    } new_subarray = { line, elem_vmt };

    return new_subarray(dims+nexprs,nexprs,expr);
  }

  int *_TRUNTIME_array_access(uint line, int index, int *arr){
    _TRUNTIME_check_null(line, arr);
    if(index < 0 or index >= arr[Length]) _TRUNTIME_error("Index out of bounds",line);
    return &arr[First_elem + index];
  }

  void _TRUNTIME_array_cast(uint line, const int *element, int dims, const int *source){
    if(!source) return;
    if(source[Base_VMT] == reinterpret_cast<int>(array_vmt)){
      if(source[Dimensions] != dims) _TRUNTIME_error("Invalid cast", line);
      if(source[Element_VMT] == 0 and element == 0) return;
      _TRUNTIME_cast(line, element, reinterpret_cast<const int*>(source[Element_VMT]));
      return;
    }
    _TRUNTIME_error("Invalid cast", line);
  }

  inline void _TRUNTIME_cast2(unsigned int n, const int *base, const int *derived){
    const int *c = derived;
    if(!c) return;
    while(c){
      if(c == base) return;
      c = reinterpret_cast<const int*>(*c);
    }
    _TRUNTIME_error("Invalid array assignment", n);
  }

  int *_TRUNTIME_array_insert(uint line, const int *rhs, int index, int *arr){
    int *elem = _TRUNTIME_array_access(line,index,arr);
    if(arr[Element_VMT])  // ref element
      _TRUNTIME_cast2(line, reinterpret_cast<const int*>(arr[Element_VMT]), rhs);
    *elem = reinterpret_cast<int>(rhs);
    return elem;
  }
}
