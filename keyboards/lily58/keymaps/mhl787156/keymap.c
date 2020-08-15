#include QMK_KEYBOARD_H

#ifdef PROTOCOL_LUFA
  #include "lufa.h"
  #include "split_util.h"
#endif
#ifdef SSD1306OLED
  #include "ssd1306.h"
#endif

#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif

extern uint8_t is_master;

bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;

#define _QWERTY 0
#define _COLEMAK 1
#define _LOWER 2
#define _RAISE 3
#define _ADJUST 4
#define _FUNCTION 5

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  COLEMAK,
  LOWER,
  RAISE,
  ADJUST,
  FUNCTION,
  ALT_TAB,
  BSP_DEL
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
        [0] = LAYOUT( //QWERTY
          LT(4, KC_ESC), KC_1, KC_2, KC_3, KC_4, KC_5,                                                                KC_6, KC_7, KC_8, KC_9, KC_0, LT(4, KC_GRV),
          KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T,                                                                       KC_Y, KC_U, KC_I, KC_O, KC_P, LT(5,KC_MINS),
          LSFT_T(KC_ESC), KC_A, KC_S, KC_D, KC_F, KC_G,                                                               KC_H, KC_J, KC_K, KC_L, KC_SCLN, RSFT_T(KC_QUOT),
          OSM(MOD_LCTL), KC_Z, KC_X, KC_C, KC_V, KC_B,              TG(6),                TG(8),                      KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, RCTL_T(KC_ENT),
                KC_LALT, KC_LGUI, LT(3,KC_ENT),               LT(2,KC_SPC),         LT(2,KC_BSPC),                    LT(3, KC_DEL), KC_UP, KC_DOWN
          ),
        [1] = LAYOUT( // Colemak
          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                                       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
          KC_TRNS, KC_Q, KC_W, KC_F, KC_P, KC_G,                                                                      KC_J, KC_L, KC_U, KC_Y, KC_SCLN, KC_TRNS,
          KC_TRNS, KC_A, KC_R, KC_S, KC_T, KC_D,                                                                      KC_H, KC_N, KC_E, KC_I, KC_O, KC_TRNS,
          KC_TRNS, KC_Z, KC_X, KC_C, KC_V, KC_B,                    KC_TRNS,              KC_TRNS,                    KC_K, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_TRNS,
                KC_TRNS, KC_TRNS, KC_TRNS,                          KC_TRNS,              KC_TRNS,                    KC_TRNS, KC_TRNS, KC_TRNS
          ),
        [2] = LAYOUT( // Symbol Layer, Lower
          KC_TRNS, KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC,                                                          KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_TILD,
          KC_TRNS, KC_PIPE, KC_P7, KC_P8, KC_P9, KC_PLUS,                                                             KC_RBRC, KC_RPRN, KC_RCBR, KC_UNDS, KC_PLUS, KC_NO,
          KC_TRNS, KC_GRV, KC_P4, KC_P5, KC_P6, KC_EQL,                                                               KC_LBRC, KC_LPRN, KC_LCBR, KC_DQUO, KC_COLN, KC_DQUO,
          KC_TRNS, KC_P0, KC_P1, KC_P2, KC_P3, KC_DOT,              KC_NO,                KC_NO,                      KC_NO, KC_BSLS, KC_LT, KC_GT, KC_QUES, KC_TRNS,
                  KC_TRNS, KC_TRNS, KC_TRNS,                        KC_TRNS,              KC_TRNS,                    KC_TRNS, KC_TRNS, KC_TRNS
          ),
        [3] = LAYOUT( // Shortcut Layer, Raise
          LCTL(LSFT(KC_ESC)), KC_NO, LGUI(LSFT(KC_M)), LGUI(KC_M), LALT(KC_F4), KC_NO,                                KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
          LGUI(KC_TAB), LCTL(LSFT(KC_TAB)), LCTL(KC_W), LCA(KC_LEFT), LCA(KC_RGHT), LCA_T(KC_T),                      LCTL(KC_Y), LCTL(KC_Z), KC_UP, KC_NO, LCTL(KC_V), KC_TRNS,
          KC_TRNS, LCTL(KC_TAB), ALT_TAB, LCTL(LGUI(KC_LEFT)), LCTL(LGUI(KC_RIGHT)), MEH(KC_TAB),                     KC_PGUP, KC_LEFT, KC_DOWN, KC_RGHT, KC_HOME, KC_TRNS,
          KC_TRNS, LCTL(KC_Z), LCTL(KC_X), LCTL(KC_C), LCTL(KC_V), LCTL(KC_N),        KC_TRNS,      KC_TRNS,          KC_PGDN, LCTL(KC_LEFT), KC_NO, LCTL(KC_RGHT), KC_END, KC_TRNS,
                  KC_TRNS, KC_TRNS, KC_TRNS,                                          KC_TRNS,      KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS
          ),
        [4] = LAYOUT( // PC Control Layer, Adjust
          RESET, DF(0), DF(1), KC_NO, KC_NO, KC_PWR,                                                                  KC_WAKE, KC_SLEP, KC_NO, KC_NO, KC_NO, KC_NO,
          KC_NO, KC_NO, LCTL(KC_W), LCTL(LSFT(KC_T)), KC_NO, LCTL(KC_T),                                              KC_MPRV, KC_MNXT, KC_PSCR, KC_SLCK, KC_NO, KC_NO,
          KC_NO, KC_TRNS, KC_WSTP, KC_WBAK, KC_WFWD, KC_NO,                                                           KC_VOLD, KC_VOLU, KC_MUTE, KC_MPLY, KC_TRNS, KC_NO,
          KC_NO, KC_NO, KC_NO, KC_NO, KC_WHOM, KC_MYCM,             KC_NO,                KC_NO,                      KC_MYCM, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                  KC_TRNS, KC_TRNS, KC_TRNS,                        KC_TRNS,              KC_TRNS,                    KC_TRNS, KC_TRNS, KC_TRNS
          ),
        [5] = LAYOUT( // Function Layer
          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,                                                                   KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_NO,
          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,                                                                   KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_TRNS,
          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,                                                                   KC_F11, KC_F13, KC_F14, KC_F15, KC_F16, KC_NO,
          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,                 KC_NO,                KC_NO,                      KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                  KC_NO, KC_NO, KC_NO,                              KC_NO,                KC_NO,                      KC_RALT, KC_RCTL, KC_RSFT
          ),
        [6] = LAYOUT( // Gaming Layer 1
          KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5,                                                                       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
          KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T,                                                                       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
          KC_LSFT, KC_A, KC_S, KC_D, KC_F, KC_G,                                                                      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
          KC_LCTRL, KC_Z, KC_X, KC_C, KC_V, KC_B,                   KC_TRNS,              KC_TRNS,                    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                  KC_LALT, KC_SPC, LT(7,KC_SPC),                     KC_SPC,               KC_TRNS,                    KC_TRNS, KC_TRNS, KC_TRNS
          ),
        [7] = LAYOUT( // Gaming Layer 2
          KC_ESC, KC_6, KC_7, KC_8, KC_9, KC_0,                                                                       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
          KC_TRNS, KC_Y, KC_U, KC_I, KC_O, KC_P,                                                                      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
          KC_TRNS, KC_H, KC_J, KC_K, KC_L, KC_NO,                                                                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
          KC_TRNS, KC_N, KC_M, KC_COMM, KC_DOT, KC_NO,              KC_NO,                KC_TRNS,                    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                  KC_NO, KC_NO, KC_TRNS,                            KC_NO,                KC_TRNS,                    KC_TRNS, KC_TRNS, KC_TRNS
          ),
        [8] = LAYOUT( // Navigation Layer
          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                                       KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                                       KC_NO, KC_NO, KC_UP, KC_NO, KC_NO, KC_NO,
          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                                       KC_PGUP, KC_LEFT, KC_DOWN, KC_RGHT, KC_HOME, KC_PENT,
          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS,              KC_TRNS,                    KC_PGDN, LCTL(KC_LEFT), KC_NO, LCTL(KC_RGHT), KC_END, KC_PENT,
                  KC_TRNS, KC_TRNS, KC_TRNS,                        KC_TRNS,              KC_SPC,                     KC_SPC, KC_SPC, KC_SPC)
};
int RGB_current_mode;

// Setting ADJUST layer RGB back to default
void update_tri_layer_RGB(uint8_t layer1, uint8_t layer2, uint8_t layer3) {
  if (IS_LAYER_ON(layer1) && IS_LAYER_ON(layer2)) {
    layer_on(layer3);
  } else {
    layer_off(layer3);
  }
}

void matrix_init_user(void) {
    #ifdef RGBLIGHT_ENABLE
      RGB_current_mode = rgblight_config.mode;
    #endif
}

void matrix_scan_user(void) {
  // For Alt-Tabbing with own key
  if (is_alt_tab_active) {
    if (timer_elapsed(alt_tab_timer) > 1000) {
      unregister_code(KC_LALT);
      is_alt_tab_active = false;
    }
  }
}

//SSD1306 OLED update loop, make sure to enable OLED_DRIVER_ENABLE=yes in rules.mk
#ifdef OLED_DRIVER_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_master())
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
  return rotation;
}

// When you add source files to SRC in rules.mk, you can use functions.
const char *read_layer_state(void);
const char *read_logo(void);
void set_keylog(uint16_t keycode, keyrecord_t *record);
const char *read_keylog(void);
const char *read_keylogs(void);

// const char *read_mode_icon(bool swap);
// const char *read_host_led_state(void);
// void set_timelog(void);
// const char *read_timelog(void);

void oled_task_user(void) {
  if (is_keyboard_master()) {
    // If you want to change the display of OLED, you need to change here
    oled_write_ln(read_layer_state(), false);
    oled_write_ln(read_keylog(), false);
    oled_write_ln(read_keylogs(), false);
    //oled_write_ln(read_mode_icon(keymap_config.swap_lalt_lgui), false);
    //oled_write_ln(read_host_led_state(), false);
    //oled_write_ln(read_timelog(), false);
  } else {
    oled_write(read_logo(), false);
    // oled_write_ln("Right Keyboard", false);
    // oled_write_ln(read_layer_state(), false);
    // oled_write_ln(read_keylog(), false);
    // oled_write_ln(read_keylogs(), false);
  }
}
#endif // OLED_DRIVER_ENABLE

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  static uint8_t saved_mods  = 0;
  uint16_t       temp_keycode = keycode;

  // Filter out the actual keycode from MT and LT keys.
  if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) || (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) {
      temp_keycode &= 0xFF;
  }

  switch (temp_keycode) {
    case BSP_DEL:
        if (record->event.pressed) {
            saved_mods = get_mods() & MOD_MASK_SHIFT;

            if (saved_mods == MOD_MASK_SHIFT) {  // Both shifts pressed
                register_code(KC_DEL);
            } else if (saved_mods) {   // One shift pressed
                del_mods(saved_mods);  // Remove any Shifts present
                register_code(KC_DEL);
                add_mods(saved_mods);  // Add shifts again
            } else {
                register_code(KC_BSPC);
            }
        } else {
            unregister_code(KC_DEL);
            unregister_code(KC_BSPC);
        }
        break;
  }

  switch (keycode) {
    case ALT_TAB:
       // Register own alt tab
      if (record->event.pressed) {
        if (!is_alt_tab_active) {
          is_alt_tab_active = true;
          register_code(KC_LALT);
        }
        alt_tab_timer = timer_read();
        register_code(KC_TAB);
      } else {
        unregister_code(KC_TAB);
      }
      break;
    case QWERTY:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_QWERTY);
      }
      return false;
      break;
    case COLEMAK:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_COLEMAK);
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case ADJUST:
        if (record->event.pressed) {
          layer_on(_ADJUST);
        } else {
          layer_off(_ADJUST);
        }
        return false;
        break;
    case FUNCTION:
        if (record->event.pressed) {
          layer_on(_FUNCTION);
        } else {
          layer_off(_FUNCTION);
        }
        return false;
        break;
  }

  if (record->event.pressed) {
    #ifdef OLED_DRIVER_ENABLE
        set_keylog(keycode, record);
    #endif
        // set_timelog();
  }

  return true;
}
