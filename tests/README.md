Tetsts
======
To run a test use

`prototype1 <type>_<color>_<distance>_<lighting>_0.cmds [options]`

with

* `<type>`

  `rotate` or `translate`

  The type of the scan.

* `<color>`

  `red`, `green`, `blue`, `orange` or `yellow`

  The color of the Rubik's Cube

* `<distance>`

  `200`, `300`, `400` or `500`

  The distance to the cube

* `<lighting>`

  `d`, `t` or `dt`

  The type of lighting used: direct, top or both


* For additional options see `prototype1 --help`

Examples
--------

`prototype1 rotate\_blue\_300\_t\_0.cmds`

`prototype1 rotate\_red\_300\_dt\_0.cmds -lvvkL peak`

