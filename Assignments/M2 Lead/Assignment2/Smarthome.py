import random
import time
from playsound import playsound
while True:
    ranTempVal = random.randrange(50,100)
    ranHumidVal = random.randrange(1,100)
    print("Temperature inside house : " + str(ranTempVal))
    print("Humidity inside the house : " + str(ranHumidVal) + " %")
    if(ranTempVal>50):
        print("Alaram Trigered")
        playsound('Triger.wav')
    else:
        pass
    time.sleep(5)