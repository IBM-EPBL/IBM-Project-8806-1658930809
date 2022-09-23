import random
import time
from playsound import playsound
while True:
    TempVal = random.randrange(50,100)
    HumidVal = random.randrange(1,100)
    print("Temperature inside house : " + str(TempVal))
    print("Humidity inside the house : " + str(HumidVal) + " %")
    if(TempVal>50):
        print("Alaram Trigered")
        playsound('alert.wav')
    else:
        pass
    time.sleep(5)