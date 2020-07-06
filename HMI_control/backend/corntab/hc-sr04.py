#!/usr/bin/python3
from hcsr04sensor import sensor
from datetime import datetime
import time

TRIGGER = 23
ECHO = 24


def get_distance():
    sr04 = sensor.Measurement(TRIGGER, ECHO)
    sr04.temperature = 30
    measurement = 0
    while True:
        try:
            measurement = sr04.raw_distance()
            # more than 8 meters is impossible
            if(measurement > 800):
                raise RuntimeError("HC-SR04 signal lost\n")
            else:
                break
        except Exception:
            """Error is too frequenctly"""
            #now = datetime.now()
            #current_time = now.strftime("%Y-%m-%d-%H-%M-%S")
            #print_2_file(current_time + ": " + str(message) + "\n", "a")
            pass
        finally:
            time.sleep(30)
    return measurement


if __name__ == '__main__':
    print(get_distance())
