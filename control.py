#import evdev
from evdev import InputDevice, categorize, ecodes
import mido
import time
from threading import Thread
import signal
import sys

#creates object 'gamepad' to store the data
#you can call it whatever you like
gamepad = InputDevice('/dev/input/event8')

#prints out device info at start
print(gamepad)
outport = mido.open_output()

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

buttons = {
	288: 60,
	289: 62,
	290: 64,
	291: 65,
	292: 67,
	293: 69,
	294: 71,
	295: 72
}

controls = {
	288: 0,
	289: 1,
	290: 2,
	291: 3,
	292: 4,
	293: 5,
	294: 6,
	295: 7,
	0: 8,
	1: 9,
	2: 10,
	5: 11,
}

notes = {
	288: 0,
	289: 0,
	290: 0,
	291: 0,
	292: 0,
	293: 0,
	294: 0,
	295: 0
}

signal.signal(signal.SIGINT, terminateProcess)
Pad_X = 0
Pad_Y = 0
start = 0
gravar = False

def send_note(event):
	if event.value == 1:
		msg = mido.Message('note_on', note = buttons.get(event.code), velocity = 100)
		notes[event.code] = buttons.get(event.code)
		outport.send(msg)
	else:
		msg = mido.Message('note_off', note = notes.get(event.code), velocity = 0)
		outport.send(msg)

def send_control(event):
	if event.type == 1:
		msg = mido.Message('control_change', control = controls.get(event.code), value = event.value)
		outport.send(msg)
	if event.type == 3:
		if event.code == 16:
			if event.value == 1:
				msg =  mido.Message('control_change', control = 16, value = 1)
				outport.send(msg)
				Pad_X = 1
			if event.value == 0:
				if Pad_X == 1:
					msg =  mido.Message('control_change', control = 16, value = 0)
					outport.send(msg)
				if Pad_X == -1:
					msg =  mido.Message('control_change', control = 17, value = 0)
					outport.send(msg)
				Pad_X = 0
			if event.value == -1:
				msg =  mido.Message('control_change', control = 17, value = 1)
				outport.send(msg)
				Pad_X = -1
		elif event.code == 17:
			if event.value == 1:
				msg =  mido.Message('control_change', control = 18, value = 1)
				outport.send(msg)
				Pad_Y = 1
			if event.value == 0:
				if Pad_Y == 1:
					msg =  mido.Message('control_change', control = 18, value = 0)
					outport.send(msg)
				if Pad_Y == -1:
					msg =  mido.Message('control_change', control = 17, value = 0)
					outport.send(msg)
				Pad_Y = 0
			if event.value == -1:
				msg =  mido.Message('control_change', control = 19, value = 1)
				outport.send(msg)
				Pad_Y = -1
		else:
			val = int(event.value/255 * 127)
			msg =  mido.Message('control_change', control = controls.get(event.code), value = val)
			outport.send(msg)

def record(event, loop):
	size = len(loop.seq)
	if size > 0:
		loop.seq[size - 1].delay = event.sec - loop.seq[size - 1].delay
	if event.value == 1:
		n = Note('note_on', buttons.get(event.code), 100, event.sec)
	else:
		n = Note('note_off', buttons.get(event.code), 0, event.sec)
	loop.seq.append(n)
		
#evdev takes care of polling the controller in a loop
for event in gamepad.read_loop():
	#print(event)
	#Botão, codes 288 a 295
	if event.type == 1:
		if event.code == 288: #TRIANGLE
			#print("TRIANGLE")
			send_note(event)
			if gravar:
				record(event, l)
		if event.code == 289: #CIRCLE
			#print("CIRCLE")
			send_note(event)
			if gravar:
				record(event, l)
		if event.code == 290: #XIS
			#print("XIS")
			send_note(event)
			if gravar:
				record(event, l)
		if event.code == 291: #SQUARE
			#print("SQUARE")
			send_note(event)
			if gravar:
				record(event, l)
		if event.code == 292: #L1
			#print("L1")
			send_note(event)
			if gravar:
				record(event, l)
		if event.code == 293: #R1
			#print("R1")
			send_note(event)
			if gravar:
				record(event, l)
		if event.code == 294: #L2
			#print("L2")
			send_note(event)
			if gravar:
				record(event, l)
		if event.code == 295: #R2
			#print("R2")
			send_note(event)
			if gravar:
				record(event, l)
		if event.code == 296: #SELECT
			#print("SELECT")
			send_control(event)
		if event.code == 297: #START
			#print("START")
			if event.value == 1:
				if start == 0:
					gravar = True
					l = Loop(1)
					start = 1
					print("Gravando")
				elif start == 1:
					size = len(l.seq)
					if size > 0:
						l.seq[size - 1].delay = event.sec - l.seq[size - 1].delay
						l.start()
						print("Loop iniciado")
						print(len(l.seq))
						for member in l.seq:
							print(member.cmd)
							print(member.val)
							print(member.vel)
							print(member.delay)
							print('');
						start = 2
					else:
						start = 0
						print("Loop nao iniciado")
					gravar = False
				elif start == 2:
					l.keep = False
					start = 0
					print("Loop interrompido")
						
		if event.code == 298: #L3
			#print("L3")
			send_control(event)
		if event.code == 299: #R3
			#print("R3")
			send_control(event)
	#Eixos
	elif event.type == 3:
		if event.code == 0: #LEFT JOYSTICK X AXIS
			print("LEFT X AXIS")
			send_control(event)
		if event.code == 1: #LEFT JOYSTICK Y AXIS
			print("LEFT Y AXIS")
			send_control(event)
		if event.code == 2: #RIGHT JOYSTICK X AXIS
			print("RIGHT X AXIS")
			send_control(event)
		if event.code == 5: #RIGHT JOYSTICK Y AXIS
			print("RIGHT Y AXIS")
			send_control(event)
		if event.code == 16: #D-PAD X AXIS
			print("D-PAD X AXIS")
			send_control(event)
		if event.code == 17: #D-PAD Y AXIS
			print("D-PAD Y AXIS")
			send_control(event)
