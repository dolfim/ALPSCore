/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                                 *
 * ALPS Project: Algorithms and Libraries for Physics Simulations                  *
 *                                                                                 *
 * ALPS Libraries                                                                  *
 *                                                                                 *
 * Copyright (C) 2010 - 2011 by Lukas Gamper <gamperl@gmail.com>                   *
 *                              Matthias Troyer <troyer@comp-phys.org>             *
 *                                                                                 *
 * This software is part of the ALPS libraries, published under the ALPS           *
 * Library License; you can use, redistribute it and/or modify it under            *
 * the terms of the license, either version 1 or (at your option) any later        *
 * version.                                                                        *
 *                                                                                 *
 * You should have received a copy of the ALPS Library License along with          *
 * the ALPS Libraries; see the file LICENSE.txt. If not, the license is also       *
 * available from http://alps.comp-phys.org/.                                      *
 *                                                                                 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        *
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT       *
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE       *
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,     *
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER     *
 * DEALINGS IN THE SOFTWARE.                                                       *
 *                                                                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define PY_ARRAY_UNIQUE_SYMBOL pyngsobservable_PyArrayHandle

#include <alps/ngs/hdf5.hpp>
#include <alps/ngs/hdf5/complex.hpp>
#include <alps/ngs/mcobservable.hpp>

#include <alps/ngs/boost_python.hpp>
#include <alps/ngs/lib/numpy_import.ipp>

#include <valarray>

namespace alps {
    namespace detail {

        void observable_append(alps::mcobservable & self, boost::python::object const & data) {
            import_numpy();
            if (false);
            #define NGS_PYTHON_HDF5_CHECK_SCALAR(N)                                                                                                 \
                else if (std::string(data.ptr()->ob_type->tp_name) == N)                                                                            \
                    self << boost::python::extract< double >(data)();
            NGS_PYTHON_HDF5_CHECK_SCALAR("int")
            NGS_PYTHON_HDF5_CHECK_SCALAR("long")
            NGS_PYTHON_HDF5_CHECK_SCALAR("float")
            NGS_PYTHON_HDF5_CHECK_SCALAR("numpy.float64")
            else if (std::string(data.ptr()->ob_type->tp_name) == "numpy.ndarray" && PyArray_Check(data.ptr())) {
                if (!PyArray_ISCONTIGUOUS(data.ptr()))
                    throw std::runtime_error("numpy array is not continous");
                else if (!PyArray_ISNOTSWAPPED(data.ptr()))
                    throw std::runtime_error("numpy array is not native");
                self << std::valarray< double >(static_cast< double const *>(PyArray_DATA(data.ptr())), *PyArray_DIMS(data.ptr()));
            } else
                throw std::runtime_error("unsupported type");
        }

        void observable_load(alps::mcobservable & self, alps::hdf5::archive & ar, std::string const & path) {
            std::string current = ar.get_context();
            ar.set_context(path);
            self.load(ar);
            ar.set_context(current);
        }

    }
}

BOOST_PYTHON_MODULE(pyngsobservable_c) {

    boost::python::class_<alps::mcobservable>(
        "observable",
        boost::python::no_init
    )
        .def("append", &alps::detail::observable_append)
        .def("merge", &alps::mcobservable::merge)
        .def("save", &alps::mcobservable::save)
        .def("load", &alps::detail::observable_load)
    ;

}

