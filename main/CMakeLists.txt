idf_component_register(
    SRCS "main.cpp"
    INCLUDE_DIRS "include"
)

# Ensure this component uses C++20
set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 20)
