
macro (ugdk_setup_package package_name)
  include (cmake/${package_name}.cmake)
endmacro (ugdk_setup_package package_name)

set (UGDK_LIBRARIES       "")
set (UGDK_LIBRARIES_DEBUG "")

