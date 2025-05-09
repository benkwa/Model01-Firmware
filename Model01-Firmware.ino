// Copyright 2016 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details

#include "Kaleidoscope.h"
#include "Kaleidoscope-HardwareTestMode.h"
#include "Kaleidoscope-HostPowerManagement.h"
#include "Kaleidoscope-IdleLEDs.h"
#include "Kaleidoscope-LED-ActiveModColor.h"
#include "Kaleidoscope-LEDBrightnessConfig.h"
#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-LEDEffect-BootGreeting.h"
#include "Kaleidoscope-LEDEffect-Rainbow.h"
#include "Kaleidoscope-Macros.h"
#include "Kaleidoscope-MagicCombo.h"
#include "Kaleidoscope-MouseKeys.h"
#include "Kaleidoscope-OneShot.h"
#include "Kaleidoscope-Qukeys.h"
#include "Kaleidoscope-USB-Quirks.h"


// Macros
// To assign in a keymap: M(MACRO_NAME)  e.g. M(MACRO_ARROW)
// Also, see macroAction function
enum {
    MACRO_VERSION_INFO,
    MACRO_ARROW,
    MACRO_CTRL_K,
};


/**
  * `Key_` definitions here:
  *    https://github.com/keyboardio/Kaleidoscope/blob/master/src/kaleidoscope/key_defs_keyboard.h
  *    https://github.com/keyboardio/Kaleidoscope/blob/master/src/kaleidoscope/key_defs_consumerctl.h
  *    https://github.com/keyboardio/Kaleidoscope/blob/master/src/kaleidoscope/key_defs_sysctl.h
  *    https://github.com/keyboardio/Kaleidoscope/blob/master/src/kaleidoscope/key_defs_keymaps.h
  *
  *  - ___ will let keypresses fall through to the previously active layer
  *  - XXX will mark a keyswitch as 'blocked' on this layer
  *  - ShiftToLayer() switch to layer while held
  *  - LockLayer() change to layer when pressed
  *
  * PROG key docs:
  * https://community.keyboard.io/t/how-the-prog-key-gets-you-into-the-bootloader/506/8
  */

// Note: layers are 0-indexed
enum {
    PRIMARY,
    EMACS,
    ALT,
    CTRL,
    SYMBOL,
    MOUSE,
    WM0,
    WM1,
};


// Aliases for readability
#define Key_LeftCurly    Key_LeftCurlyBracket
#define Key_RightCurly   Key_RightCurlyBracket
#define Key_ShiftLock    OSM(LeftShift)

#define BackWord LCTRL(Key_LeftArrow)
#define ForWord LCTRL(Key_RightArrow)
#define DelWord LCTRL(Key_Delete)

#define Key_WsLeft  MEH(Key_W)
#define Key_WsRight MEH(Key_R)
#define Key_WmLeft  MEH(Key_S)
#define Key_WmRight MEH(Key_F)
#define Key_WmMaxT  MEH(Key_I)
#define Key_WmMaxL  MEH(Key_J)
#define Key_WmMaxR  MEH(Key_L)
#define Key_WmPush  MEH(Key_E)
#define Key_WmPull  MEH(Key_D)


KEYMAPS(

  [PRIMARY] = KEYMAP_STACKED(
          // Left Hand
          Key_Equals,    Key_1, Key_2, Key_3, Key_4, Key_5, Key_Escape,
          ___,           Key_Q, Key_W, Key_E, Key_R, Key_T, ___,
          Key_Tab,       Key_A, Key_S, Key_D, Key_F, Key_G,
          Key_ShiftLock, Key_Z, Key_X, Key_C, Key_V, Key_B, ShiftToLayer(WM0),

          CTL_T(Backspace), ALT_T(Delete), CTL_T(Home), Key_End,
          ShiftToLayer(SYMBOL),


          // Right Hand
          ___,                 Key_6, Key_7, Key_8,     Key_9,         Key_0,         Key_Minus,
          LockLayer(EMACS),    Key_Y, Key_U, Key_I,     Key_O,         Key_P,         Key_Backslash,
                               Key_H, Key_J, Key_K,     Key_L,         Key_Semicolon, Key_Quote,
          ShiftToLayer(MOUSE), Key_N, Key_M, Key_Comma, Key_Period,    Key_Slash,     Key_ShiftLock,

          Key_PageDown, CTL_T(PageUp), ALT_T(Enter), CTL_T(Spacebar),
          ShiftToLayer(SYMBOL)
    ),


  // Emacs emulation - map L_CTL and R_ALT to layers that emulate emacs navigation keys
  [EMACS] =  KEYMAP_STACKED
  (___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   LT(CTRL, Backspace), ___, ___, ___,
   ___,

   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
        ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, LT(ALT, Enter), ___,
   ___),


  // Emacs emulation - map alt keys to forward/backward/delete word
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


  // Emacs emulation - map ctrl keys to arrow/home/end/etc
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

          ___, ___,              ___,             ___,             ___          , ___,           ___,
          ___, Key_mouseWarpEnd, Key_mouseWarpNW, Key_mouseWarpNE, Key_mouseBtnL, Key_mouseBtnR, ___,
               ___,              Key_mouseWarpSW, Key_mouseWarpSE, ___,           ___,           ___,
          ___, ___,              ___,             ___,             ___,           ___,           ___,
          ___, ___, ___, ___,
          ___),

  [WM0] =  KEYMAP_STACKED (
          ___, ___, ___,        ___,        ___,         ___, ___,
          ___, ___, Key_WsLeft, Key_WmPush, Key_WsRight, ___, ___,
          ___, ___, Key_WmLeft, OSL(WM1),   Key_WmRight, ___,
          ___, ___, ___,        ___,        ___,         ___, ___,
          ___, ___, ___,        ___,
          ___,

          ___, ___, ___, ___, ___, ___, ___,
          ___, ___, ___, ___, ___, ___, ___,
               ___, ___, ___, ___, ___, ___,
          ___, ___, ___, ___, ___, ___, ___,
          ___, ___, ___, ___,
          ___),

  [WM1] =  KEYMAP_STACKED (
          ___, ___, ___,        ___,        ___,        ___, ___,
          ___, ___, ___,        Key_WmMaxT, ___,        ___, ___,
          ___, ___, Key_WmMaxL, Key_WmPull, Key_WmMaxR, ___,
          ___, ___, ___,        ___,        ___,        ___, ___,
          ___, ___, ___,        ___,
          ___,

          ___, ___, ___, ___, ___, ___, ___,
          ___, ___, ___, ___, ___, ___, ___,
               ___, ___, ___, ___, ___, ___,
          ___, ___, ___, ___, ___, ___, ___,
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


) // KEYMAPS


#ifndef BUILD_INFORMATION
#define BUILD_INFORMATION "locally built on " __DATE__ " at " __TIME__
#endif
#define XSTR(s) STR(s)
#define STR(s) #s


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


// macro dispatch
// - macro_id: see MACRO_* enum above
// - event: see Kaleidoscope/src/kaleidoscope/KeyEvent.h
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


// toggle the LEDs off when the host goes to sleep, and turn them back on when it wakes up.
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

// dispatch power management events (suspend, resume, and sleep)
void hostPowerManagementEventHandler(kaleidoscope::plugin::HostPowerManagement::Event event) {
  toggleLedsOnSuspendResume(event);
}

// See USE_MAGIC_COMBOS call below.
enum {
  // Toggle between Boot (6-key rollover; for BIOSes and early boot) and NKRO
  // mode.
  COMBO_TOGGLE_NKRO_MODE,
  // Enter test mode
  COMBO_ENTER_TEST_MODE,
};

// toggle the keyboard protocol via USBQuirks
static void toggleKeyboardProtocol(uint8_t combo_index) {
  USBQuirks.toggleKeyboardProtocol();
}

// enter the hardware test mode
static void enterHardwareTestMode(uint8_t combo_index) {
  HardwareTestMode.runTests();
}

USE_MAGIC_COMBOS(
        [COMBO_TOGGLE_NKRO_MODE] = {
            .action = toggleKeyboardProtocol,
            .keys = { R3C6, R2C6, R3C7 }  // Left Fn + Esc + Shift
        },
        [COMBO_ENTER_TEST_MODE] = {
            .action = enterHardwareTestMode,
            .keys = { R3C6, R0C0, R0C6 }  // Left Fn + Prog + LED
        },
);

// Init order is important.
KALEIDOSCOPE_INIT_PLUGINS(

        // pulse the LED button when the keyboard is starting
        BootGreetingEffect,

        // For primary/secondary actions on tap/hold
        Qukeys,

        HardwareTestMode,

        // LED stuff
        LEDControl,
        LEDRainbowWaveEffect,
        ActiveModColorEffect,
        IdleLEDs,

        // See macroAction, above
        // https://kaleidoscope.readthedocs.io/en/latest/plugins/Kaleidoscope-Macros.html
        Macros,

        // The MouseKeys plugin lets you add keys to your keymap which move the mouse.
        MouseKeys,

        // see hostPowerManagementEventHandler
        // https://kaleidoscope.readthedocs.io/en/latest/plugins/Kaleidoscope-HostPowerManagement.html
        HostPowerManagement,

        // See USE_MAGIC_COMBOS
        // https://kaleidoscope.readthedocs.io/en/latest/plugins/Kaleidoscope-MagicCombo.html
        MagicCombo,

        // https://kaleidoscope.readthedocs.io/en/latest/plugins/Kaleidoscope-OneSho.thtml
        OneShot,

        // Low-level workarounds for USB stuff.
        // https://kaleidoscope.readthedocs.io/en/latest/plugins/Kaleidoscope-USB-Quirks.html
        USBQuirks

);

// Standard Arduino sketch function, called when your keyboard first powers up.
// Set up Kaleidoscope and any plugins.
void setup() {
    // First, call Kaleidoscope's internal setup function
    Kaleidoscope.setup();

    // greenish
    BootGreetingEffect.hue = 85;

    // Scale is 0-255
    LEDRainbowWaveEffect.brightness(100);

    // Set the action key the test mode should listen for to Left Fn
    HardwareTestMode.setActionKey(R3C6);

    // We want to make sure that the firmware starts with LED effects off
    // This avoids over-taxing devices that don't have a lot of power to share
    // with USB devices
    LEDOff.activate();

    // 80% is the default; set here for reference
    // Increase this value if getting too many non-modified keys
    // Decrease this value if getting too many modified keys
    Qukeys.setOverlapThreshold(80);
}

// Standard Arduino sketch function.  Event loop.
// For Kaleidoscope-based keyboard firmware, you usually just want to
// call Kaleidoscope.loop(); and not do anything custom here.
void loop() {
    Kaleidoscope.loop();
}
