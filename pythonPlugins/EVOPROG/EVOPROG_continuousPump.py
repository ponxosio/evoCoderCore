import time
from extractor import Extractor

class EvoprogContinuousPump(Extractor):
	def __init__(self, params):
		"""constructor"""
		self.pinNumber = int(params["pin_number"]);

	@classmethod
	def getParamsType(cls):
		"""must return a list with the types expected at the params variable in the init function"""
		return {"pin_number":"int"}

	def getInstructions(self):
		""" must return a string with the instructions to make this component"""
		return ""

	def getMovementType(self):
		""" must return the movement type of this extractor:
			or Extractor.continuous or Extractor.discrete or Extractor.irrelevant
		"""
		return Extractor.continuous

	def extractLiquid(self, rate, communications):
		"""must send instructions via communications object to extract liquid from the container
			communications object has the next api:
				*) nbytessend sendString(string) -- send the string to the machine, return the bytes sended;
				*) string receiveString() -- receive and returns a string from the machine (stops when \n is received), can block;
				*) string readUntil(endCharacter) -- returns a string received from the machine, stops when the endCharacter arrives;
				*) void synch() -- synchronize with the machine, not always necesary, only for protocols compatibles;
		"""
		command = "DIO " + str(self.pinNumber) + " " + "1" if rate > 0 else "0"
		communications.sendString(command)
		time.sleep(0.05)
		communications.synch()