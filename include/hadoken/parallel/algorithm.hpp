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
#ifndef _HADOKEN_PARALLEL_ALGORITHM_HPP_
#define _HADOKEN_PARALLEL_ALGORITHM_HPP_

#include <algorithm>



#ifdef HADOKEN_PARALLEL_USE_TBB

#include <hadoken/parallel/bits/tbb_algorithm_impl.hpp>

#else

#include <hadoken/parallel/bits/cxx11_thread_algorithm_impl.hpp>

#endif

namespace hadoken{


namespace parallel{


/// sequential execution, no parallelism
class sequential_execution_policy{};

/// parallel execution allowed
class parallel_execution_policy{};

/// parallel execution allowed, vector execution allowed
class parallel_vector_execution_policy{};


/// constexpr for sequential execution
constexpr sequential_execution_policy seq{};

/// constexpr for parallel execution
constexpr sequential_execution_policy par{};

/// constexpr for parallel vector execution
constexpr sequential_execution_policy par_vec{};



/// for_each algorithm with execution specifier
template<typename ExecPolicy, typename Iterator, typename Function>
inline Function for_each(ExecPolicy && policy, Iterator begin_it, Iterator end_it, Function fun);


} // parallel



} // hadoken



#endif
