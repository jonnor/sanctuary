console.log 'hey from cofeee'

renderSamples = (canvas, data, options) ->
  ctx = canvas.getContext('2d')
  
  ctx.fillStyle = 'rgba(50, 255, 50, 0.7)'

  for i in [0...data.length]
    d = data[i]
    x = i*5
    y = options.height-d
    ctx.fillRect x, y, 3, 3
    console.log 'rendered dot'


flowtrace = require 'flowtrace'


tracepath = '/trace.json'

showTrace = (callback) ->
  console.log 'show'
  console.time 'show'
  flowtrace.trace.loadHttp tracepath, (err, trace) ->
    return callback err if err

    console.log 'tra', err, trace
    brightnessData = []
    for e in trace.events
      if e.command == 'data' and e.protocol == 'network'
        p = e.payload
        if p.tgt.node == 'anim' and p.tgt.node == 'time'
          null
        if p.src.node == 'anim' and p.src.port == 'out' 
          brightnessData.push Number(p.data)

    e = document.getElementById('out')
    o = 
      width: e.width
      height: e.height
      ymax: 255
      ymin: 0
    console.log brightnessData
    renderSamples e, brightnessData, o

    console.timeEnd 'show'
    return callback null

showCallback = (err) ->
  console.error err if err
  console.log 'show done'
showTrace showCallback
#setInterval () ->
#  showTrace showCallback
#, 10*1000


console.log 'coffe load done'
