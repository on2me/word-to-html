# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\word-to-html_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\word-to-html_autogen.dir\\ParseCache.txt"
  "word-to-html_autogen"
  )
endif()
