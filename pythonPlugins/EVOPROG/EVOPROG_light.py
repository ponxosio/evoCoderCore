import time
from light import Light

class EvoprogLight(Light):
	def __init__(self, params):
		"""constructor"""
		self.wavelengthPin = int(param["wavelengthPin"])
		self.intensityPin = int(param["intensityPin"])

	
	@classmethod
	def getParamsType(cls):
		"""must return a list with the types expected at the params variable in the init function"""
		return {"wavelengthPin":"int", "intensityPin":"int"}

	def getInstructions(self):
		""" must return a string with the instructions to make this component"""
		return ""

	def applyLigth(self, wavelength, intensity, communications):
		"""must send instructions to apply a ligth with the given real numbers wavelength and intensity, 
		communication with the machine must be done via communications object
		ommunications object has the next api:
				*) nbytessend sendString(string) -- send the string to the machine, return the bytes sended;
				*) string receiveString() -- receive and returns a string from the machine (stops when \n is received), can block;
				*) string readUntil(endCharacter) -- returns a string received from the machine, stops when the endCharacter arrives;
				*) void synch() -- synchronize with the machine, not always necesary, only for protocols compatibles;
		"""
		commandWave = "PWD " + str(self.wavelengthPin) + " " + "255" if wavelength > 255 else round(abs(wavelength))
		commandIntensity = "PWD " + str(self.intensityPin) + " " + "255" if intensity > 255 else round(abs(intensity))
		
		communications.sendString(commandWave)
		time.sleep(0.01)
		communications.sendString(commandIntensity)
		time.sleep(0.01)
		communications.synch()