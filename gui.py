import pygame
import RPi.GPIO as GPIO


pygame.init()


size = width, height = 320,240

WHITE = (255,255,255)
BLACK = (0,0,0)
RED   = (255, 0, 0)

screen = pygame.display.set_mode(size)

pygame.mouse.set_visible(True)

font_small = pygame.font.Font(None,25)


text_surface_box = font_small.render("Time Interval:",True, WHITE)
rect_box = text_surface_box.get_rect(center=(65,150))
screen.blit(text_surface_box, rect_box)

text_surface_quit = font_small.render("Quit",True, WHITE)
rect_quit = text_surface_quit.get_rect(center=(40,220))
screen.blit(text_surface_quit, rect_box)

def printTimeInterval(key, loc, color):
	text_surface_text = font_small.render(key,True, color)
	rect_text = text_surface_text.get_rect(center=loc)
	screen.blit(text_surface_text, rect_text)

# ASCII to number mapping
numbers = {48 : 0,
		   49 : 1,
		   50 : 2,
		   51 : 3,
		   52 : 4,
		   53 : 5,
		   54 : 6,
		   55 : 7,
		   56 : 8,
		   57 : 9,
		   }

	
def convertAsciiToNumeral(ascii):
	if(ascii < 48 or ascii > 57):
		text = "INVALID INPUT"
	elif(ascii == 13):
		text = ascii
	else:
		text = numbers[ascii]
	
	return text
		
def eraseLine(length):
	loc = (10,170)
	for i in length:
		text_surface_erase = font_small.render(" ",True,WHITE)
		rect_erase = text_surface_erase.get_rect(center=loc)
		screen.blit(text_surface_erase, rect_erase)
		loc = (10+i,170)
	
userInput = 0
incLoc = 10
j=0
string = {}
while True:
	screen.fill(BLACK)
	screen.blit(text_surface_box, rect_box)
	screen.blit(text_surface_quit, rect_quit)
	if(userInput == 1):
		printTimeInterval(string[j], loc, color)
	
	for event in pygame.event.get():
		if(event.type is pygame.MOUSEBUTTONDOWN):
			pos = pygame.mouse.get_pos()
			x,y = pos
			
			if y>200 and 20<x and x<60:
				exit()
				
		if(event.type is pygame.KEYDOWN):
			userInput = 1
			print event.key
			number = convertAsciiToNumeral(event.key)
				
			if(number == "INVALID INPUT"):	
				loc = (80, 170)
				color = RED
				string[j] = number
			else:
				if(number == 13):
					eraseLine()
					j=0
					userInput = 0
				else:
					incLoc = incLoc + 1
					loc = (incLoc, 170)
					color = WHITE
					
				string[j] = str(number)
				j = j+1
			
		

	pygame.display.flip()



