Tetsts
======
To run a test use  
`prototype1 [options] <type>_<color>_<distance>_<lighting>_0.cmds`  
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

`prototype1 rotate_blue_300_t_0.cmds`  
`prototype1 -lvvkL peak rotate_red_300_dt_0.cmds`  

Notes
-----

* Your working directory has to be this directory  
* Not all combinations of `.cmds` exist  
