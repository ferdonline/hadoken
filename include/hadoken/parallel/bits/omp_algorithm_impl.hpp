/**
 * Copyright (c) 2016, Adrien Devresse <adrien.devresse@epfl.ch>
 *
 * Boost Software License - Version 1.0
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
*
*/
#ifndef _HADOKEN_OMP_ALGORITHM_BITS_HPP_
#define _HADOKEN_OMP_ALGORITHM_BITS_HPP_

#include <type_traits>
#include <iterator>
#include <stdexcept>
#include <cstdint>


#include <omp.h>

#include <hadoken/parallel/algorithm.hpp>
#include <hadoken/utility/range.hpp>

#include <hadoken/parallel/bits/parallel_algorithm_generics.hpp>
#include <hadoken/parallel/bits/parallel_none_any_all_generic.hpp>
#include <hadoken/parallel/bits/parallel_transform_generic.hpp>
#include <hadoken/parallel/bits/parallel_sort_generic.hpp>
#include <hadoken/parallel/bits/parallel_numeric_generic.hpp>


namespace hadoken{


namespace parallel{


class sequential_execution_policy;
class parallel_execution_policy;
class parallel_vector_execution_policy;



namespace detail{



/// for_each algorithm
template<typename Iterator, typename Function>
inline void _omp_parallel_for_range(Iterator begin_it, Iterator end_it, Function fun){
    range<Iterator> global_range(begin_it, end_it);

    #pragma omp parallel
    {
       int id = omp_get_thread_num();
       int num_thread = omp_get_num_threads();
       range<Iterator> my_range = take_splice(global_range, id, num_thread);
       fun(my_range.begin(), my_range.end());
    }
}



} // detail



/// for_each algorithm
template<typename ExecPolicy, typename Iterator, typename RangeFunction>
inline void for_range(ExecPolicy && policy, Iterator begin_it, Iterator end_it, RangeFunction fun){
    if( detail::is_parallel_policy(policy) ){
        detail::_omp_parallel_for_range(begin_it, end_it, fun);
        return;
    }

   fun(begin_it, end_it);
}

/// parallel count_if algorithm
template< class ExecutionPolicy, class InputIterator, class UnaryPredicate >
typename std::iterator_traits<InputIterator>::difference_type
    count_if( ExecutionPolicy&& policy, InputIterator first, InputIterator last, UnaryPredicate p ){

    typedef typename std::iterator_traits<InputIterator>::difference_type counter_type;

    static_assert(std::is_same< typename std::iterator_traits<InputIterator>::iterator_category, std::random_access_iterator_tag>::value , "parallel::count requires random_access_iterator");

    if( detail::is_parallel_policy(policy) ){

        const std::size_t nelems = std::distance(first, last);
        std::uint64_t counter = 0;

        #pragma omp parallel reduction(+:counter)
        {
           int id = omp_get_thread_num();
           int num_thread = omp_get_num_threads();

           std::size_t nelem_per_slice = nelems / num_thread;

           InputIterator my_begin = first + (id * nelem_per_slice);
           InputIterator my_end = ( ((id +1) == num_thread) ? (last): (my_begin + nelem_per_slice));

           counter += std::count_if(my_begin, my_end, p);
        }

        return counter_type(counter);
    }else{
        return std::count_if(first, last, p);
    }
}

/// parallel count algorithm
template< class ExecutionPolicy, class InputIterator, class T >
typename std::iterator_traits<InputIterator>::difference_type
    count( ExecutionPolicy&& policy, InputIterator first, InputIterator last, const T &value ){
    using value_type = typename std::iterator_traits<InputIterator>::value_type;

    return count_if(std::move(policy),
                    first, last,
                    [&value](const value_type & v){
                        return (v == static_cast<value_type>(value));
                    }
    );

}





} // parallel



} // hadoken


#endif
