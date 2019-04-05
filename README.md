lua-nbt
=======

Lua library to aid encoding/decoding of Named Binary Tag data.

Compression
-----------

This library dies not handle compression of any kind. It's your responsibility
to decompress the NBT data first before passing it to this library.

Install
-------

Just copy `nbt.lua` to your libraries folder and `require` it.

Tests
-----

See `spec` folder. Tests are done using `busted` Note that tests may not cover
everything (the best luacov I got as of writing is 66.57% with 458 hits and 230 misses)

Rocks
-----

Until I can ensure this library is bug-free I won't create rockfile for this.
