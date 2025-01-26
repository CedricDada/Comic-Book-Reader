# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "BookReaderView_autogen"
  "CMakeFiles\\BookReaderView_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\BookReaderView_autogen.dir\\ParseCache.txt"
  )
endif()
