import time
from temperature import Temperature

class EvoprogTemperature(Temperature):

	def __init__(self, params):
		"""constructor"""
		self.pinNumber = int(params["pinNumber"])

	@classmethod
	def getParamsType(cls):
		"""must return a list with the types expected at the params variable in the init function"""
		return {"pinNumber":"int"}

	def getInstructions(self):
		""" must return a string with the instructions to make this component"""
		return ""

	def apply(self, degrees, communications):
		"""must send instructions to change the temperature to the given real number degrees, 
		communication with the machine must be done via communications object
		ommunications object has the next api:
				*) nbytessend sendString(string) -- send the string to the machine, return the bytes sended;
				*) string receiveString() -- receive and returns a string from the machine (stops when \n is received), can block;
				*) string readUntil(endCharacter) -- returns a string received from the machine, stops when the endCharacter arrives;
				*) void synch() -- synchronize with the machine, not always necesary, only for protocols compatibles;
		"""
		command = "PWD " + str(self.pinNumber) + " " + "255" if abs(degress) > 255 else round(abs(degress))
		communications.sendString(command)
		time.sleep(0.01)
		communications.synch()