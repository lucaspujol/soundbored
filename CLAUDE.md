# Soundbored

Cross-platform (Windows + Linux) desktop soundboard. Import audio files, fire them from a button grid or keybinds, and route playback to any system output device, including a virtual cable so the sounds come through as a mic.

## Stack

- **Language:** C++20 (needed for Clay's designated-initializer macros)
- **Window / rendering / input:** raylib, built **without** its audio module (`SUPPORT_MODULE_RAUDIO` off). raylib already bundles miniaudio, so keeping its audio module on causes duplicate symbols.
- **UI layout:** Clay, drawn with its raylib renderer
- **Audio:** miniaudio, used directly (`ma_context` for device enumeration, `ma_engine` for playback)
- **File dialogs:** nativefiledialog-extended (drag and drop is also supported through raylib)
- **Build:** CMake, with dependencies pulled via `FetchContent`

## Build

```sh
cmake -B build
cmake --build build
./soundbored
```

## Architecture

> The following section is not final, it is subject to change

- `AudioEngine`: owns one `ma_engine` per output device (a monitor device plus an optional "mic" device such as a virtual cable) and plays each sound on both.
- `DeviceManager`: enumerates playback devices and handles hot-plug by re-enumerating and re-initializing engines when a device disappears.
- `SoundLibrary`: the imported sounds, with per-sound volume and keybind.
- `Config`: saves and loads JSON. Devices are stored by **name**, not ID, because IDs aren't stable across reboots.
- `UI`: builds the Clay layout each frame and holds no audio logic.

## Rules

- Keep audio, UI, and platform code separate. Platform-specific code (global hotkeys: `RegisterHotKey` on Windows, `XGrabKey` on X11) goes behind a single interface in `platform/`.
- Every change must build on both Windows (MSVC) and Linux (GCC/Clang). No platform-only APIs outside `platform/`.
- Wayland global hotkeys are best-effort. Don't block features on them.
- Supported formats: WAV, MP3, MP4 (ffmpeg or using `ma_decoder` & lightweight mp4 decoding lib like `libfaad2` or `dr_aac`), FLAC (built into miniaudio), and OGG (via stb_vorbis).
- Use RAII wrappers around miniaudio and raylib resources. No raw `new`/`delete`.

## Working with me

- Explain the plan before making large changes, and go step by step. I'd rather understand the code than get a big dump.
- Keep explanations short and technical. prefer bullet points when summarizing. Never do long sentences: two short ones & an example are easier to understand than a paragraph.