Tools
=====
The tools in this collection are used to work on reconstructed points.
They read points from stdin and write the result to stdout.

getenv (get environment)
------------------------
This program opens a new shell. The new shel has the directory of
`getenv` in `PATH`. It is used to make it easier to work with this
tools.

color
-----
This program can be used to filter points by color.

Usage: `color <min_r> <min_g> <min_b> <max_r> <max_g> <max_b>`

truncate
--------
This program is used cut the nearst and and farest points.

Usage: `truncate <amount>`

You can specify the amount of points to cut on both ends. `<amount>` can either
be a floating point between 0 and 1, a percentage term like `20%` or an
absolute number of points.

stats
-----
This tool is used to create some stats with LaTeX.

Usage: `stats [options and tools]`

Example: `stats -s -D distribution.csv -S xy:scatter.csv`

For more information about existing tools and options write
`stats -h`.
