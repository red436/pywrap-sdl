from Window import Window
import socket
import math
window = Window()

dur_sec = 3
duration = int( 44100 * dur_sec)
hz = 250
volume = 20000

window.makeWindow(640, 480, "Changed Window Two")
window.addRect("imageOne")
window.addRect("imageTwo")
window.addSurface("test.png")

x = 0.0
incr_theta = hz * 2 * math.pi / 44100
for n in range(0, duration):
   sample = math.sin( x ) * volume
   window.playRawSound( int(sample) )
   x += incr_theta

imageX = 0
imageY = 0

class Player:

   pos = dict()

   def processEvents( self, evs, x, y ):

      self.pos["x"] = x
      self.pos["y"] = y

      if evs["type"] == "KEYBOARD" and evs["state"] == "DOWN":

         if evs["key"] == "KEY_LEFT":

            self.pos["x"] = self.pos["x"] - 5

         if evs["key"] == "KEY_RIGHT":

            self.pos["x"] = self.pos["x"] + 5

      return self.pos

quit = False
reload = False
res = ""
player = Player()


while True:

    window.clearWindow()
    window.applySurface("test.png", imageX, imageY)
    evs = window.getEvent()

    while evs["type"] != "NO_EVENT":

       pos = player.processEvents( evs, imageX, imageY )
       imageX = pos["x"]

       if(evs["type"] == "KEYBOARD"):
          if evs["key"] == "KEY_r":
             res = "restart"
             break

       if(evs["type"] == "QUIT"):
          res = "quit"
          break

       evs = window.getEvent()

    window.updateWindow()

    if res == "restart" or res == "quit":
       break

window.closeWindow()

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:

    s.connect(('127.0.0.1', 4000))
    s.sendall(bytes(res, 'utf-8'))
