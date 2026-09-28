# Audio Architecture

Last updated: 28-09-2026

Three-layer design for dual-device playback (physical speaker + optional virtual mic).

## Layers

### AudioEngine (lowest)
**Location:** `src/audio/AudioEngine.{hpp,cpp}`

RAII wrappers around miniaudio primitives:
- `MaEngine`: owns `ma_engine`, bound to single output device
- `MaSound`: owns `ma_sound`, tied to parent engine
- `AudioEngine`: factory for `MaSound` via `createSound(path)` → `unique_ptr<MaSound>`

Primitives:
- `play(MaSound&)` → `ma_sound_start`
- `stop(MaSound&)` → `ma_sound_stop`
- `restart(MaSound&)` → seek to frame 0 + start

### SoundBank (middle)
**Location:** `src/audio/SoundBank.{hpp,cpp}`

Sound collection per device:
- Holds `map<SoundId, unique_ptr<MaSound>>`
- Non-owning ref to `AudioEngine&` (engine owned by manager)
- Load/unload sounds by ID
- Playback control: `play(id)`, `stop(id)`, `restart(id)`
- Time queries: `getSoundLengthMs(id)`, `getSoundRemainingLengthMs(id)`

**SoundId:** `int32_t`, starts at 0, increments globally. `-1` = failure sentinel.

### AudioManager (top, singleton)
**Location:** `src/audio/AudioManager.{hpp,cpp}`

Orchestrates dual-device setup:
- Owns `MaContext` for device enumeration
- **Physical device** (always exists):
  - `unique_ptr<AudioEngine> physicalEngine`
  - `unique_ptr<SoundBank> physicalSoundBank`
  - Defaults to system default device on startup
- **Virtual device** (optional, for mic routing):
  - `unique_ptr<AudioEngine> virtualEngine`
  - `unique_ptr<SoundBank> virtualSoundBank`
  - Set via `setVirtualDevice(name)`, clear via `clearVirtualDevice()`

Global state:
- `map<SoundId, SoundMeta> soundMetaMap`: ID → path mapping
- `nextSoundId`: increments on import
- `cachedDevices`: refreshed on `getAvailableDevices()` call

## Dual-Device Sync

### Sound Import
`importSound(path)` sequence:
1. Load into physical bank
2. If virtual device active, load into virtual bank
3. On virtual load failure: rollback physical, return `-1`
4. Add to `soundMetaMap`, increment `nextSoundId`

### Playback
`playSound(id)` / `stopSound(id)` / `restartSound(id)` broadcast to both banks if virtual active.

### Device Switching
`setPhysicalDevice(name)` / `setVirtualDevice(name)`:
1. Resolve device name → `ma_device_id` via cached device list
2. Rebuild engine + bank pair
3. Iterate `soundMetaMap`, reload all sounds into new bank
4. Return `false` on any load failure

## Device Enumeration & Hot-Plug

**Detection:** `getAvailableDevices()` calls `refreshDeviceCache()` → `ma_context_get_devices()`. Repeated polling detects plugged/unplugged devices.

**Switching:** Manual. User must call `setPhysicalDevice(name)` to switch active device mid-session (triggers engine rebuild + sound reload).

## RAII Chain

All miniaudio resources wrapped:
- `MaContext` → `~MaContext()` calls `ma_context_uninit()`
- `MaEngine` → `~MaEngine()` calls `ma_engine_uninit()`
- `MaSound` → `~MaSound()` calls `ma_sound_uninit()`

Ownership via stack or `unique_ptr`. No raw `new`/`delete`.

**⚠️ Move semantics:** Miniaudio structs (`ma_engine`, `ma_sound`, etc.) hold internal pointers. Moving invalidates these refs → memory corruption. RAII wrappers delete move constructors/assignment. Always pass by ref or use `unique_ptr`.

## Example Usage

Typical flow:
1. Get singleton: `AudioManager::getInstance()`
2. Enumerate devices: `getAvailableDevices()`
3. Set virtual device: `setVirtualDevice("Virtual_Microphone")`
4. Import sounds: `importSound("file.wav")` → returns `SoundId`
5. Play: `playSound(id)` (broadcasts to both devices)
6. Query: `getSoundLengthMs(id)`, `getSoundRemainingLengthMs(id)`
