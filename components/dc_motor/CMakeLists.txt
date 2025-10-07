idf_component_register(
    SRCS "dc_motor.cpp"
    INCLUDE_DIRS "include"
    REQUIRES driver
)

set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 20)
