file(GLOB srcs "*.cpp")
#   if (ENABLE_PCH)
#      add_pch_rule("core/core.h" srcs_core)
#   endif()

set(srcs_unit_all  ${srcs}  PARENT_SCOPE)
