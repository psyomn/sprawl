# psycal

Very simple, calendar for text, or other window managers. I originally wrote
this to combine with my status updater for the `dwm`.

    psycald -d

runs the daemon in server mode. If you need to run the server in the foreground
you can simply do:

    psycald -s

You can add en event for your calendar in either the shorthand mode, or the
longer mode. The shorthand mode looks like this:

    psycald -t 4h30 -m "meeting with the people"

will book a meeting at 4h30 the same day, whereas the long form:

    psycald -t 2021-06-01T4h30 "meeting with the old ones"

will book a meeting at 4h30 the given day (with the old ones).

If you need to set a different port than then default, you can use the `-p`
flag:

    psycald -p 6666 -t 3h33 "Ph'nglui mglw'nafh Cthulhu R'lyeh wgah'nagl fhtagn"

## ~/.config/psycal/{events.psycal, history.psycal}

If the user has no `HOME` environment set, `psycal` will throw an exception and
terminate. Considering this is more of an application for users, it makes sense
to add this sanity check.

`psycal` will create two files, where new, unexpired events are stored in
`events.psycal`, and old ones are preserved in `history.psycal`. If you want to
look at the innards for whatever reason, you can use the `-x` dump flag:

    psycald -x /path/to/file/events.psycal
    psycald -x /path/to/file/history.psycal

## design considerations

- should vacuum the data which is before `now()` into another read only
  file.
- keep file format simple of `timestamp`-`message`.

## future endeavors

- add libnotify support if available on the system this is being
  compiled on
- considering UDP sockets are used in this design, it would be fun to think of a
  unified, remote setup, compatible with whatever work environment one finds
  themselves in.
