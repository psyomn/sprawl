# packetdraw

The idea simple:

The server runs, and draws an X11 window, which is blank.

Clients can send UDP messages to the server, and tell it to draw things on said
window.

## PROTOCOL

DRAW PIXEL

    1 octets: PIXEL_TYPE
    6 octets: r, g, b (16 bit unsigned)
    4 octets: x, y    (16 bit unsigned)

DRAW LINE

    1 octets: LINE_TYPE
    6 octets: r, g, b        (16 bit unsigned)
    8 octets: x1, y1, x2, y2 (16 bit unsigned)

DRAW RECTANGLE

    1 octets: RECTANGLE_TYPE
    6 octets: r, g, b        (16 bit unsigned)
    8 octets: x1, y1, x2, y2 (16 bit unsigned)

CONSTANTS

    UNKNOWN_TYPE   = 0
    PIXEL_TYPE     = 1
    LINE_TYPE      = 2
    RECTANGLE_TYPE = 3

