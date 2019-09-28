"""Communicate from server to raspberry pi"""
import socket
import sys
import queue
import serial
import syslog
import time
import math
import threading
'''from TopsidesGlobals import GLOBALS
#import topsidesComms

# Change IP addresses for a production or development environment
if ((len(sys.argv) > 1) and (sys.argv[1] == "--dev")):
    ipSend = GLOBALS['ipSend-5-dev']
    ipHost = GLOBALS['ipHost-5-dev']
else:
    ipSend = GLOBALS['ipSend-4']
    ipHost = GLOBALS['ipHost']

portSend = GLOBALS['portSend-5']
portHost = GLOBALS['portHost']

received = queue.Queue()
# Try opening a socket for communication
try:
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
except socket.error:
    print("Failed To Create Socket")
    sys.exit()
except Exception as e:
    print("failed")
# Bind the ip and port of topsides to the socket and loop coms
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind((ipHost, portHost))

# Queue to hold send commands to be read by simulator
simulator = queue.Queue()'''


# This function sends data to the ROV
def sendData(inputData):
    global s
    #s.sendto(inputData.encode('utf-8'), ("192.168.88.5", portSend))

# TESTING
def sendDataB(inputData):
    global s
    #s.sendto(inputData.encode('utf-8'), (ipSend, portSend))

# This function is constantly trying to receive data from the ROV
def receiveData(flag):
    global s
    while True:
        outputData, addr = s.recvfrom(1024)
        outputData = outputData.decode("utf-8")
        if (outputData == "exit"):
            break
        received.put(outputData)
        if flag.is_set():
            break


def putMessage(msg):
    sendData(msg)
    simulator.put(msg, timeout=0.005)


def runThruster(tData):
    for control in tData:
        val = tData[control]
        putMessage("runThruster.py " + str(GLOBALS["thrusterPorts"][control]) + " " + str(val))
    print("good")


# TODO:
    # sway
    # yaw
    # pitch
    # heave
    # roll

def sway(power):
    good = False
    try:
        float(power)
        good = True
    except ValueError:
        good = False
        return False

    tData = {
        "fore-port-horz": power,
        "fore-star-horz": -power,
        "aft-port-horz": -power,
        "aft-star-horz": power,
    }
    print("Send command")
    runThruster(tData)

def yaw(power):
    good = False
    try:
        float(power)
        good = True
    except ValueError:
        good = False
        return False

    tData = {
        "fore-port-horz": power,
        "fore-star-horz": power,
        "aft-port-horz": power,
        "aft-star-horz": -power,
    }
    print("Send command")
    runThruster(tData)

def heave(power):
    good = False
    try:
        float(power)
        good = True
    except ValueError:
        good = False
        return False

    tData = {
        "fore-port-vert": power,
        "fore-star-vert": power,
        "aft-port-vert": -power,
        "aft-star-vert": -power,
    }
    print("Send command")
    runThruster(tData)

def pitch(power):
    good = False
    try:
        float(power)
        good = True
    except ValueError:
        good = False
        return False

    tData = {
        "fore-port-vert": power,
        "fore-star-vert": power,
        "aft-port-vert": power,
        "aft-star-vert": power,
    }
    print("Send command")
    runThruster(tData)

def roll(power):
    good = False
    try:
        float(power)
        good = True
    except ValueError:
        good = False
        return False

    tData = {
        "fore-port-vert": power,
        "fore-star-vert": -power,
        "aft-port-vert": -power,
        "aft-star-vert": power,
    }
    print("Send command")
    runThruster(tData)

def surge(power):
    good = False
    try:
        float(power)
        good = True
    except ValueError:
        good = False
        return False

    tData = {
        "fore-port-horz": power,
        "fore-star-horz": -power,
        "aft-port-horz": -power,
        "aft-star-horz": power,
    }
    print("Send command")
    runThruster(tData)
