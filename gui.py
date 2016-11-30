import pygame


pygame.init()


size = width, height = 320,240

WHITE = (255,255,255)

screen = pygame.display.set_mode(size)

pygame.mouse.set_visible(True)

font_small = pygame.font.Font(None,25)

menu_data = ('Time Interval', ('1min', '5min', '10min', '15min','30min','1hr','2hr','5hr','10hr','24hr',), 'Quit',)

PopupMenu(menu_data)

for e in pygame.event.get():
	if e.type == USEREVENT and e.code == 'MENU':
		print 'menu event: %s.%d: %s' % (e.name, e.item_id, e.text)
		if(e.name, e.text) == ('Main', 'Quit'):
			quit()
