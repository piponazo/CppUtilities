file(GLOB srcs "*.cpp")
file(GLOB del "test_cv.cpp")
list(REMOVE_ITEM srcs ${del})

set(srcs_unit_all  ${srcs}  PARENT_SCOPE)
