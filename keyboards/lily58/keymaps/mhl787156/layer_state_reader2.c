
#include QMK_KEYBOARD_H
#include <stdio.h>
#include "lily58.h"

#define L_BASE 0
#define DL_COLEMAK 2
#define L_LOWER (1 << 2)
#define L_RAISE (1 << 3)
#define L_ADJUST (1 << 4)
#define L_ADJUST_TRI (L_ADJUST | L_RAISE | L_LOWER)
#define L_FUNCTION (1 << 5)
#define L_LEFT_GAMING_1 (1 << 6)
#define L_LEFT_GAMING_2 (1 << 7)
#define L_LEFT_GAMING_BOTH (L_LEFT_GAMING_1 | L_LEFT_GAMING_2)
#define L_RIGHT_ARROWS (1 << 8)
#define L_LGAME1_ARROWS (L_LEFT_GAMING_1 | L_RIGHT_ARROWS)
#define L_LGAME2_ARROWS (L_LEFT_GAMING_2 | L_RIGHT_ARROWS)
#define L_LEFT_AND_RIGHT (L_LEFT_GAMING_BOTH | L_RIGHT_ARROWS)

char layer_state_str[24];

const char *read_layer_state(void) {
  switch (layer_state)
  {
  case L_BASE:
    switch (default_layer_state) {
      case DL_COLEMAK:
        snprintf(layer_state_str, sizeof(layer_state_str), "Layer: COLEMAK");
        break;
      default:
        snprintf(layer_state_str, sizeof(layer_state_str), "Layer: QWERTY");
        break;
    }
    break;
  case L_RAISE:
    snprintf(layer_state_str, sizeof(layer_state_str), "Layer: RAISE");
    break;
  case L_LOWER:
    snprintf(layer_state_str, sizeof(layer_state_str), "Layer: LOWER");
    break;
  case L_ADJUST:
  case L_ADJUST_TRI:
    snprintf(layer_state_str, sizeof(layer_state_str), "Layer: ADJUST");
    break;
  case L_FUNCTION:
    snprintf(layer_state_str, sizeof(layer_state_str), "Layer: FUNCTION");
    break;
  case L_LEFT_GAMING_1:
    snprintf(layer_state_str, sizeof(layer_state_str), "Layer: GAMING ONE");
    break;
  case L_LEFT_GAMING_2:
  case L_LEFT_GAMING_BOTH:
    snprintf(layer_state_str, sizeof(layer_state_str), "Layer: GAMING TWO");
    break;
  case L_RIGHT_ARROWS:
    snprintf(layer_state_str, sizeof(layer_state_str), "Layer: RIGHT ARROWS");
    break;
  case L_LGAME1_ARROWS:
  case L_LGAME2_ARROWS:
  case L_LEFT_AND_RIGHT:
    snprintf(layer_state_str, sizeof(layer_state_str), "GAMING N ARROWS");
    break;
  default:
    snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Undef-%ld", layer_state);
  }
  return layer_state_str;
}
