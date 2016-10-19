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
#ifndef _HADOKEN_GEOMETRY_HPP_
#define _HADOKEN_GEOMETRY_HPP_


#include <boost/geometry/geometry.hpp>


#include <hadoken/geometry/objects/sphere.hpp>
#include <hadoken/geometry/objects/point_base.hpp>

#if __cplusplus >= 2011L


namespace hadoken{

namespace geometry{

namespace cartesian {
    // namespace alias for internal use
    namespace bg =  boost::geometry;

    /// import boost geometry for existing algorithms
    using namespace boost::geometry;



    /// object models

    /// default point type
    template<typename CoordType, std::size_t N>
    using point = point_base<CoordType, N>;

    /// default point model with type specification
    template<typename CoordType>
    using point3 = point_base<CoordType, 3>;

    /// default point in double precision
    using point3d = point3<double>;

    ///default point in simple precision
    using point3f = point3<float>;

    /// default multi_point model with type specification
    template<typename CoordType>
    using multi_point3 = model::multi_point<point3<CoordType>>;

    /// default point in double precision
    using multi_point3d = multi_point3<double>;

    ///default point in simple precision
    using multi_point3f = multi_point3<float>;

    /// default linestring
    template<typename CoordType>
    using linestring3 = model::linestring<point3<CoordType>>;

    /// default point in double precision
    using linestring3d = linestring3<double>;

    ///default point in simple precision
    using linestring3f = linestring3<float>;

    /// default sphere object
    template<typename CoordType>
    using sphere = sphere_base<point3<CoordType>, CoordType>;

    using sphere3d = sphere<double>;

    using sphere3f = sphere<float>;

    /// coordinate mapper
    template<typename CoordType>
    struct coordinate_type{

    };

    template<typename CoordType>
    struct coordinate_type<point3<CoordType>>{
            typedef CoordType type;
    };


    /// accessor x coordinate
    template<typename Point>
    typename coordinate_type<Point>::type get_x(const Point & p){
        return bg::get<0>(p);
    }

    /// accessor y coordinate
    template<typename Point>
    typename coordinate_type<Point>::type get_y(const Point & p){
        return bg::get<1>(p);
    }

    /// accessor z coordinate
    template<typename Point>
    typename coordinate_type<Point>::type get_z(const Point & p){
        return bg::get<2>(p);
    }

    //// make point
    template<typename CoordType>
    point3<CoordType> make_point(const std::array<CoordType, 3> & coord ){
        return point3<CoordType>(coord[0], coord[1], coord[2]);
    }


} // cartesian


} // geometry

} // hadoken


#endif

#include <hadoken/geometry/geometry_legacy.hpp>

#endif // _HADOKEN_GEOMETRY_HPP_
