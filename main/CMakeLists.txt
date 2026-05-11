set(LV_DEMO_DIR ../managed_components/lvgl__lvgl/demos)
file(GLOB_RECURSE LV_DEMOS_SOURCES ${LV_DEMO_DIR}/*.c)

# Smart Garden UI sources
file(GLOB_RECURSE UI_SOURCES
    "ui/core/*.c"
    "ui/pages/*.c"
    "ui/widgets/*.c"
    "ui/fonts/*.c"
    "ui/model/*.c"
    "ui/services/*.c"
)

idf_component_register(
    SRCS main.c lvgl_adapter_init.c ${LV_DEMOS_SOURCES} ${UI_SOURCES}
    INCLUDE_DIRS . ${LV_DEMO_DIR} ui ui/core ui/pages ui/widgets ui/fonts ui/model ui/services
)

idf_component_get_property(LVGL_LIB lvgl__lvgl COMPONENT_LIB)
target_compile_options(
    ${LVGL_LIB}
    PRIVATE
        -DLV_LVGL_H_INCLUDE_SIMPLE
        -DLV_USE_DEMO_MUSIC
)
