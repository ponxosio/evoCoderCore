import time
from control import Control

class Evoprog1WayValve(Control):
	def __init__(self, params):
		"""constructor"""
		super(Evoprog1WayValve, self).__init__(1)
		self.address = int(params["address"])
		self.map = {}
		self.lastPos = 0

	@classmethod
	def getParamsType(cls):
		"""must return a list with the types expected at the params variable in the init function"""
		dict = {}
		dict["address"] = "int"
		dict.update(super(Evoprog1WayValve, cls).getParamsType())
		return dict

	def getInstructions(self):
		""" must return a string with the instructions to make this component"""
		return ""

	def addConnection(self, idSource, idTarget, communications):
		"""
			must register a new connection between idSource container and idTarget container
		"""
		if self.lastPos < self.maxconnections :
			self.map[(idSource, idTarget)] = self.lastPos
			self.lastPos += 1
			

	def setConnection(self, idSource, idTarget, communications):
		"""
			must send instructions to the machine to allow connection between conatiners idSource and idTarget,
			the command must be sended throught the communications object,
			communications object has the next api:
				*) nbytessend sendString(string) -- send the string to the machine, return the bytes sended;
				*) string receiveString() -- receive and returns a string from the machine (stops when \n is received), can block;
				*) string readUntil(endCharacter) -- returns a string received from the machine, stops when the endCharacter arrives;
				*) void synch() -- synchronize with the machine, not always necesary, only for protocols compatibles;
		"""
		valvePos = 4
		if (idSource, idTarget) in self.map:
			valvePos = self.map[(idSource, idTarget)]	
		command = "M " + str(self.address) + " " + str(valvePos) + "\r"
		communications.sendString(command)
		time.sleep(0.01)
		communications.synch()

	def clearConnections(self):
		"""
			must removed all the connections added with addConnection
		"""
		self.map.clear()