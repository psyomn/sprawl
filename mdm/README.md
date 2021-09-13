# mdm

I like using Markdown, but there are too many features I don't care about.

I think Markdown has potential do be an OK markup language for simple technical
docs, or (very simple) books.

Most of the tools that I've come up with don't seem to be complete in any
fashion, and most of the features they support I do not find particularly
useful.

Because of this, I've decided to define a subset of Markdown, as Markdown--,
which will support only few features:

- headers.
- code blocks (the difference being that programming language definition is
  required, else this would raise a parsing error).
- lists (basically newlines with hyphens).

This file, is a good example of the features I'd like to be supported.

This tool will:
- automatically format Markdown-- in an opinionated fashion.
- give you the possibility to bind many documents together to form some sort of
  "book" (eventually).

BK is a implementation of MD--.

## Considerations

I'm thinking about also implementing:

- hyperlinks (square bracket thingies)
- image hyperlinks (media for books/article generation would be important)

## Contributions

I'm probably not interested in any other features.

If you have a fix for something I will be very happy to incorporate your work.
