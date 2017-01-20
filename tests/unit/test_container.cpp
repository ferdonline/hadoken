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

#define BOOST_TEST_MODULE containerTests
#define BOOST_TEST_MAIN

#include <iostream>
#include <map>
#include <stdexcept>

#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>


#include <hadoken/containers/small_vector.hpp>

#include <hadoken/utility/range.hpp>


#include "test_helpers.hpp"

typedef boost::mpl::list<
    short int, short unsigned int,
    int, unsigned int,
    long, unsigned long,
    long long, unsigned long long,
    double, float, long double,
    std::string
 > small_vector_types;


BOOST_AUTO_TEST_CASE_TEMPLATE( small_vector_test, T, small_vector_types )
{

    using namespace hadoken::containers;

    constexpr std::size_t small_size = 16;
    const std::size_t size_push =500;

    content_generator<T> gen;

    small_vector<T, small_size> values;

    BOOST_CHECK_EQUAL(values.size(), 0);
    BOOST_CHECK_EQUAL(values.empty(), true);
    BOOST_CHECK_EQUAL(values.capacity(), 0);
    BOOST_CHECK_EQUAL(values.data(), &(*values.begin()));

    const auto begin = values.begin();
    const auto end = values.end();

    BOOST_CHECK(begin == end);
    BOOST_CHECK_EQUAL(std::distance(begin, end), 0);

    BOOST_CHECK(std::begin(values) == values.begin());
    BOOST_CHECK(std::end(values) == values.end());

    // access non existing value with boundary check
    BOOST_CHECK_THROW({ values.at(0); }, std::out_of_range);

    for(std::size_t i =0;  i < size_push; ++i ){

        // alternate emplace back and push_back
        if(i%3 == 0){
            values.emplace_back(gen(i));
         }else{
            values.push_back(gen(i));
        }

        if(i < small_size){
            //std::cout << " capa " <<  values.capacity() << " " << i << std::endl;
            BOOST_CHECK_EQUAL(values.capacity(), 0);
        }else{
            BOOST_CHECK_LE(i, values.capacity());
        }
    }

    BOOST_CHECK_EQUAL(values.size(), size_push);
    BOOST_CHECK_EQUAL(std::distance(values.begin(), values.end()), size_push);
    BOOST_CHECK_EQUAL(values.empty(), false);


    for(std::size_t i =0;  i < values.size(); ++i ){
        BOOST_CHECK_EQUAL(values[i], gen(i));
        BOOST_CHECK_EQUAL(values.at(i), gen(i));
    }
}


BOOST_AUTO_TEST_CASE( small_vector_test_unique_ptr)
{
    using namespace hadoken::containers;

    using content_type = std::unique_ptr<std::string>;

    constexpr std::size_t small_size = 16;
    const std::size_t size_push =500;

    content_generator<content_type> gen;

    small_vector<content_type, small_size> values;

    for(std::size_t i =0;  i < size_push; ++i ){

        values.emplace_back(gen(i));

        if(i < small_size){
            //std::cout << " capa " <<  values.capacity() << " " << i << std::endl;
            BOOST_CHECK_EQUAL(values.capacity(), 0);
        }else{
            BOOST_CHECK_LE(i, values.capacity());
        }
    }

    BOOST_CHECK_EQUAL(values.size(), size_push);
    BOOST_CHECK_EQUAL(std::distance(values.begin(), values.end()), size_push);
    BOOST_CHECK_EQUAL(values.empty(), false);


    for(std::size_t i =0;  i < values.size(); ++i ){
        BOOST_CHECK_EQUAL(*values[i], *gen(i));
        BOOST_CHECK_EQUAL(*values.at(i), *gen(i));
    }
}



template<typename T, typename Mod, typename Check>
void  test_check_range(T vec, size_t partition, const Mod & modifier, const Check & checker){
    using namespace hadoken;

    typedef range<typename T::iterator> range_vec;

    range_vec my_range(vec.begin(), vec.end());


    BOOST_CHECK_EQUAL(vec.size(), my_range.size());

    std::vector< range_vec> ranges = split_range(my_range, partition);
    BOOST_CHECK_EQUAL(ranges.size(), partition);
    BOOST_CHECK(ranges[0].begin() == vec.begin());
    BOOST_CHECK(ranges[partition-1].end() == vec.end());


    // test validity of all iterators
    for(typename std::vector<range_vec>::iterator it = ranges.begin(); it != ranges.end(); ++it){
        std::for_each(it->begin(), it->end(), modifier);
    }

    std::for_each(vec.begin(), vec.end(), checker);

    // test sequence
    // no range should have a difference of more than 1 elem
    std::size_t min_elem = std::numeric_limits<std::size_t>::max(), max_elem = std::numeric_limits<std::size_t>::min();
    for(typename std::vector<range_vec>::iterator it = ranges.begin(); it < ranges.end(); ++it){
        const std::size_t diff = std::distance(it->begin(), it->end());
        min_elem = std::min(min_elem, diff);
        max_elem = std::min(max_elem, diff);
    }

    BOOST_CHECK_GE(min_elem+1,max_elem);

    // create on purpose over range
    const std::size_t over_range = my_range.size() + 100;

    BOOST_CHECK_EQUAL(take_splice(my_range, over_range, over_range*2).size(), 0);


    // check matching between split and take_splice
    std::vector< range_vec> other_ranges = split_range(my_range, 200);
    for(std::size_t i = 0; i < 200; ++i){
        range<typename T::iterator> my_slice = take_splice(my_range, i, 200);
        BOOST_CHECK(my_slice == other_ranges[i]);
    }

}
