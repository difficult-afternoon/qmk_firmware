/* Copyright 2023 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

// Tap Dance - Begin
	//Tap Dance Declarations
	enum {
	  TD_ESC_CAPS = 0
	  ,TD_ALTTAB_CTRL // Our example key: `LALT` when held, `(` when tapped. Add additional keycodes for each tapdance.
	};

	// Define a type containing as many tapdance states as you need
	typedef enum {
		TD_SINGLE_TAP
		,TD_SINGLE_HOLD
		,TD_DOUBLE_SINGLE_TAP
	} td_state_t;

	// Create a global instance of the tapdance state type
	static td_state_t td_state;

	// Declare your tapdance functions:

	// Function to determine the current tapdance state
	int cur_dance(tap_dance_state_t *state);

	// `finished` and `reset` functions for each tapdance keycode
	void ALTTAB_CTRL_finished(tap_dance_state_t *state, void *user_data);
	void ALTTAB_CTRL_reset(tap_dance_state_t *state, void *user_data);

	// determine the tapdance state to return
	int cur_dance (tap_dance_state_t *state) {
	  if (state->count == 1) {
		if (state->interrupted || !state->pressed) { return TD_SINGLE_TAP; }
		else { return TD_SINGLE_HOLD; }
	  }
	  if (state->count == 2) { return TD_DOUBLE_SINGLE_TAP; }
	  else { return 3; } // any number higher than the maximum state value you return above
	}
	 

	// Handle the possible states for each tapdance keycode you define:

	void ALTTAB_CTRL_finished(tap_dance_state_t *state, void *user_data) {
		td_state = cur_dance(state);
		switch (td_state) {
			case TD_SINGLE_TAP:
				register_code16(LALT(KC_TAB));
				break;
			case TD_SINGLE_HOLD:
				register_mods(MOD_BIT(KC_LCTL)); // For a layer-tap key, use `layer_on(_MY_LAYER)` here
				break;
			case TD_DOUBLE_SINGLE_TAP: // Allow nesting of 2 parens `((` within tapping term
				//tap_code16(KC_LPRN);
				//register_code16(KC_LPRN);
				break;
			default:
				break;
		}
	}

	void ALTTAB_CTRL_reset(tap_dance_state_t *state, void *user_data) {
		switch (td_state) {
			case TD_SINGLE_TAP:
				unregister_code16(LALT(KC_TAB));
				//(KC_TAB);
				break;
			case TD_SINGLE_HOLD:
				unregister_mods(MOD_BIT(KC_LCTL)); // For a layer-tap key, use `layer_off(_MY_LAYER)` here
				break;
			case TD_DOUBLE_SINGLE_TAP:
				//unregister_code16(KC_LPRN);
				break;
			default:
				break;
		}
	}

	//Tap Dance Definitions
	tap_dance_action_t tap_dance_actions[] = {
	  //Tap once for Esc, twice for Caps Lock
	  [TD_ESC_CAPS]  = ACTION_TAP_DANCE_DOUBLE(KC_ESC, KC_CAPS)
	  ,[TD_ALTTAB_CTRL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ALTTAB_CTRL_finished, ALTTAB_CTRL_reset)
	  // Other s would go here, separated by commas, if you have them
	};	
// Tap Dance - End

enum layers {
    MAC_BASE,
    MAC_FN,
    WIN_BASE,
    WIN_FN,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MAC_BASE] = LAYOUT_90_ansi(
        KC_MUTE,    KC_ESC,   KC_BRID,  KC_BRIU,  KC_MICT,  KC_LAPA,  RGB_VAD,   RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_INS,             KC_DEL,
        MC_1,       KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,      KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_PGUP,
        MC_2,       KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,      KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,            KC_PGDN,
        MC_3,       KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,      KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,             KC_HOME,
        MC_4,       KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,      KC_B,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT,  KC_UP,
        MC_5,       KC_LCTL,  KC_LOPTN,           KC_LCMMD, KC_SPC,  MO(MAC_FN),           KC_SPC,             KC_RCMMD,           KC_RCTL,            KC_LEFT,  KC_DOWN,  KC_RGHT),

    [MAC_FN] = LAYOUT_90_ansi(
        RGB_TOG,    _______,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,     KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   _______,            _______,
        _______,    _______,  BT_HST1,  BT_HST2,  BT_HST3,  _______,  _______,   _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,    RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,   _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,    _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,   _______,  _______,  _______,  _______,  _______,  _______,            _______,            _______,
        _______,    _______,            _______,  _______,  _______,  _______,   BAT_LVL,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,    _______,  _______,            _______,  _______,  _______,             _______,            _______,            _______,            _______,  _______,  _______),

    [WIN_BASE] = LAYOUT_90_ansi(
        KC_MUTE,    		KC_ESC,   			KC_F1,	KC_F2,    KC_F4,    KC_F5,    KC_F9,     KC_F6,    KC_F7,    KC_F8,    KC_F3,    KC_F10,   KC_F11,   KC_F12,   KC_INS,             KC_DEL,
        MC_1,       		KC_GRV,   			KC_1,	KC_2,     KC_3,     KC_4,     KC_5,      KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_PGUP,
        MC_2,       		KC_TAB,   			KC_Q,	KC_W,     KC_E,     KC_R,     KC_T,      KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,            KC_PGDN,
        MC_3,       		TD(TD_ALTTAB_CTRL),	KC_A,	KC_S,     KC_D,     KC_F,     KC_G,      KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,             KC_HOME,
        TD(TD_ESC_CAPS),	KC_LSFT,            KC_Z,	KC_X,     KC_C,     KC_V,      KC_B,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT,  KC_UP,
        MC_5,				KC_LCTL,KC_LWIN,			KC_LALT,  KC_SPC,  MO(WIN_FN),           KC_SPC,             KC_RALT,            KC_RCTL,            KC_LEFT,  KC_DOWN,  KC_RGHT),

    [WIN_FN] = LAYOUT_90_ansi(
        RGB_TOG,    _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,   RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,   KC_VOLU,  _______,            _______,
        _______,    _______,  BT_HST1,  BT_HST2,  BT_HST3,  _______,  _______,   _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,            _______,
        _______,    RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,   _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,            _______,
        _______,    _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,   _______,  _______,  _______,  _______,  _______,  _______,             _______,            _______,
        _______,    _______,            _______,  _______,  _______,  _______,   BAT_LVL,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,   _______,  _______,  _______,
        _______,    _______,  _______,            _______,  _______,  _______,             _______,            _______,            _______,             _______,  _______,  _______),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [MAC_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [MAC_FN]   = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI) },
    [WIN_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [WIN_FN]   = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI) },
};
#endif // ENCODER_MAP_ENABLE
