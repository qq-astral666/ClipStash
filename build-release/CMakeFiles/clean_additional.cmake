# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles/ClipStash_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/ClipStash_autogen.dir/ParseCache.txt"
  "CMakeFiles/clipstash_core_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/clipstash_core_autogen.dir/ParseCache.txt"
  "ClipStash_autogen"
  "clipstash_core_autogen"
  )
endif()
