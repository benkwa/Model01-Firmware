// -*- mode: c++ -*-
// Copyright 2016 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details

/**
 * These #include directives pull in the Kaleidoscope firmware core,
 * as well as the Kaleidoscope plugins we use in the Model 01's firmware
 */


// The Kaleidoscope core
#include "Kaleidoscope.h"

// Support for keys that move the mouse
#include "Kaleidoscope-MouseKeys.h"

// Support for macros
#include "Kaleidoscope-Macros.h"

// Support for controlling the keyboard's LEDs
#include "Kaleidoscope-LEDControl.h"

// Support for the "Boot greeting" effect, which pulses the 'LED' button for 10s
// when the keyboard is connected to a computer (or that computer is powered on)
#include "Kaleidoscope-LEDEffect-BootGreeting.h"

// Support for LED modes that set all LEDs to a single color
#include "Kaleidoscope-LEDEffect-SolidColor.h"

// Support for an LED mode that makes all the LEDs 'breathe'
#include "Kaleidoscope-LEDEffect-Breathe.h"

// Support for LED modes that pulse the keyboard's LED in a rainbow pattern
#include "Kaleidoscope-LEDEffect-Rainbow.h"

// Support for shared palettes for other plugins, like Colormap below
#include "Kaleidoscope-LED-Palette-Theme.h"

// Support for an LED mode that lets one configure per-layer color maps
#include "Kaleidoscope-Colormap.h"

// Support for Keyboardio's internal keyboard testing mode
#include "Kaleidoscope-HardwareTestMode.h"

// Support for host power management (suspend & wakeup)
#include "Kaleidoscope-HostPowerManagement.h"

// Support for magic combos (key chords that trigger an action)
#include "Kaleidoscope-MagicCombo.h"

// Support for USB quirks, like changing the key state report protocol
#include "Kaleidoscope-USB-Quirks.h"

#include "Kaleidoscope-Qukeys.h"


/** This 'enum' is a list of all the macros used by the Model 01's firmware
  * The names aren't particularly important. What is important is that each
  * is unique.
  *
  * These are the names of your macros. They'll be used in two places.
  * The first is in your keymap definitions. There, you'll use the syntax
  * `M(MACRO_NAME)` to mark a specific keymap position as triggering `MACRO_NAME`
  *
  * The second usage is in the 'switch' statement in the `macroAction` function.
  * That switch statement actually runs the code associated with a macro when
  * a macro key is pressed.  */

enum {
    MACRO_VERSION_INFO,
    MACRO_ARROW,
    MACRO_MOUSE_SCREEN_L,
    MACRO_MOUSE_SCREEN_R,
    MACRO_CTRL_K,
};



/** The Model 01's key layouts are defined as 'keymaps'. By default, there are three
  * keymaps: The standard QWERTY keymap, the "Function layer" keymap and the "Numpad"
  * keymap.
  *
  * Each keymap is defined as a list using the 'KEYMAP_STACKED' macro, built
  * of first the left hand's layout, followed by the right hand's layout.
  *
  * Keymaps typically consist mostly of `Key_` definitions. There are many, many keys
  * defined as part of the USB HID Keyboard specification. You can find the names
  * (if not yet the explanations) for all the standard `Key_` defintions offered by
  * Kaleidoscope in these files:
  *    https://github.com/keyboardio/Kaleidoscope/blob/master/src/kaleidoscope/key_defs_keyboard.h
  *    https://github.com/keyboardio/Kaleidoscope/blob/master/src/kaleidoscope/key_defs_consumerctl.h
  *    https://github.com/keyboardio/Kaleidoscope/blob/master/src/kaleidoscope/key_defs_sysctl.h
  *    https://github.com/keyboardio/Kaleidoscope/blob/master/src/kaleidoscope/key_defs_keymaps.h
  *
  * Additional things that should be documented here include
  *   using ___ to let keypresses fall through to the previously active layer
  *   using XXX to mark a keyswitch as 'blocked' on this layer
  *   using ShiftToLayer() and LockLayer() keys to change the active keymap.
  *   keeping NUM and FN consistent and accessible on all layers
  *
  * The PROG key is special, since it is how you indicate to the board that you
  * want to flash the firmware. However, it can be remapped to a regular key.
  * When the keyboard boots, it first looks to see whether the PROG key is held
  * down; if it is, it simply awaits further flashing instructions. If it is
  * not, it continues loading the rest of the firmware and the keyboard
  * functions normally, with whatever binding you have set to PROG. More detail
  * here: https://community.keyboard.io/t/how-the-prog-key-gets-you-into-the-bootloader/506/8
  *
  * The "keymaps" data structure is a list of the keymaps compiled into the firmware.
  * The order of keymaps in the list is important, as the ShiftToLayer(#) and LockLayer(#)
  * macros switch to key layers based on this list.
  *
  *

  * A key defined as 'ShiftToLayer(FUNCTION)' will switch to FUNCTION while held.
  * Similarly, a key defined as 'LockLayer(NUMPAD)' will switch to NUMPAD when tapped.
  */

/**
  * layers are "0-indexed" -- That is the first one is layer 0. The second one is layer 1.
  * The third one is layer 2.
  * This 'enum' lets us use names like QWERTY, FUNCTION, and NUMPAD in place of
  * the numbers 0, 1 and 2.
  *
  */

enum {
    PRIMARY,
    ALT,
    CTRL,
    SYMBOL,
    MOUSE,
}; // layers


// Aliases for readability
#define Key_LeftCurly    Key_LeftCurlyBracket
#define Key_RightCurly   Key_RightCurlyBracket
#define Mouse_ScrnL      M(MACRO_MOUSE_SCREEN_L)
#define Mouse_ScrnR      M(MACRO_MOUSE_SCREEN_R)

#define BackWord LCTRL(Key_LeftArrow)
#define ForWord LCTRL(Key_RightArrow)
#define DelWord LCTRL(Key_Delete)

/* This comment temporarily turns off astyle's indent enforcement
 *   so we can make the keymaps actually resemble the physical key layout better
 */
// *INDENT-OFF*

KEYMAPS(

  [PRIMARY] = KEYMAP_STACKED(
          // Left Hand
          Key_Equals,    Key_1, Key_2, Key_3, Key_4, Key_5, Key_Escape,
          Key_CapsLock,  Key_Q, Key_W, Key_E, Key_R, Key_T, Key_LEDEffectNext,
          Key_Tab,       Key_A, Key_S, Key_D, Key_F, Key_G,
          Key_LeftShift, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Meh,

          LT(CTRL, Backspace), ALT_T(Delete), CTL_T(Home), Key_End,
          ShiftToLayer(SYMBOL),


          // Right Hand
          ___,                 Key_6, Key_7, Key_8,     Key_9,         Key_0,         Key_Minus,
          ___,                 Key_Y, Key_U, Key_I,     Key_O,         Key_P,         Key_Backslash,
                               Key_H, Key_J, Key_K,     Key_L,         Key_Semicolon, Key_Quote,
          ShiftToLayer(MOUSE), Key_N, Key_M, Key_Comma, Key_Period,    Key_Slash,     Key_RightShift,

          Key_PageDown, CTL_T(PageUp), LT(ALT, Enter), CTL_T(Spacebar),
          ShiftToLayer(SYMBOL)
    ),


  [ALT] = KEYMAP_STACKED(
          // Left Hand
          LALT(Key_Equals),    LALT(Key_1), LALT(Key_2), LALT(Key_3), LALT(Key_4), LALT(Key_5), LALT(Key_Escape),
          LALT(Key_CapsLock),  LALT(Key_Q), LALT(Key_W), LALT(Key_E), LALT(Key_R), LALT(Key_T), ___,
          LALT(Key_Tab),       LALT(Key_A), LALT(Key_S), DelWord,     ForWord,     LALT(Key_G),
          LALT(Key_LeftShift), LALT(Key_Z), LALT(Key_X), LALT(Key_C), LALT(Key_V), BackWord,    ___,

          LALT(Key_Backspace), LALT(Key_Delete), LALT(Key_Home), LALT(Key_End),
          ShiftToLayer(SYMBOL),


          // Right Hand
          ___, LALT(Key_6), LALT(Key_7), LALT(Key_8),     LALT(Key_9),      LALT(Key_0),         LALT(Key_Minus),
          ___, LALT(Key_Y), LALT(Key_U), LALT(Key_I),     LALT(Key_O),      LALT(Key_P),         LALT(Key_Backslash),
               LALT(Key_H), LALT(Key_J), LALT(Key_K),     LALT(Key_L),      LALT(Key_Semicolon), LALT(Key_Quote),
          ___, LALT(Key_N), LALT(Key_M), LALT(Key_Comma), LALT(Key_Period), LALT(Key_Slash),     LALT(Key_RightShift),

          LALT(Key_PageDown), LALT(Key_PageUp), ALT_T(Enter), LALT(Key_Spacebar),
          ShiftToLayer(SYMBOL)
    ),


  [CTRL] = KEYMAP_STACKED(
          // Left Hand
          LCTRL(Key_Equals),    LCTRL(Key_1), LCTRL(Key_2), LCTRL(Key_3), LCTRL(Key_4),   LCTRL(Key_5),  LCTRL(Key_Escape),
          LCTRL(Key_CapsLock),  LCTRL(Key_Q), LCTRL(Key_X), Key_End,      LCTRL(Key_R),   LCTRL(Key_T),  ___,
          LCTRL(Key_Tab),       Key_Home,     LCTRL(Key_S), LCTRL(Key_D), Key_RightArrow, LCTRL(Key_G),
          LCTRL(Key_LeftShift), LCTRL(Key_Z), LCTRL(Key_X), LCTRL(Key_C), LCTRL(Key_V),   Key_LeftArrow, ___,

          ___, LCTRL(Key_Delete), LCTRL(Key_Home), LCTRL(Key_End),
          ShiftToLayer(SYMBOL),


          // Right Hand
          ___, LCTRL(Key_6),  LCTRL(Key_7), LCTRL(Key_8),     LCTRL(Key_9),      LCTRL(Key_0),         LCTRL(Key_Minus),
          ___, LCTRL(Key_V),  LCTRL(Key_U), LCTRL(Key_I),     LCTRL(Key_O),      Key_UpArrow,          LCTRL(Key_Backslash),
               LCTRL(Key_H),  LCTRL(Key_J), M(MACRO_CTRL_K),  LCTRL(Key_L),      LCTRL(Key_Semicolon), LCTRL(Key_Quote),
          ___, Key_DownArrow, LCTRL(Key_M), LCTRL(Key_Comma), LCTRL(Key_Period), LCTRL(Key_Slash),     LCTRL(Key_RightShift),

          LCTRL(Key_PageDown), LCTRL(Key_PageUp), LCTRL(Key_Enter), LCTRL(Key_Spacebar),
          ShiftToLayer(SYMBOL)
    ),

  [SYMBOL] =  KEYMAP_STACKED(
          Key_Backtick,  Key_F1,  Key_F2,  Key_F3,  Key_F4,  Key_F5,  M(MACRO_VERSION_INFO),
          ___, ___, ___, ___, ___, ___, ___,
          ___, ___, ___, ___, ___, ___,
          ___, ___, ___, ___, ___, ___, ___,
          ___, ___, ___, ___,
          ___,

          ___,          Key_F6,        Key_F7,        Key_F8,         Key_F9,          Key_F10,          ___,
          ___,          ___,           Key_LeftCurly, Key_RightCurly, Key_LeftBracket, Key_RightBracket, ___,
                        Key_LeftArrow, Key_DownArrow, Key_UpArrow,    Key_RightArrow,  ___,              ___,
          Key_LeftGui,  ___,           ___,           Key_8,          M(MACRO_ARROW),  ___,              ___,
          ___, ___, ___, ___,
          ___),


  [MOUSE] =  KEYMAP_STACKED(
          ___, ___, ___,          ___,         ___,          ___, ___,
          ___, ___, Key_mouseUpL, Key_mouseUp, Key_mouseUpR, ___, ___,
          ___, ___, Key_mouseL,   Key_mouseDn, Key_mouseR,   ___,
          ___, ___, Key_mouseDnL, ___,         Key_mouseDnR, ___, ___,
          ___, ___, ___, ___,
          ___,

          ___, ___,              Mouse_ScrnL,     Mouse_ScrnR    , ___          , ___,           ___,
          ___, Key_mouseWarpEnd, Key_mouseWarpNW, Key_mouseWarpNE, Key_mouseBtnL, Key_mouseBtnR, ___,
               ___,              Key_mouseWarpSW, Key_mouseWarpSE, ___,           ___,           ___,
          ___, ___,              ___,             ___,             ___,           ___,           ___,
          ___, ___, ___, ___,
          ___),


  // Template
  // [LAYER_NAME] =  KEYMAP_STACKED
  // (___, ___, ___, ___, ___, ___, ___,
  //  ___, ___, ___, ___, ___, ___, ___,
  //  ___, ___, ___, ___, ___, ___,
  //  ___, ___, ___, ___, ___, ___, ___,
  //  ___, ___, ___, ___,
  //  ___,

  //  ___, ___, ___, ___, ___, ___, ___,
  //  ___, ___, ___, ___, ___, ___, ___,
  //       ___, ___, ___, ___, ___, ___,
  //  ___, ___, ___, ___, ___, ___, ___,
  //  ___, ___, ___, ___,
  //  ___),


) // KEYMAPS(

/* Re-enable astyle's indent enforcement */
// *INDENT-ON*

#ifndef BUILD_INFORMATION
#define BUILD_INFORMATION "locally built on " __DATE__ " at " __TIME__
#endif
#define XSTR(s) STR(s)
#define STR(s) #s

/** versionInfoMacro handles the 'firmware version info' macro
 *  When a key bound to the macro is pressed, this macro
 *  prints out the firmware build information as virtual keystrokes
 */

static void versionInfoMacro(uint8_t keyState) {
    if (keyToggledOn(keyState)) {
        Macros.type(PSTR("Keyboardio Model 01 - Firmware version "));
        Macros.type(PSTR(XSTR(BUILD_INFORMATION)));
    }
}

static void arrowOperatorMacro(uint8_t keyState) {
    if (keyToggledOn(keyState)) {
        Macros.type(PSTR("->"));
    }
}

static void ctrlKMacro(uint8_t keyState) {
    if (keyToggledOn(keyState)) {
        Macros.play(MACRO(D(LeftShift), T(End), U(LeftShift), T(Backspace)));
    }
}

// TODO: add a Ctrl-K macro

/** macroAction dispatches keymap events that are tied to a macro
    to that macro. It takes two uint8_t parameters.

    The first is the macro being called (the entry in the 'enum' earlier in this file).
    The second is the state of the keyswitch. You can use the keyswitch state to figure out
    if the key has just been toggled on, is currently pressed or if it's just been released.

    The 'switch' statement should have a 'case' for each entry of the macro enum.
    Each 'case' statement should call out to a function to handle the macro in question.

 */

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
  switch (macroIndex) {

  case MACRO_VERSION_INFO:
      versionInfoMacro(keyState);
      break;

  case MACRO_ARROW:
      arrowOperatorMacro(keyState);
      break;

  case MACRO_CTRL_K:
      ctrlKMacro(keyState);
      break;
  }

  return MACRO_NONE;
}



// These 'solid' color effect definitions define a rainbow of
// LED color modes calibrated to draw 500mA or less on the
// Keyboardio Model 01.


static kaleidoscope::plugin::LEDSolidColor solidRed(160, 0, 0);
static kaleidoscope::plugin::LEDSolidColor solidOrange(140, 70, 0);
static kaleidoscope::plugin::LEDSolidColor solidYellow(130, 100, 0);
static kaleidoscope::plugin::LEDSolidColor solidGreen(0, 160, 0);
static kaleidoscope::plugin::LEDSolidColor solidBlue(0, 70, 130);
static kaleidoscope::plugin::LEDSolidColor solidIndigo(0, 0, 170);
static kaleidoscope::plugin::LEDSolidColor solidViolet(130, 0, 120);

/** toggleLedsOnSuspendResume toggles the LEDs off when the host goes to sleep,
 * and turns them back on when it wakes up.
 */
void toggleLedsOnSuspendResume(kaleidoscope::plugin::HostPowerManagement::Event event) {
  switch (event) {
  case kaleidoscope::plugin::HostPowerManagement::Suspend:
    LEDControl.disable();
    break;
  case kaleidoscope::plugin::HostPowerManagement::Resume:
    LEDControl.enable();
    break;
  case kaleidoscope::plugin::HostPowerManagement::Sleep:
    break;
  }
}

/** hostPowerManagementEventHandler dispatches power management events (suspend,
 * resume, and sleep) to other functions that perform action based on these
 * events.
 */
void hostPowerManagementEventHandler(kaleidoscope::plugin::HostPowerManagement::Event event) {
  toggleLedsOnSuspendResume(event);
}

/** This 'enum' is a list of all the magic combos used by the Model 01's
 * firmware The names aren't particularly important. What is important is that
 * each is unique.
 *
 * These are the names of your magic combos. They will be used by the
 * `USE_MAGIC_COMBOS` call below.
 */
enum {
  // Toggle between Boot (6-key rollover; for BIOSes and early boot) and NKRO
  // mode.
  COMBO_TOGGLE_NKRO_MODE,
  // Enter test mode
  COMBO_ENTER_TEST_MODE,
};

/** Wrappers, to be used by MagicCombo. **/

/**
 * This simply toggles the keyboard protocol via USBQuirks, and wraps it within
 * a function with an unused argument, to match what MagicCombo expects.
 */
static void toggleKeyboardProtocol(uint8_t combo_index) {
  USBQuirks.toggleKeyboardProtocol();
}

/**
 *  This enters the hardware test mode
 */
static void enterHardwareTestMode(uint8_t combo_index) {
  HardwareTestMode.runTests();
}

/** Magic combo list, a list of key combo and action pairs the firmware should
 * recognise.
 */
USE_MAGIC_COMBOS(
    [COMBO_TOGGLE_NKRO_MODE] = {
                                .action = toggleKeyboardProtocol,
                                // Left Fn + Esc + Shift
                                .keys = { R3C6, R2C6, R3C7 }
    },
    [COMBO_ENTER_TEST_MODE] = {
                               .action = enterHardwareTestMode,
                               // Left Fn + Prog + LED
                               .keys = { R3C6, R0C0, R0C6 }
    },
);

// First, tell Kaleidoscope which plugins you want to use.
// The order can be important. For example, LED effects are
// added in the order they're listed here.
KALEIDOSCOPE_INIT_PLUGINS(

  // The boot greeting effect pulses the LED button for 10 seconds after the
  // keyboard is first connected
  BootGreetingEffect,

  Qukeys,

  // The hardware test mode, which can be invoked by tapping Prog, LED and the
  // left Fn button at the same time.
  HardwareTestMode,

  // LEDControl provides support for other LED modes
  LEDControl,

  // We start with the LED effect that turns off all the LEDs.
  LEDOff,

  // The rainbow effect changes the color of all of the keyboard's keys at the same time
  // running through all the colors of the rainbow.
  LEDRainbowEffect,

  // The rainbow wave effect lights up your keyboard with all the colors of a rainbow
  // and slowly moves the rainbow across your keyboard
  LEDRainbowWaveEffect,

  // These static effects turn your keyboard's LEDs a variety of colors
  solidRed, solidOrange, solidYellow, solidGreen, solidBlue, solidIndigo, solidViolet,

  // The breathe effect slowly pulses all of the LEDs on your keyboard
  LEDBreatheEffect,

  // The LED Palette Theme plugin provides a shared palette for other plugins,
  // like Colormap below
  LEDPaletteTheme,

  // The Colormap effect makes it possible to set up per-layer colormaps
  ColormapEffect,

  // The macros plugin adds support for macros
  Macros,

  // The MouseKeys plugin lets you add keys to your keymap which move the mouse.
  MouseKeys,

  // The HostPowerManagement plugin allows us to turn LEDs off when then host
  // goes to sleep, and resume them when it wakes up.
  HostPowerManagement,

  // The MagicCombo plugin lets you use key combinations to trigger custom
  // actions - a bit like Macros, but triggered by pressing multiple keys at the
  // same time.
  MagicCombo,

  // The USBQuirks plugin lets you do some things with USB that we aren't
  // comfortable - or able - to do automatically, but can be useful
  // nevertheless. Such as toggling the key report protocol between Boot (used
  // by BIOSes) and Report (NKRO).
  USBQuirks
);

/** The 'setup' function is one of the two standard Arduino sketch functions.
 * It's called when your keyboard first powers up. This is where you set up
 * Kaleidoscope and any plugins.
 */
void setup() {
  // First, call Kaleidoscope's internal setup function
  Kaleidoscope.setup();

  // We set the brightness of the rainbow effects to 150 (on a scale of 0-255)
  // This draws more than 500mA, but looks much nicer than a dimmer effect
  LEDRainbowEffect.brightness(150);
  LEDRainbowWaveEffect.brightness(150);

  // Set the action key the test mode should listen for to Left Fn
  HardwareTestMode.setActionKey(R3C6);

  // We want to make sure that the firmware starts with LED effects off
  // This avoids over-taxing devices that don't have a lot of power to share
  // with USB devices
  LEDOff.activate();

  // We need to tell the Colormap plugin how many layers we want to have custom
  // maps for. To make things simple, we set it to five layers, which is how
  // many editable layers we have (see above).
  // ColormapEffect.max_layers(5);

  Qukeys.setOverlapThreshold(50);
}

/** loop is the second of the standard Arduino sketch functions.
  * As you might expect, it runs in a loop, never exiting.
  *
  * For Kaleidoscope-based keyboard firmware, you usually just want to
  * call Kaleidoscope.loop(); and not do anything custom here.
  */

void loop() {
  Kaleidoscope.loop();
}
