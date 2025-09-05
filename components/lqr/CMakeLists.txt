idf_component_register(
    SRCS "lqr.cpp"
    INCLUDE_DIRS "include"
)

set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 20)
