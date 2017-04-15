
fs = require 'fs'
cheerio = require 'cheerio'
SVGPathData = require 'svg-pathdata'

findPaths = (svg) ->
  c = cheerio.load svg, { xmlMode: true }
  paths = []
  c('path').map (i, e) ->
    paths.push e.attribs.d
  return paths

pathDataToPoints = (data) ->
  path = new SVGPathData data
    .toAbs()
    .normalizeHVZ()
  [move, lines] = [path.commands[0], path.commands[1...]]
  throw new Error 'First command must be a move' if move.type != SVGPathData.MOVE_TO
  unsupported = lines.filter (cmd) -> cmd.type != SVGPathData.LINE_TO
  throw new Error "Only straigth lines are supported. Found: #{unsupported}" if unsupported.length
  return path.commands.map (line) ->
    point =
      x: line.x - move.x
      y: -(line.y - move.y) # Y-axis is downwards in SVG? We want positive Y upwards
    return point

main = () ->
  [node, script, svgpath] = process.argv

  if not svgpath
    console.error 'Usage: svgpath FILE.svg'
    process.exit 1

  callback = (err, data) ->
    if err
      console.error err
      process.exit 2
    console.log data

  fs.readFile svgpath, 'utf-8', (err, contents) ->
    return callback err if err
    paths = findPaths contents
    points = pathDataToPoints paths[0]
    return callback null, points

main() if not module.parent
