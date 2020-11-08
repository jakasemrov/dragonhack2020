import datetime
import random
import serial
import json
import requests
import time

server = "http://192.168.1.27:300/data/postData"
ser = serial.Serial('/dev/ttyACM0', 9600)
init = 5
while init > 0:
	out = ser.readLine()
while True:
	arduino_out = ser.readline()
	output_table = arduino_out.split(" ")
	light = output_table(1)
	humidity = output_table(3)
	temperature = output_table(5)
	noise = random.randInt(50, 70)
	tempJson = {
		"temperature": temperature,
		"humidity": humidity,
		"brightness": light,
		"noise": noise
	}
	sendJson = json.dumps(tempJson)
	print(sendJson)
	print(datetime.datetime.now())
	headers = {'Content-type': 'application/json'}
	try:
		send = requests.post(url = sever, data=sendJson, headers=headers, timeout=1)
		print(send)
	except requests.exceptions.Timeout:
		time.sleep(300)
		continue
	time.sleep(300)