idf_component_register(
    SRCS "encoder.cpp"
    INCLUDE_DIRS "include"
    REQUIRES driver esp_timer
)

set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 20)
