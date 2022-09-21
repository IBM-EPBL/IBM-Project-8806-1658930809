import random
import time
from playsound import playsound
while True:
    randomTempVal = random.randrange(50,100)
    randomHumidVal = random.randrange(1,100)
    print("Temperature inside house : " + str(randomTempVal))
    print("Humidity inside the house : " + str(randomHumidVal) + " %")
    if(randomTempVal>50):
        print("Alaram Trigered")
        playsound('emergency.mp3')
    else:
        pass
    time.sleep(5)