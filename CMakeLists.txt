cmake_minimum_required(VERSION 2.8)


if(MSVC)
  find_program(WKHTMLTOPDF_EXECUTABLE 
    wkhtmltopdf
    PATHS "C:/Program Files/wkhtmltopdf/bin" "C:/Program Files (x86)/wkhtmltopdf/bin")
else()
  find_program(WKHTMLTOPDF_EXECUTABLE 
    wkhtmltopdf
    )
endif()

# wkhtmltopdf
execute_process(COMMAND ${WKHTMLTOPDF_EXECUTABLE} -V 
  OUTPUT_VARIABLE wkhtmltopdfversionout)

string(REGEX MATCH "([0-9]+\\.?)+" WKHTMLTOPDF_VER "${wkhtmltopdfversionout}")
string(FIND "${wkhtmltopdfversionout}" "with patched qt" WKHTMLTOPDF_PATCHEDQT )


if (WKHTMLTOPDF_VER VERSION_LESS "0.12.2.1")
  message(FATAL_ERROR "wkhtmltopdf version 0.12.2.1 or greater required")
endif()

if (WKHTMLTOPDF_PATCHEDQT LESS "0")
  message(FATAL_ERROR "wkhtmltopdf with special patchedqt build required")
endif()

# pandoc

find_program(PANDOC_EXECUTABLE pandoc)


execute_process(COMMAND ${PANDOC_EXECUTABLE} -v 
  OUTPUT_VARIABLE pandocversionout)

string(REGEX MATCH "([0-9]+\\.?)+" PANDOC_VER "${pandocversionout}")

if (PANDOC_VER VERSION_LESS "1.13.2")
  message(FATAL_ERROR "pandoc version 1.13.2 or greater required")
endif()


Function(AddPDF docName)
  set(output_name ${CMAKE_BINARY_DIR}/${docName}/${docName}.pdf)
  add_custom_command(OUTPUT ${output_name}

    COMMAND ${WKHTMLTOPDF_EXECUTABLE} -q --enable-smart-shrinking --page-size Letter --load-error-handling abort --footer-left "Copyright 2015 Jason Turner"  --footer-center "@lefticus" --footer-right "[page] / [topage]" -O Landscape -s Letter file:///${CMAKE_BINARY_DIR}/${docName}/${docName}.html?print-pdf ${CMAKE_BINARY_DIR}/${docName}/${docName}.pdf
    DEPENDS ${docName}_html
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${docName}
    )

  add_custom_target(${docName}_pdf ALL
    DEPENDS ${output_name}
    )

  if (PACKAGE_BUILT_DOCS)
    INSTALL(FILES "${output_name}" DESTINATION "./Documentation")
  endif()

EndFunction()


Function(BuildDocument docName)
  set(output_name ${CMAKE_BINARY_DIR}/${docName}/${docName}.html)

  foreach(input_name ${ARGN})
    list(APPEND input_names ${CMAKE_CURRENT_SOURCE_DIR}/${input_name})
  endforeach()

  add_custom_command(OUTPUT ${output_name}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/${docName}/reveal.js
    COMMAND ${CMAKE_COMMAND} -E make_directory   ${CMAKE_BINARY_DIR}/${docName}/reveal.js
    COMMAND ${CMAKE_COMMAND} -E make_directory   ${CMAKE_BINARY_DIR}/${docName}/reveal.js/js
    COMMAND ${CMAKE_COMMAND} -E copy_directory   ${CMAKE_CURRENT_SOURCE_DIR}/reveal.js/js ${CMAKE_BINARY_DIR}/${docName}/reveal.js/js
    COMMAND ${CMAKE_COMMAND} -E make_directory   ${CMAKE_BINARY_DIR}/${docName}/reveal.js/css
    COMMAND ${CMAKE_COMMAND} -E copy_directory   ${CMAKE_CURRENT_SOURCE_DIR}/reveal.js/css ${CMAKE_BINARY_DIR}/${docName}/reveal.js/css
    COMMAND ${CMAKE_COMMAND} -E make_directory   ${CMAKE_BINARY_DIR}/${docName}/reveal.js/lib
    COMMAND ${CMAKE_COMMAND} -E copy_directory   ${CMAKE_CURRENT_SOURCE_DIR}/reveal.js/lib ${CMAKE_BINARY_DIR}/${docName}/reveal.js/lib
    COMMAND ${CMAKE_COMMAND} -E make_directory   ${CMAKE_BINARY_DIR}/${docName}/reveal.js/plugin
    COMMAND ${CMAKE_COMMAND} -E copy_directory   ${CMAKE_CURRENT_SOURCE_DIR}/reveal.js/plugin ${CMAKE_BINARY_DIR}/${docName}/reveal.js/plugin

    COMMAND ${PANDOC_EXECUTABLE} ${input_names} -o ${output_name} --css reveal.js/css/theme/lefticus.css -f markdown_github -s -t revealjs -A ${CMAKE_CURRENT_SOURCE_DIR}/revealjs-header.html  --email-obfuscation=none
    DEPENDS ${input_names} ${CMAKE_CURRENT_SOURCE_DIR}/revealjs-header.html ${CMAKE_CURRENT_SOURCE_DIR}/reveal.js/css/theme/lefticus.css
    )

  add_custom_target(${docName}_html ALL
    DEPENDS ${output_name}
    )


  AddPDF(${docName})
EndFunction()

BuildDocument(ThinkingPortable ThinkingPortable.md)
BuildDocument(CurrentStateOfStaticAnalysis CurrentStateOfStaticAnalysis.md)
BuildDocument(CrazyEasyScriptingWithChaiScript CrazyEasyScriptingWithChaiScript.md)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/CrazyEasyScriptingWithChaiScript)
