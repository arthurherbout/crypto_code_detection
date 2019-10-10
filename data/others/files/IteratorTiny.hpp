#ifndef __ITERATORTINY_HPP
#define __ITERATORTINY_HPP
#include <stddef.h>
namespace TinySTL
{

//tag types
struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag:public input_iterator_tag{};
struct bidirectional_iterator_tag:public forward_iterator_tag{};
struct random_access_iterator_tag:public bidirectional_iterator_tag{};


//
template<typename Category,typename	T,
		 typename Distance=ptrdiff_t,
		 typename Pointer=T*,
		 typename Reference = T& >
struct iterator
{
	typedef Category iterator_category;
	typedef T value_type;
	typedef Distance difference_type;
	typedef Pointer pointer;
	typedef Reference reference;

};




//iterator_traits
template<typename Iterator>
struct iterator_traits{
    typedef typename Iterator::value_type value_type;
    typedef typename Iterator::difference_type difference_type;
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::reference reference;
    typedef typename Iterator::iterator_category iterator_category;
};

template<typename Iterator>
struct iterator_traits<Iterator*>{
    typedef  Iterator value_type;
    typedef  ptrdiff_t difference_type;
    typedef  Iterator* pointer;
    typedef  Iterator& reference;
    typedef  random_access_iterator_tag iterator_category;

};

template<typename Iterator>
struct iterator_traits<const Iterator*>{
    typedef  Iterator value_type;
    typedef  ptrdiff_t difference_type;
    typedef  const Iterator* pointer;
    typedef  const Iterator& reference;
    typedef  random_access_iterator_tag iterator_category;

};

template<typename Iterator>
typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&)
{
	typedef typename iterator_traits<Iterator>::iterator_category category;
	return category();

}

template<typename Iterator>
typename iterator_traits<Iterator>::difference_type*
difference_type(const Iterator&)
{
	return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

template<typename Iterator>
typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&)
{
	return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

template<typename Iterator>
typename iterator_traits<Iterator>::difference_type
__distance(Iterator first,Iterator last,struct input_iterator_tag)
{
	typename iterator_traits<Iterator>::difference_type n=0;
	while(first!=last)
	{
		++first;
		++n;
	}
	return n;
}

template<typename Iterator>
typename iterator_traits<Iterator>::difference_type
__distance(Iterator first,Iterator last,struct random_access_iterator_tag)
{
	return last-first;
}


template<typename Iterator>
typename iterator_traits<Iterator>::difference_type
distance(Iterator first,Iterator last)
{
	typedef typename iterator_traits<Iterator>::iterator_category category;
	return __distance(first,last,category());
}

template<typename Iterator,typename Distance>
void __advance(Iterator &i,Distance n,struct input_iterator_tag)
{
	while(n-->0)
	{
		++i;
	}
}

template<typename Iterator,typename Distance>
void __advance(Iterator &i,Distance n,struct bidirectional_iterator_tag)
{
	if(n>0)
	{
		while(n-->0)
		{
			++i;
		}
	}
	else
	{
		while(n++<0)
		{
			--i;
		}
	}
}

template<typename Iterator,typename Distance>
void __advance(Iterator &i,Distance n,struct random_access_iterator_tag)
{
	i=i+n;
}



template<typename Iterator,typename Distance>
void advance(Iterator &i,Distance n)
{
	__advance(i,n,iterator_category(i));
}



}





#endif