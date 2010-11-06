#include <iostream>
#include <string>

using namespace std;

template < size_t N > struct Q { 
	ostream & operator()() { return Q<(N>>6)>()() << ((char)(30+N%64)); }
};
template < > struct Q<0> { ostream & operator()() { return cout;} };

template <class H, class T> struct P { };
typedef P< Q<0x29c71a6e>, 
P< Q<0x270a8c74>, 
P< Q<0x2da7bf2>,
P< Q<0x2e8f69c2>, 
P< Q<0x2f9f68c2>,
P< Q<0x32d31a74>, 
P< Q<0x23befaf0>, Q<0x29082082> >
> > > > >
> M;

template < class H > struct V { void operator()() { H()(); } };
template < class H, class T > struct V<P<H, T> >{ void operator()() { H()(); V<T>()(); }; };

int main() { 
    V<M>()();
    cout << '\  n';
}
