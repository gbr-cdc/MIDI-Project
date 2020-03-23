import mido
import time
from threading import Thread
import signal
import sys

class Note:
	def __init__(self, cmd, val, vel, delay):
		self.cmd = cmd
		self.val = val
		self.vel = vel
		self.delay = delay

class Loop(Thread):
	def __init__ (self, ch):
		Thread.__init__(self)
		self.ch = ch
		self.keep = True
		self.time = 1
		self.seq = []

	def run(self):
		index = 0
		if len(self.seq) != 0:
			while self.keep:
				n = self.seq[index]
				msg = mido.Message(n.cmd, note = n.val, velocity = n.vel)
				outport.send(msg)
				time.sleep(n.delay * self.time)
				index = (index + 1)%len(self.seq)
				
def terminateProcess(signalNumber, frame):
	for i in range(0, 127):
		msg = mido.Message('note_off', note = i, velocity = 0)
		outport.send(msg)
	print(' ')
	sys.exit()

signal.signal(signal.SIGINT, terminateProcess)
outport = mido.open_output()
l = Loop(1)

n1 = Note('note_on', 60, 100, 0.5)
l.seq.append(n1)
n2 = Note('note_off', 60, 0, 0.01)
l.seq.append(n2)
n3 = Note('note_on', 62, 100, 0.5)
l.seq.append(n3)
n4 = Note('note_off', 62, 0, 0.01)
l.seq.append(n4)
n5 = Note('note_on', 64, 100, 0.5)
l.seq.append(n5)
n6 = Note('note_off', 64, 0, 0.01)
l.seq.append(n6)
n7 = Note('note_on', 65, 100, 0.5)
l.seq.append(n7)
n8 = Note('note_off', 65, 0, 0.01)
l.seq.append(n8)
n9 = Note('note_on', 67, 100, 0.5)
l.seq.append(n9)
n10 = Note('note_off', 67, 0, 0.01)
l.seq.append(n10)
n11 = Note('note_on', 69, 100, 0.5)
l.seq.append(n11)
n12 = Note('note_off', 69, 0, 0.01)
l.seq.append(n12)
n13 = Note('note_on', 71, 100, 0.5)
l.seq.append(n13)
n14 = Note('note_off', 71, 0, 0.01)
l.seq.append(n14)
n15 = Note('note_on', 72, 100, 0.5)
l.seq.append(n15)
n16 = Note('note_off', 72, 0, 0.01)
l.seq.append(n16)
l.start()
