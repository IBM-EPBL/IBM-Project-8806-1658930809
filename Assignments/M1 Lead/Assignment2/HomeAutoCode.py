import random
import time
from playsound import playsound
while True:
    rTempVal = random.randrange(50,100)
    rHumidVal = random.randrange(1,100)
    print("Temperature inside house : " + str(rTempVal))
    print("Humidity inside the house : " + str(rHumidVal) + " %")
    if(rTempVal>50):
        print("Alaraming")
        playsound('Alarm.wav')
    else:
        pass
    time.sleep(5)