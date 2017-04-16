
path = require 'path'
fs = require 'fs'
async = require 'async'
SerialPort = require 'serialport'
uuid = require 'uuid'
msgfloNodejs = require 'msgflo-nodejs'
debug = require('debug')('sanctuary:ReadSerial')

ReadSerial = (client, role) ->
  id = process.env.DYNO or uuid.v4()
  id = "#{role}-#{id}"

  definition =
    id: id
    component: 'sanctuary/ReadSerial'
    icon: 'code'
    label: 'Gets data from a serial device'
    inports: [
      { id: 'out', hidden: true } # for proxying data from .send() to outports through func()
    ]
    outports: [
      { id: 'out' }
    ]

  func = (inport, indata, send) ->
    # forward
    debug 'sending', inport, indata
    return send inport, null, indata

  return new msgfloNodejs.participant.Participant client, definition, func, role

module.exports = ReadSerial

main = () ->
  broker = process.env.MSGFLO_BROKER or 'mqtt://localhost'
  p = ReadSerial broker, 'serial'

  p.start (err) ->
    throw err if err

    port = process.env.SANCTUARY_SERIAL || '/dev/ttyACM0'
    baudRate = parseInt(process.env.SANCTUARY_BAUDRATE || '115200')
    console.log 'using', port, 'at', baudRate
    options =
      baudRate: baudRate
      #parser: SerialPort.parsers.readline
    serial = new SerialPort port, options

    serial.on 'error', (err) ->
      console.error err
      throw err

    serial.on 'data', (data) ->
      d = data.toString()
      debug 'serial data', d
      p.send 'out', d      

    serial.on 'open', () ->
      console.log 'opened'

main() if not module.parent
