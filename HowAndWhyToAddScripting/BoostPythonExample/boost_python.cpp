#include <boost/python.hpp>

std::string hello( const std::string & input ) { 
  return "hello " + input;
}

BOOST_PYTHON_MODULE(CppMod) {
  boost::python::def("hello", &hello);
}

int main() {
  try {
    PyImport_AppendInittab( "CppMod", &initCppMod );
    Py_Initialize();

    boost::python::object main_module((
          boost::python::handle<>(boost::python::borrowed(PyImport_AddModule("__main__")))));

    boost::python::object main_namespace = main_module.attr("__dict__");
    boost::python::object cpp_module( (boost::python::handle<>(PyImport_ImportModule("CppMod"))) );

    main_namespace["CppMod"] = cpp_module;

    boost::python::handle<> ignored(( PyRun_String( 
R"python(
for number in range(1000000):    
    print(CppMod.hello(\"world\"))
)python",
            Py_file_input,
            main_namespace.ptr(),
            main_namespace.ptr() ) ));
  } catch ( const boost::python::error_already_set & ) {
    PyErr_Print();
  }
}
