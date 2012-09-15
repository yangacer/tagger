cmake_minimum_required (VERSION 2.8)
project (tagger)

include_directories ( /usr/local/include )
link_directories ( /usr/local/lib /usr/local/lib/gcc47 )

add_executable ( test main.cpp tagger.cpp)
