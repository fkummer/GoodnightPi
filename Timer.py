import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)

TRIG = 21
ECHO = 20

# set the trigger pin to output
GPIO.setup(TRIG, GPIO.OUT)

# set the echo pin to input
GPIO.setup(ECHO, GPIO.IN)

GPIO.output(TRIG, False)
print "waiting for sensor"
time.sleep(2)

GPIO.output(TRIG, True)
time.sleep(0.00001)
GPIO.output(TRIG, False)

while GPIO.input(ECHO) == 0:
	pulse_start = time.time()
	
while GPIO.input(ECHO) == 1:
	pulse_end = time.time()
	
pulse_duration = (pulse_start - pulse_end) * -1

distance = pulse_duration * 17150

distance = round(distance, 2)

print "Distance:",distance,"cm"

GPIO.cleanup()

target = open('sensor_data.txt', 'a')

print "Writing to file"

file_string = "%f\n" % distance

target.write(file_string)

target.close()
	

