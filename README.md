# cacophagy

Basically the same thing I'm doing with `psyomn/ecophagy` but mostly a
bunch of fun c++ projects I just want to bundle together.

## build, test, etc

Need C++17.

This code base uses exceptions.

```nocode
bazel build ...
bazel test ...
```

## subprojects

- [ds](./ds): simple data structures. for fun.
- [rfc4122-uuid](./rfc4122-uuid): implementation: mainly for fun
- [fttl](./fttl): file time to live. Simple proof of concept of a
  service to expire files after a while.
