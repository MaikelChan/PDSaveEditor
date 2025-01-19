# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.4] - 2025-01-19
### Added
- Added names of the heads in the multiplayer character selector, at least the ones I recognized.
### Fixed
- Fixed a typo in one of the character bodies: Joanna Part Frock -> Joanna Party Frock.
- A bit of extra rendeing performance improvements.

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