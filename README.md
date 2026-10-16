Bored of every soundboard app being behind a paywall? We are too! SoundBored is here to fix that

## Download

Grab the latest version from the [Releases page](https://github.com/lucaspujol/soundbored/releases/latest).

### Windows

1. Download `soundbored-<version>-windows-x64.zip` and extract it.
2. Run `soundbored.exe` inside the extracted `soundbored` folder.

Windows may show "Windows protected your PC": the app isn't code-signed. Click **More info**, then **Run anyway**.

### Linux

```sh
tar -xzf soundbored-<version>-linux-x64.tar.gz
./soundbored/soundbored
```

Needs glibc 2.35 or newer (Ubuntu 22.04, Debian 12, or any recent distro).

Keep the `assets` folder next to the executable: the app loads its fonts and icons from there.

## Build from source

```sh
cmake -B build
cmake --build build
./soundbored
```
