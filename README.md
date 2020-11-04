# cacophagy

Basically the same thing I'm doing with `psyomn/ecophagy` but mostly a
bunch of fun c++ projects I just want to bundle together.

This code base uses exceptions.

## build, test, etc

```nocode
bazel build ...
bazel test ...
```

## subprojects

- [ds](./ds): simple data structures. for fun.
- [rfc4122-uuid](./rfc4122-uuid): implementation: mainly for fun
- [fttl](./fttl): file time to live. Simple proof of concept of a
  service to expire files after a while.
