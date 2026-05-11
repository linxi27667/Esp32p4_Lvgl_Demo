#ifndef UI_H
#define UI_H

#include "lvgl.h"
#include "core/ui_app.h"
#include "core/ui_styles.h"
#include "core/ui_events.h"
#include "fonts/ui_font.h"
#include "services/ui_i18n.h"
#include "services/ui_icons.h"
#include "model/garden_model.h"

#define UI_HEADER_HEIGHT 56
#define UI_TAB_HEIGHT   52

void ui_init(void);

#endif
