//
// Created by marijn on 6/27/24.
//

/*!<
 * includes
 * */
#include "app/keymap.h"


/*!<
 * config
 * */
const uint16_t PROGMEM key_map[][ROW_COUNT][COL_COUNT] = {
	/*
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┬───┐
     * │Esc│ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ - │ = │ Backsp│Hom│
     * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┼───┤
     * │ Tab │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │ [ │ ] │  \  │PgU│
     * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┼───┤
     * │ Caps │ A │ S │ D │ F │ G │ H │ J │ K │ L │ ; │ ' │  Enter │PgD│
     * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────┬───┼───┤
     * │ Shift  │ Z │ X │ C │ V │ B │ N │ M │ , │ . │ / │ Shift│ ↑ │End│
     * ├────┬───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴──┬┴──┬┴──┬───┼───┼───┤
     * │Ctrl│ GUI│ Alt│                        │Alt│ Fn│Ctl│ ← │ ↓ │ → │
     * └────┴────┴────┴────────────────────────┴───┴───┴───┴───┴───┴───┘
	 */
	[0] = {
		{ KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,     KC_9,   KC_0,     KC_MINUS,  KC_EQUAL,  KC_BSPC,   KC_HOME },
		{ KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,     KC_O,   KC_P,     KC_LBRC,   KC_RBRC,   KC_BSLS,   KC_PGUP },
		{ KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,     KC_L,   KC_SCLN,  KC_QUOTE,  0x0000,    KC_ENTER,  KC_PGDN },
		{ KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMMA, KC_DOT, KC_SLASH, KC_RSFT,   0x0000,    KC_UP,     KC_END },
		{ KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPACE,                  KC_RALT,  0/*TODO*/,      KC_RCTL,  KC_LEFT,   0x0000,    KC_DOWN,   KC_RIGHT }
	}
};