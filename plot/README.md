Plot Tool
=========
This program shows the reconstucted points in a three-dimensional space.

Usage: `plot [options]`

Control
-------

Move the mouse while holding the left or right button to look around. Key
bindings are shown in the table below.

|   Key   | Function
|:-------:|---------------------------------------
|    W    | Move forward
|    A    | Move left
|    S    | Move backward
|    D    | Move right
|  SPACE  | Move up
| C / STRG| Move down
|  SHIFT  | Hold for double speed
|  Q / E  | Rotate
|   TAB   | Change background color
|    R    | Increase speed of movements
|    F    | Decrease speed of movements
|    /    | Increase size of points (- with german layout)
|    ,    | Decrease size of points
|    .    | Toggle size depends on distance
|    K    | Move camera to position of projector

Options
-------

| Option           | Description
|------------------|------------------------------
| `-h` or `--help` | Shows some help
| `-v`             | Prints something more to stdout
| `--box`          | Add random points between (0,0,0) and (1,1,1)
