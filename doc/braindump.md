
## Original notes

From [jonnor/rebirth](https://github.com/jonnor/rebirth)

* Tool that allows drawing datalines with Beziers in SVG.
* Then evaluates this by sampling, and generate C data. 
* Raw data samples as array, number of entries, value mapping (meaning of 0 and max), timestep.
* This can then be used to get exactly the behavior wanted.
* No need to tween etc at runtime.
* Runtime code should be able to do interpolation, at minimum linear.
* Initially draw in Inkscape. Over time move to have dedicated webUI inputs.
* Data formation should be (de)serializable, so it can be sent over protocol, and persisted.

## Related

* How Beziers work: http://h14s.p5r.org/2013/01/bezier.html

Fast embedded math hacks

* Fast inverse/squareroot computation, http://h14s.p5r.org/2012/09/0x5f3759df.html
* Fast implementation of average (of two numbers), https://www.approxion.com/?p=119
