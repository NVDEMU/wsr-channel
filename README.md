# Resort Records

A custom Wii Menu companion channel concept for **Wii Sports Resort**, inspired by the idea of the Mario Kart Channel. The first milestone is a native Wii homebrew prototype with offline-first design and read-only save detection.

## Current status

This repository is at the **bootable-prototype** stage.

- Native Wii homebrew app using devkitPPC and libogc.
- Wii Remote menu navigation.
- Records, Challenges, Settings, and About screens.
- Read-only probe for the known North American and European Sports2.dat save locations.
- GitHub Actions build using the official devkitPro devkitPPC container.
- Homebrew Channel SD-card test package.

**Not implemented yet:** decoding the save's binary structure, real record display, persisted challenge completion, a custom channel banner, and WAD packaging. The UI must not claim to show game records until the parser has been validated against real save samples. The WAD workflow will be added only when it can produce a real, structurally valid channel package.

## Build

The recommended build path is GitHub Actions. Push a commit and open the **Actions** tab to download the resort-records-build artifact.

To compile locally, install devkitPro's Wii development group (devkitPPC and libogc), then run:

~~~sh
make clean all package-sd
~~~

The build creates:

- resort-records.dol — native Wii executable.
- dist/resort-records-sd.zip — folder package for SD-card testing through the Homebrew Channel.
- dist/apps/resort-records/boot.dol and meta.xml — SD-card app files.

## Test in Dolphin

1. Download and extract the CI artifact.
2. For a quick executable test, open resort-records.dol in Dolphin using **File → Open**.
3. For Homebrew Channel-style testing, copy the apps/resort-records folder to the root of an SD card or Dolphin's emulated SD card, then launch it through the Homebrew Channel.
4. Verify the UI, Wii Remote input, exit behavior, and save-probe message. At this stage, no actual game records are decoded.

Once WAD packaging exists, install that WAD only in a disposable Dolphin NAND first.

## Save-data investigation

Wii Sports Resort uses a save file named Sports2.dat. The PAL save location has been documented as:

~~~text
/title/00010000/525a5450/data/Sports2.dat
~~~

The North American title ID is RZTE (525a5445); the implementation probes the corresponding path as a candidate. See [docs/save-format.md](docs/save-format.md).

The application opens the candidate file read-only just to test accessibility. It does **not** decode or modify the save. Filesystem access may differ between a Homebrew Channel launch and an installed channel, so both need testing.

## Safety and project principles

- Never write to or replace the original Wii Sports Resort save.
- Keep the first release offline-only. No DNS, server, account, or internet connection is required.
- Store future channel-owned challenge data separately from the game's save.
- Reject missing, malformed, or unsupported save data gracefully.
- Build and test in Dolphin before testing on a real Wii.
- Do not install an unverified WAD on a primary console. Keep a NAND backup and a recovery method before real-hardware channel installation.

## Roadmap

1. **Bootable prototype:** compile, menu navigation, safe return-to-menu behavior.
2. **Save access:** test read-only access to regional saves in Dolphin and on real hardware.
3. **Format research:** map verified fields with multiple legitimate save samples and in-game comparisons.
4. **Offline features:** show only verified records and track channel-owned challenges separately.
5. **Channel packaging:** original banner/layout/audio, unique title ID, valid WAD build, and clean installation tests.

## References

- [devkitPro Docker images](https://github.com/devkitPro/docker)
- [Wii Sports Resort save-data discussion and PAL path](https://forum.wii-homebrew.com/index.php/Thread/60154-SP2P01-Wii-Sports-Wii-Sports-Resort-PAL-Save-Hacking/)
- [Wii game save folder structure](https://wiibrew.org/wiki/Title_Database)
