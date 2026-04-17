set(LV_DEMO_DIR ../managed_components/lvgl__lvgl/demos)
file(GLOB_RECURSE LV_DEMOS_SOURCES ${LV_DEMO_DIR}/*.c)

# UI files from SquareLine Studio (if ui/ directory exists)
file(GLOB_RECURSE UI_SOURCES "ui/*.c")

idf_component_register(
    SRCS main.c lvgl_adapter_init.c ${LV_DEMOS_SOURCES} ${UI_SOURCES}
    INCLUDE_DIRS . ${LV_DEMO_DIR} ui
)

idf_component_get_property(LVGL_LIB lvgl__lvgl COMPONENT_LIB)
target_compile_options(
    ${LVGL_LIB} 
    PRIVATE
        -DLV_LVGL_H_INCLUDE_SIMPLE
        -DLV_USE_DEMO_MUSIC
)
