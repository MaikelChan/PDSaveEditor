# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.1.0] - 2026-05-17
### Changed
- Now you can open save files that are bigger than 2048 bytes. Despite that Perfect Dark save data is only 2048 bytes, some emulators generate bigger files with padding zeroes at the end.
- The program now uses SDL3 instead of GLFW for handling the window, and Vulkan / Direct3D 12 instead of OpenGL for rendering. This has the following advantages:
  - Rendering performance has improved.
  - It uses less memory.
  - Now, when opening a file, the program uses the operating system's native open file dialog, which has more features, like accessing favorite folders, copying and pasting paths, etc.

  But the following disadvantages:
  - The size of the executable has increased.
  - It might require newer hardware than the previous versions.
  - Removed the window transparency setting for now, as it's currently not compatible with SDL / Vulkan / Direct3D 12.

- The program now supports higher DPI, and it will scale all its UI accordingly. The program will also resize in real-time when moving the window between monitors with different DPI settings.
- It uses a new font that will look better when it's scaled up.
- Now the program will show a progress bar in the taskbar that represents the percentage of levels completed (single player or coop), and when checking the multiplayer profiles tab, it will represent the progress on the multiplayer rank. (This might depend on the operating system and/or desktop environment).
- The program now saves a list of the 5 more recent opened files, for easier access.
- Implemented support for the Night Vision Goggles and the IR scanner in multiplayer for the PC version, and properly convert that between N64 and PC formats. Thanks to [cylonicboom](https://github.com/cylonicboom).
- Other small improvements to improve code maintainability.

## [1.0.4] - 2025-01-20
### Added
- Added names of the heads in the multiplayer character selector, at least the ones I recognized.
### Fixed
- Fixed a typo in one of the character bodies: Joanna Part Frock -> Joanna Party Frock.
- A bit of extra rendering performance improvements.

## [1.0.3] - 2025-01-06
### Fixed
- Now the initial window position is properly set when booting the program, avoiding the window appearing at a different position for an instance before finally moving to the center of the screen.
- Rendering performance improvements.

## [1.0.2] - 2024-12-29
### Changed
- Changed config format from JSON to INI.
- Added used libraries in the About window and updated hyperlinks code, since now ImGui supports it natively.
### Fixed
- Fixed some potential memory corruption and crash issues.

## [1.0.1] - 2024-08-19
### Fixed
- Fixed weapon selector in multiplayer settings being only compatible with PC weapons.
- Fixed issues with multiplayer weapons when converting save format, as the PC port added the classic weapons, which were not available on N64.

## [1.0.0] - 2024-08-19
- Initial release.