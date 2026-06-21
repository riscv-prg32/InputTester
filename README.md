# PRG32 Input Tester

PRG32 Input Tester is an original utility cartridge designed to verify hardware input functionality, screen drawing, and polyphonic audio capabilities on PRG32 devices. It provides immediate visual and auditory feedback for all standard controller buttons, making it an ideal diagnostic tool for testing newly assembled hardware or custom controllers.

The project follows the PRG32 cartridge convention: the C source exports `input_tester_init`, `input_tester_update`, and `input_tester_draw`, matching the build conventions used by PRG32 cartridges.

## Features

- **D-pad Testing**: Left, Right, Up, and Down directional inputs.
- **Action Buttons**: A and B buttons.
- **System Buttons**: Start button.
- **Visual Feedback**: Buttons illuminate with a yellow highlight and inverted text when pressed.
- **Multi-key Registration**: Detects and displays a "Multiple keys registered" flag when more than one button is pressed simultaneously. This is useful for diagnosing key ghosting or hardware matrix issues.
- **Harmonized Audio**: Each button triggers a specific musical note. When multiple buttons are pressed simultaneously, the PRG32 audio API stacks the sounds into a harmonized chord.
  - **Left**: Low C
  - **Right**: Low E
  - **Up**: Low G
  - **Down**: Middle C
  - **A**: Middle E
  - **B**: Middle G
  - **Start**: High C
- **Audio Status Indicator**: Displays "Audio is being played" dynamically at the bottom of the screen when any button is held down and sound is active.

## Capabilities exercised

PRG32 Input Tester deliberately stresses PRG32 subsystems: text rendering, colored rectangle drawing (UI highlights), edge-detection logic for inputs, bitwise masking, multi-button state tracking, audio system initialization, and concurrent multi-channel note sequence generation for dynamic chords.

## License

Code is MIT. See `LICENSE`.
