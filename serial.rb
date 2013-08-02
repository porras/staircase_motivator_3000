require 'rubygems'
require 'serialport'

port = SerialPort.new("/dev/ttyACM0", 9600)
playing = false
playing_from = 0

while line = port.gets.strip do
  puts line
  if line == "GO!" && !playing
    puts "play!"
    mplayer = IO.popen("mplayer -ss 00:00:09.50 song.mp3")
    playing = true
    playing_from = Time.now
  end
  if playing && (Time.now - playing_from > 10)
    puts "stop!"
    mplayer.close
    playing = false
  end
end
