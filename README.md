# sprawl

Mostly a bunch of fun c++ projects I just want to bundle together.

## build, test, etc

Need C++20.

This code base uses exceptions.

This code is also not meant to be portable.

```nocode
bazel build ...
bazel test ...
./tools/run-valgrind.sh
./tools/make-coverage.sh
```

## subprojects

- [brainfuck](./brainfuck): implementation of a brainfuck interpreter.
- [ds](./ds): data structures for fun.
- [fttl](./fttl): file time to live. Simple proof of concept of a
  service to expire files after a while.
- [gamejam](./gamejam): a collection of common reusable code for gamejams.
- [grimoire](./grimoire): a collection of system calls (linux), small
  experiments that I can quickly go over and remind myself of the existence of
  them.
- [libpsyseg](./libpsyseg): a simple experiment, where a header contains some
  sort of filetype. This is mostly a prototype for a tagging system for
  photographs.
- [net](./net): a collection of socket setups, that are re-usable in some of the
  subproject that require networking.
- [misc](./misc): a collection of one offs, where some might be actual useful
  tools.
- [mdm](./mdm): markdown minus: a markdown parser and formater, which only
  supports a subset of markdown.
- [pshy](./pshy): an implementation of a shell. (everyone should write one at
  least once!).
- [psycal](./psycal): a simple calendar tool that I use with `dwm`. lightweight
  desktop notifications.
- [tinydb](./tinydb): a naive implementation of a relational database (WIP).
- [tvm](./tvm): a tiny virtual machine, which is an implementation on LC3.
