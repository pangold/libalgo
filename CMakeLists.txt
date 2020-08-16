cmake_minimum_required (VERSION 3.0)

######################################################
# Project
######################################################
set (name algo)
project (lib${name})

######################################################
# Settings
######################################################
if (WIN32)
  set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
  set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MT")
  set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif ()

set (major_version 1)
set (version ${major_version}.0.0)

option (enable_static "enable static library" ON)
option (enable_shared "enable shared library" ON)
option (enable_test   "enable test" ON)

######################################################
# Sources
######################################################
set (file_inc array.h hash_map.h heap.h list.h map.h queue.h rb_tree.h sorted_set.h stack.h)
set (file_src array.c hash_map.c heap.c list.c map.c queue.c rb_tree.c sorted_set.c stack.c)

# set output name
if (WIN32)
  set (output_name lib${name})
else ()
  set (output_name ${name})
endif ()

######################################################
# Shared library
######################################################
if (${enable_shared})
  # add library
  set (target_shared lib${name}-shared)
  add_library (${target_shared} SHARED ${file_src} ${file_inc})
  # set output name
  set_target_properties (${target_shared} PROPERTIES
    OUTPUT_NAME ${output_name}
    SOVERSION ${major_version}
    VERSION ${version})
endif ()

######################################################
# Static library
######################################################
if (${enable_static})
  # add static library
  set (target_static lib${name}-static)
  add_library (${target_static} STATIC ${file_src} ${file_inc})
  # set output name
  set_target_properties (${target_static} PROPERTIES
    OUTPUT_NAME ${output_name})
endif ()

######################################################
# Algo Test
######################################################
if (${enable_test})
  add_definitions(-DENABLE_LIST_TEST -DENABLE_ARRAY_TEST -DENABLE_HEAP_TEST -DENABLE_RBTREE_TEST)
  add_definitions(-DENABLE_SORTED_SET_TEST -DENABLE_MAP_TEST -DENABLE_HASH_MAP_TEST -DENABLE_QUEUE_TEST -DENABLE_STACK_TEST)
  # add bin program
  set (target_bin ${name}-test)
  add_executable(${target_bin} algo_test.c algo.h)
endif ()