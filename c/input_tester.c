#include "prg32.h"

static uint32_t current_input = 0;
static int needs_redraw =
    1; /* Start at 1 so the initial state draws immediately */
static int multi_key_flag =
    0; /* 1 = text stays on screen, 0 = text is hidden */

/* Helper function prototype */
static void draw_button(int is_pressed, int x, int y, const char *str);

/* ---------------------------------------------------------
 * INIT
 * --------------------------------------------------------- */
void input_tester_init(void) {
  if (prg32_audio_init(NULL)) {
    prg32_audio_set_master_volume(180);
  }
}

/* ---------------------------------------------------------
 * UPDATE
 * --------------------------------------------------------- */
void input_tester_update(void) {
  uint32_t new_input = prg32_input_read();

  if (new_input == current_input) {
    return; /* If input hasn't changed, skip everything */
  }

  uint32_t pressed = new_input & ~current_input;

  if (pressed & PRG32_BTN_LEFT)
    prg32_audio_note(36, 135); /* Low C */
  if (pressed & PRG32_BTN_RIGHT)
    prg32_audio_note(40, 135); /* Low E */
  if (pressed & PRG32_BTN_UP)
    prg32_audio_note(43, 135); /* Low G */
  if (pressed & PRG32_BTN_DOWN)
    prg32_audio_note(48, 135); /* Middle C */
  if (pressed & PRG32_BTN_A)
    prg32_audio_note(52, 135); /* Middle E */
  if (pressed & PRG32_BTN_B)
    prg32_audio_note(55, 135); /* Middle G */
  if (pressed & PRG32_BTN_START)
    prg32_audio_note(60, 135); /* High C */

  /* If we made it here, input changed. Save it and flag a redraw. */
  current_input = new_input;
  needs_redraw = 1;

  /* --- MULTI-KEY PERSISTENCE LOGIC --- */
  uint32_t masked_input =
      new_input & 0x7F; /* Mask out only our 7 button bits (127) */

  if (masked_input == 0) {
    return; /* IF NO KEYS PRESSED: Leave the flag exactly as it is! */
  }

  /* If we are here, >0 keys are pressed. Let's see if it's multiple.
     Using the bitwise trick: n & (n - 1) */
  if (masked_input & (masked_input - 1)) {
    multi_key_flag = 1; /* Multiple keys pressed: Set the flag */
  } else {
    multi_key_flag = 0; /* Single key pressed: Clear the flag */
  }
}

/* ---------------------------------------------------------
 * DRAW
 * --------------------------------------------------------- */
void input_tester_draw(void) {
  /* --- TEST BEFORE CLEAR --- */
  if (!needs_redraw) {
    return; /* If needs_redraw == 0, skip drawing entirely! */
  }

  /* Clear the redraw flag so we don't draw next frame unless input changes */
  needs_redraw = 0;

  /* Now we are safe to clear the screen */
  prg32_gfx_clear(PRG32_COLOR_BLACK);

  /* --- D-PAD --- */
  draw_button(current_input & PRG32_BTN_LEFT, 40, 100, "L");
  draw_button(current_input & PRG32_BTN_RIGHT, 80, 100, "R");
  draw_button(current_input & PRG32_BTN_UP, 60, 80, "U");
  draw_button(current_input & PRG32_BTN_DOWN, 60, 120, "D");

  /* --- ACTION BUTTONS --- */
  draw_button(current_input & PRG32_BTN_A, 220, 100, "A");
  draw_button(current_input & PRG32_BTN_B, 260, 100, "B");

  /* --- SYSTEM BUTTONS --- */
  draw_button(current_input & PRG32_BTN_START, 150, 100, "S");

  /* --- DRAW PERSISTENT MULTI-KEY TEXT --- */
  if (multi_key_flag) {
    prg32_gfx_text8(40, 20, "Multiple keys registered", PRG32_COLOR_WHITE,
                    PRG32_COLOR_BLACK);
  }

  /* --- AUDIO PLAYING TEXT --- */
  if (current_input & 0x7F) {
    prg32_gfx_text8(40, 180, "Audio is being played", PRG32_COLOR_GREEN,
                    PRG32_COLOR_BLACK);
  }
}

/* ---------------------------------------------------------
 * HELPER: draw_button
 * --------------------------------------------------------- */
static void draw_button(int is_pressed, int x, int y, const char *str) {
  if (is_pressed) {
    /* --- DRAW YELLOW HIGHLIGHT RECTANGLE --- */
    /* Shift x and y slightly for padding */
    prg32_gfx_rect(x - 4, y - 4, 16, 16, PRG32_COLOR_YELLOW);

    /* Draw the text inverted so it's readable on the yellow box */
    prg32_gfx_text8(x, y, str, PRG32_COLOR_BLACK, PRG32_COLOR_YELLOW);
  } else {
    /* --- DRAW DEFAULT TEXT --- */
    prg32_gfx_text8(x, y, str, PRG32_COLOR_WHITE, PRG32_COLOR_BLACK);
  }
}