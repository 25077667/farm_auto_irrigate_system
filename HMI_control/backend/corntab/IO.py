from serial import Serial
from time import sleep
from fcntl import flock, LOCK_EX, LOCK_NB, LOCK_UN
from random import random

ser = Serial('/dev/ttyS0', 9600, timeout=1)
prevGoal = ""


def sendGoal(msg):
    inWait = ser.in_waiting
    while inWait > 0:
        print("Wait for input buffer")
        trash = ser.readline()
        print(trash)
        inWait = ser.in_waiting
    print("Do Send!")
    ser.write(msg)
    sleep(0.1)
    print("Sent!")

def cleanCSV(line):
    newLine = ""
    for i in line:
        if i == ',' or i == '.' or i.isdigit():
            newLine += i
    return newLine

def readGoal():
    """
    Read raw file and send to Arduino
    last number is check sum(hash).
    """
    goalPath = '/var/www/html/Data/goal.csv'
    f = open(goalPath, 'r')

    try:
        flock(f, LOCK_EX | LOCK_NB)
        newGoal = f.readline()
        newGoal = cleanCSV(newGoal)
        if not prevGoal:
            prevGoal = newGoal
        elif prevGoal != newGoal:
            sendGoal(newGoal + '\n')
            prevGoal = newGoal

    except OSError:
        print('Goal locked')
        sleep(random())
    finally:
        flock(f, LOCK_UN)
        f.close()


def writeHistory(msg):
    historyPath = '/var/www/html/Data/history'
    f = open(historyPath, 'a')
    try:
        flock(f, LOCK_EX | LOCK_NB)
        f.write(msg+'\n')
    except OSError:
        print('Histroy locked')
        sleep(random())
    finally:
        flock(f, LOCK_UN)
        f.close()


def readSerial():
    inWait = ser.in_waiting
    if inWait > 0:
        print("Do receive")
        res = ser.readline()
	receive_time = str(res[:res.find(',')])
	#### NOT COMPELETE ABOUT TIME FORMATTING ####
        sleep(0.1)
        print("Write history")
        writeHistory(res)


if __name__ == "__main__":
    sleep(0.3)
    readGoal()
    sleep(0.2)
    readSerial()
