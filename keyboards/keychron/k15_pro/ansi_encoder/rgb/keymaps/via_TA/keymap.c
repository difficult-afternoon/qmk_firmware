

#include QMK_KEYBOARD_H
#define LAN_USER "H842923"
#define LAN_PASS ""



// Tap Dance - Begin
	//Tap Dance Declarations
	enum {
	  TD_ESC_CAPS = 0
	  ,TD_ALTTAB_CTRL // 1. Single tap = Alt_Tab, Hold = Ctrl
	  ,TD_CopyPaste // 2. Single tap = Ctrl + V, Double tap = Ctrl + Shift + V (Copy value), Triple = Ctrl + Alt + V; Hold = Ctrl + C
	  ,TD_Password // 3. Single: LAN ID Password then enter, Double = LAN ID H842923 then tab, Hold = Ctrl+Alt_Del
	  ,TD_Replace_Bold // 4. Single: Ctrl+F, Double = Ctrl+H, Hold = Ctrl+B
	  ,TD_Undo_Ctrl_A // 5. Single: Ctrl+Z, Double = Ctrl+Y, Hold = Ctrl+A
	  ,TD_PyHotkey // 6. Single: Ctrl+Alt+1, Double = Ctrl+Alt+2, Triple = Ctrl+Alt+3
	};

	// Define a type containing as many tapdance states as you need
	typedef enum {
		TD_NONE
		,TD_UNKNOWN
		,TD_SINGLE_TAP
		,TD_SINGLE_HOLD
		,TD_DOUBLE_TAP
		,TD_DOUBLE_HOLD
		,TD_DOUBLE_SINGLE_TAP // Send two single taps
		,TD_TRIPLE_TAP
		,TD_TRIPLE_HOLD
	} td_state_t;
	
	typedef struct {
    bool is_press_action;
    td_state_t state;
	} td_tap_t;
	
	// Create a global instance of the tapdance state type
	static td_state_t td_state;

	// Declare your tapdance functions:

	// Function to determine the current tapdance state
	td_state_t cur_dance(tap_dance_state_t *state);

	// `finished` and `reset` functions for each tapdance keycode
	void ALTTAB_CTRL_finished(tap_dance_state_t *state, void *user_data);
	void ALTTAB_CTRL_reset(tap_dance_state_t *state, void *user_data);

	/* Return an integer that corresponds to what kind of tap dance should be executed.
	 *
	 * How to figure out tap dance state: interrupted and pressed.
	 *
	 * Interrupted: If the state of a dance is "interrupted", that means that another key has been hit
	 *  under the tapping term. This is typically indicitive that you are trying to "tap" the key.
	 *
	 * Pressed: Whether or not the key is still being pressed. If this value is true, that means the tapping term
	 *  has ended, but the key is still being pressed down. This generally means the key is being "held".
	 *
	 * One thing that is currenlty not possible with qmk software in regards to tap dance is to mimic the "permissive hold"
	 *  feature. In general, advanced tap dances do not work well if they are used with commonly typed letters.
	 *  For example "A". Tap dances are best used on non-letter keys that are not hit while typing letters.
	 *
	 * Good places to put an advanced tap dance:
	 *  z,q,x,j,k,v,b, any function key, home/end, comma, semi-colon
	 *
	 * Criteria for "good placement" of a tap dance key:
	 *  Not a key that is hit frequently in a sentence
	 *  Not a key that is used frequently to double tap, for example 'tab' is often double tapped in a terminal, or
	 *    in a web form. So 'tab' would be a poor choice for a tap dance.
	 *  Letters used in common words as a double. For example 'p' in 'pepper'. If a tap dance function existed on the
	 *    letter 'p', the word 'pepper' would be quite frustating to type.
	 *
	 * For the third point, there does exist the 'TD_DOUBLE_SINGLE_TAP', however this is not fully tested
	 *
	 */
	td_state_t cur_dance(tap_dance_state_t *state) {
		if (state->count == 1) {
			if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
			// Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
			else return TD_SINGLE_HOLD;
		} else if (state->count == 2) {
			// TD_DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
			// action when hitting 'pp'. Suggested use case for this return value is when you want to send two
			// keystrokes of the key, and not the 'double tap' action/macro.
			if (state->interrupted) return TD_DOUBLE_SINGLE_TAP;
			else if (state->pressed) return TD_DOUBLE_HOLD;
			else return TD_DOUBLE_TAP;
		}

		// Assumes no one is trying to type the same letter three times (at least not quickly).
		// If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
		// an exception here to return a 'TD_TRIPLE_SINGLE_TAP', and define that enum just like 'TD_DOUBLE_SINGLE_TAP'
		if (state->count == 3) {
			if (state->interrupted || !state->pressed) return TD_TRIPLE_TAP;
			else return TD_TRIPLE_HOLD;
		} else return TD_UNKNOWN;
	}

	// Handle the possible states for each tapdance keycode you define:

	void ALTTAB_CTRL_finished(tap_dance_state_t *state, void *user_data) {
		td_state = cur_dance(state);
		switch (td_state) {
			case TD_SINGLE_TAP:
				tap_code16(LALT(KC_TAB));
				break;
			case TD_SINGLE_HOLD:
				register_mods(MOD_BIT(KC_LCTL)); // For a layer-tap key, use `layer_on(_MY_LAYER)` here
				break;
			default:
				break;
		}
	}

	void ALTTAB_CTRL_reset(tap_dance_state_t *state, void *user_data) {
		switch (td_state) {
			case TD_SINGLE_HOLD:
				unregister_mods(MOD_BIT(KC_LCTL)); // For a layer-tap key, use `layer_off(_MY_LAYER)` here
				break;
			default:
				break;
		}
	}
	
	
	// Pair 2: CopyPaste
	void CopyPaste_finished(tap_dance_state_t *state, void *user_data) {
		td_state = cur_dance(state);
		switch (td_state) {
			case TD_SINGLE_TAP:
				tap_code16(LCTL(KC_V));
				reset_tap_dance(state);
				break;
			case TD_DOUBLE_TAP:
				tap_code16(LSFT(LCTL(KC_V)));
				reset_tap_dance(state);
				break;
			case TD_TRIPLE_TAP:
				tap_code16(LCTL(LALT(KC_V)));
				reset_tap_dance(state);
				break;
			case TD_SINGLE_HOLD:
				tap_code16(LCTL(KC_C));
				reset_tap_dance(state);
				break;
			default:
				break;
		}
	}
	
	// Pair 3: Password
	void Password_finished(tap_dance_state_t *state, void *user_data) {
		td_state = cur_dance(state);
		switch (td_state) {
			case TD_SINGLE_TAP:
				SEND_STRING(LAN_PASS); wait_ms(300);
				tap_code16(KC_ENT);
				reset_tap_dance(state);
				break;
			case TD_DOUBLE_TAP:
				SEND_STRING(LAN_USER); wait_ms(300);
				tap_code16(KC_TAB);
				reset_tap_dance(state);
				break;
			case TD_SINGLE_HOLD:
				tap_code16(LCTL(LALT((KC_DEL))));
				break;
			default:
				break;
		}
	}

	// TD 4: Replace_Bold
	void Replace_Bold_finished(tap_dance_state_t *state, void *user_data) {
		td_state = cur_dance(state);
		switch (td_state) {
			case TD_SINGLE_TAP:
				tap_code16(LCTL(KC_F));
				reset_tap_dance(state);
				break;
			case TD_DOUBLE_TAP:
				tap_code16(LCTL(KC_H));
				reset_tap_dance(state);
				break;
			case TD_SINGLE_HOLD:
				tap_code16(LCTL(KC_B));
				reset_tap_dance(state);
				break;
			default:
				break;
		}
	}
	
	// TD 5: Undo_Ctrl_A
	void Undo_Ctrl_A_finished(tap_dance_state_t *state, void *user_data) {
		td_state = cur_dance(state);
		switch (td_state) {
			case TD_SINGLE_TAP:
				tap_code16(LCTL(KC_Z));
				reset_tap_dance(state);
				break;
			case TD_DOUBLE_TAP:
				tap_code16(LCTL(KC_Y));
				reset_tap_dance(state);
				break;
			case TD_SINGLE_HOLD:
				tap_code16(LCTL(KC_A));
				reset_tap_dance(state);
				break;
			default:
				break;
		}
	}
	
	// TD 6: PyHotkey
	void PyHotkey_finished(tap_dance_state_t *state, void *user_data) {
		td_state = cur_dance(state);
		switch (td_state) {
			case TD_SINGLE_TAP:
				tap_code16(LCTL(LALT(KC_1)));
				reset_tap_dance(state);
				break;
			case TD_DOUBLE_TAP:
				tap_code16(LCTL(LALT(KC_2)));
				reset_tap_dance(state);
				break;
			default:
				break;
		}
	}
	
	//Tap Dance Definitions
	tap_dance_action_t tap_dance_actions[] = {
	  [TD_ESC_CAPS]  = ACTION_TAP_DANCE_DOUBLE(KC_ESC, KC_CAPS)
	  ,[TD_ALTTAB_CTRL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ALTTAB_CTRL_finished, ALTTAB_CTRL_reset)
	  ,[TD_CopyPaste] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, CopyPaste_finished, NULL)
	  ,[TD_Password] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, Password_finished, NULL)
	  ,[TD_Replace_Bold] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, Replace_Bold_finished, NULL)
	  ,[TD_Undo_Ctrl_A] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, Undo_Ctrl_A_finished, NULL)
	  ,[TD_PyHotkey] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, PyHotkey_finished, NULL)
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
        KC_MUTE,    		KC_ESC,   			KC_F2,	KC_F4,    KC_F5,    KC_F9,    KC_F3,     KC_F6,    KC_F7,    KC_F8,    KC_F1,    KC_F10,   KC_F11,	TD(TD_PyHotkey),		TD(TD_Password),	KC_END,
        TD(TD_CopyPaste),	KC_DEL,   			KC_1,	KC_2,     KC_3,     KC_4,     KC_5,      KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,	KC_EQL,					KC_BSPC,			KC_PGUP,
        TD(TD_Replace_Bold),KC_TAB,   			KC_Q,	KC_W,     KC_E,     KC_R,     KC_T,      KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,	KC_RBRC,				KC_BSLS,			KC_PGDN,
        TD(TD_Undo_Ctrl_A), TD(TD_ALTTAB_CTRL),	KC_A,	KC_S,     KC_D,     KC_F,     KC_G,      KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,							KC_ENT,				KC_HOME,
        MC_4,				KC_LSFT,            KC_Z,	KC_X,     KC_C,     KC_V,      KC_B,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,	KC_RSFT,				KC_UP,
        KC_GRV,				KC_ENT,KC_LWIN,		KC_LALT,  KC_SPC,  MO(WIN_FN),           KC_SPC,             KC_RALT,            KC_RCTL,					KC_LEFT,				KC_DOWN,			KC_RGHT),

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
    [WIN_BASE] = { ENCODER_CCW_CW(KC_PGDN, KC_PGUP) },
    [WIN_FN]   = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI) },
};
#endif // ENCODER_MAP_ENABLE
