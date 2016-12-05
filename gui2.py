import pygame
import RPi.GPIO as GPIO
import os
import time


pygame.init()


size = width, height = 320,240

WHITE = (255,255,255)
BLACK = (0,0,0)
RED   = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE =  (0, 0, 255)

screen = pygame.display.set_mode(size)

pygame.mouse.set_visible(True)

font_small = pygame.font.Font(None,25)
font_info  = pygame.font.Font(None,20)


text_surface_box = font_small.render("Time Interval:",True, WHITE)
rect_box = text_surface_box.get_rect(center=(65,150))
screen.blit(text_surface_box, rect_box)

text_surface_quit = font_small.render("Quit",True, BLUE)
rect_quit = text_surface_quit.get_rect(center=(40,220))
screen.blit(text_surface_quit, rect_quit)

text_surface_info = font_info.render("up/down keys to iterate and enter to select",True, WHITE)
rect_info = text_surface_info.get_rect(center=(150,170))
screen.blit(text_surface_info, rect_info)

text_surface_event = font_small.render("********Event********",True, WHITE)
rect_event = text_surface_event.get_rect(center=(150,20))
screen.blit(text_surface_event, rect_event)

text_surface_time = font_small.render("Interrupt 0 ",True, BLUE)
rect_time = text_surface_time.get_rect(center=(80,40))
screen.blit(text_surface_time, rect_time)

text_surface_external = font_small.render("Interrupt 1",True, BLUE)
rect_external = text_surface_external.get_rect(center=(250,40))
screen.blit(text_surface_external, rect_external)

text_surface_ping = font_small.render("Ping",True, BLUE)
rect_ping = text_surface_ping.get_rect(center=(30,90))
screen.blit(text_surface_ping, rect_ping)

text_surface_pingresp = font_small.render("PING SUCCESS!!!",True, RED)
rect_pingresp = text_surface_pingresp.get_rect(center=(150,90))
screen.blit(text_surface_pingresp, rect_pingresp)

text_surface_configresp = font_small.render("CONFIG SUCCESS!!!",True, RED)
rect_configresp = text_surface_configresp.get_rect(center=(100,200))
screen.blit(text_surface_configresp, rect_configresp)

def eraseLine(spaces, loc):
	text_surface_erase = font_small.render(spaces,True, RED)
	rect_erase = text_surface_erase.get_rect(center=loc)
	screen.blit(text_surface_erase, rect_erase)


def printTimeInterval(key):
	text_surface_text = font_small.render(key,True, GREEN)
	rect_text = text_surface_text.get_rect(center=(150,150))
	screen.blit(text_surface_text, rect_text)

		
times = ["1min","2min","5min","10min","15min","30min","1hr","2hr","5hr","10hr","15hr","20hr","24hr"]
number = 0
up = 0
minutes = {"1min"  : 60,
		   "2min"  : 120,
		   "5min"  : 300,
		   "10min" : 600,
		   "15min" : 900,
		   "30min" : 1800,
		   "1hr"   : 3600,
		   "2hr"   : 7200,
		   "5hr"   : 18000,
		   "10hr"  : 36000,
		   "15hr"  : 54000,
		   "20hr"  : 72000,
		   "24hr"  : 86400,
		   }
ping = 0
time1=0
interval = 0
time2 = 0
		   
while True:
	screen.fill(BLACK)
	screen.blit(text_surface_box, rect_box)
	screen.blit(text_surface_quit, rect_quit)
	screen.blit(text_surface_info, rect_info)
	screen.blit(text_surface_event, rect_event)
	screen.blit(text_surface_external, rect_external)
	screen.blit(text_surface_time, rect_time)
	screen.blit(text_surface_ping, rect_ping)
	printTimeInterval(str(times[up]))
	
	
	if(ping == 1):
		fo = open("ping.txt", "r+")
		read = fo.read(13);
		if(read == "Ping Success!"):
			print "sucessfully read from file"
			fo.close()
			fo = open("ping.txt", "w")
			fo.write("             ")
			fo.close()
			time1 = time.time()
			
	if(ping == 1):
		if(time.time() - time1 > 5):
			eraseLine("             ", (150,90))
			ping = 0
		else:
			screen.blit(text_surface_pingresp, rect_pingresp)
			
	if(interval == 1):
		fo = open("config.txt", "r+")
		read = fo.read(17);
		if(read == "Interval Success!"):
			print "sucessfully read from file"
			fo.close()
			fo = open("config.txt", "w")
			fo.write("                 ")
			fo.close()
			time2 = time.time()
			
	if(interval == 1):
		if(time.time() - time2 > 5):
			eraseLine("                 ", (100,200))
			ping = 0
		else:
			screen.blit(text_surface_configresp, rect_configresp)
			
			
		
		
	
	for event in pygame.event.get():
		if(event.type is pygame.MOUSEBUTTONDOWN):
			pos = pygame.mouse.get_pos()
			x,y = pos
			
			if y>200 and 20<x and x<60:
				exit()
				
			if (y>30 and y<50) and (x>24 and x<60):
				os.system('sudo ./guiMain interrupt0')
				
			
			if (y>30 and y<50) and (x>224 and x<284):
				os.system('sudo ./guiMain interrupt1')
				
			if (y>83 and y<97) and (x>27 and x<59):
				os.system('sudo ./guiMain ping')
				ping = 1
								
				
		if(event.type is pygame.KEYDOWN):
			
			if(event.key == 273):
				up = up + 1
				if(up > 12):
					up = 0;
			elif(event.key == 274):
				up = up - 1
				if(up < 0):
					up = 12
			elif(event.key == 13):
				number = minutes[times[up]]
				print number
				string = 'sudo ./guiMain' + " " + "interval" + " " + str(number)
				os.system(string)
				interval = 1
			
			
		

	pygame.display.flip()



