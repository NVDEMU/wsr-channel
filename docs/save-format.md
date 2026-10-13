# Wii Sports Resort save-data research

## Known file

The game save file is commonly named **Sports2.dat**.

The PAL save location is documented as:

~~~text
/title/00010000/525a5450/data/Sports2.dat
~~~

The North American disc/title code is RZTE; the corresponding candidate directory is:

~~~text
/title/00010000/525a5445/data/Sports2.dat
~~~

Sources:
- https://forum.wii-homebrew.com/index.php/Thread/60154-SP2P01-Wii-Sports-Wii-Sports-Resort-PAL-Save-Hacking/
- https://gamefaqs.gamespot.com/wii/950923-wii-sports-resort/saves

## What the prototype does

The initial reader only tests whether ISFS can open the candidate file for read access. It does not parse the file, make assumptions about offsets, or change game data. Successful open means only that the file is readable—not that its contents are valid or decoded.

## Next research steps

1. Capture a few save files from supported regions with owners' permission.
2. Record file size and SHA-256 hash, and keep the originals unchanged.
3. Compare save files before and after one controlled in-game action at a time.
4. Identify structure, version markers, checksums, and candidate statistic fields.
5. Confirm each decoded value against the game's UI before displaying it.
6. Add parser tests using minimized fixtures with personal Mii names and other private data removed where possible.

Never commit an entire personal save file to this public repository without clear permission. Treat save samples as user data.

## Access constraints

ISFS permissions can differ when a DOL is launched from the Homebrew Channel versus when it runs as an installed channel. Test both environments. If the installed channel cannot read the game's save, show that limitation rather than writing to NAND, requesting unsafe permissions, or silently falling back to guessed values.
