idf_component_register(
    INCLUDE_DIRS "include"
)

# Force C++20 for this header-only component
set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 20)
