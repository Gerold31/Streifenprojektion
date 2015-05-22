Prototype 1
===========
This program reconstructs three-dimensional points from some images or
a special device. It uses a projected line to do that.

Usage: `prototype1 [options] [file]`

`file`: File with commands to execute. Program uses stdin by default

Options
-------

* `-L <line-detector>`  
  Chooses line detector. Possible values: `default`/`diff`, `free` and
  `peak`.
* `-R <reconstructor>`  
  Chooses reconstructor. There is only one possible value: `default`.
* `-c <device>`  
  Chooses `<device>` as capture device.
* `-s <prefix>`  
  Save captured images.
* `-d <fov> <skew> <offset>`  
  Set device configuration.
* `-r`  
  Perform rotation scan.
* `-t <dx> <dy> <dz> <n>`  
  Perform  translation scan. Move (`<dx>`, `<dy>`, `<dz>`) each step
  for `<n>` steps.
* `--create-commands`  
  Creates commands for future reconstrutions and disable
  reconstruction. This option implies `-s` if it is not given.
* `-h` or `--help`  
  Show this information.

### Debug

* `-C`  
  Show captured images.
* `-l`  
  Shows reconstructed lines
* `-H`  
  Shows heightmap
* `-k`  
  Waits for pressing some key after processing an image.
* `-v`  
  Prints some more information to stdout. If you use it twice, there
  could be some more debug windows but it is only used by Peak yet.
