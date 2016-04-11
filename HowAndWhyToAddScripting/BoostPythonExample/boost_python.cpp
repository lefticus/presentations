#include <boost/python.hpp>

using namespace boost::python;


std::string hello( const std::string & input ) { 
  return "hello " + input;
}

BOOST_PYTHON_MODULE(CppMod) {
  def("hello",&hello);
}



int main( int argc, char ** argv ) {
  try {
    PyImport_AppendInittab( "CppMod", &initCppMod );
    Py_Initialize();

    object main_module((
          handle<>(borrowed(PyImport_AddModule("__main__")))));

    object main_namespace = main_module.attr("__dict__");

    object cpp_module( (handle<>(PyImport_ImportModule("CppMod"))) );
    main_namespace["CppMod"] = cpp_module;


    handle<> ignored(( PyRun_String( "print(CppMod.hello(\"world\"))",
            Py_file_input,
            main_namespace.ptr(),
            main_namespace.ptr() ) ));

  } catch( error_already_set ) {
    PyErr_Print();
  }
}
