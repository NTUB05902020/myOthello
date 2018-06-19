import pygame
import asyncio
import time
import select
#init screen
pygame.init()
GREEN = (0, 153, 51)
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
BROWN = (237, 217, 177)
screen = pygame.display.set_mode((500, 500))
screen.fill((237, 217, 177))
center = pygame.draw.rect(screen, GREEN, pygame.Rect(50, 50, 400, 400))
  #draw lines
for dist in range(50, 500, 50):
	pygame.draw.line(screen, BLACK, [dist, 50], [dist, 450])
	pygame.draw.line(screen, BLACK, [50, dist], [450, dist])

  #create index
font = pygame.font.Font(None, 30)
for num in range(0, 8):
	index = chr(ord('a') + num)
	text = font.render(index, True, BLACK)
	screen.blit(text, (70 + num * 50,20))
	text = font.render(str(num + 1), True, BLACK)
	screen.blit(text, (20, 70 + num * 50))


turn = 1#black piece for 1, white piece for 2
  #Scoreboard
pygame.draw.circle(screen, (0,0,0), (125,475), 15)
font = pygame.font.Font(None, 45)
text = font.render('2', True, BLACK)
screen.blit(text, (165, 462))
pygame.draw.circle(screen, WHITE, (325,475), 15)
text = font.render('2', True, BLACK)
screen.blit(text, (358, 462))

  #pieces
pygame.draw.circle(screen, WHITE, (225, 225), 15)
pygame.draw.circle(screen, WHITE, (275, 275), 15)
pygame.draw.circle(screen, BLACK, (275, 225), 15)
pygame.draw.circle(screen, BLACK, (225, 275), 15)
#================================#

BASE = 2#position of up-left corner
board = [ [0, 0, 0, 0, 0, 0, 0, 0, 0] for x in range(9) ]
board[4][4] = 2
board[5][5] = 2
board[4][5] = 1
board[5][4] = 1
pygame.display.flip()

def changeScore(black, white):#score of black
	global font
	#remove old score
	pygame.draw.rect(screen, BROWN, pygame.Rect(352, 452, 45, 45))
	pygame.draw.rect(screen, BROWN, pygame.Rect(152, 452, 45, 45))

	#set new score
	text = font.render(str(black), True, BLACK)
	if black >= 10:
		screen.blit(text, (158, 462))
	else:
		screen.blit(text, (165, 462))
	text = font.render(str(white), True, BLACK)
	if white >= 10:
		screen.blit(text, (358, 462))
	else:
		screen.blit(text, (365, 462))
	return black + white

X = [1, 1, 0, -1, -1, -1, 0, 1]
Y = [0, -1, -1, -1, 0, 1, 1, 1]
COLOR = (GREEN, BLACK, WHITE)



while True:
	r, w, e = select.select([0],[],[],0.3)
	
	if r:
		string = input()
		string = [ int(x) for x in string ]
		black, white = 0, 0
		for entry in range(64):
			row, col = entry // 8 + 1, entry % 8 + 1
			pygame.draw.circle(screen, COLOR[string[entry]], (25 + 50 * col, 25 + 50 * row), 15)
			if entry == 1:
				black += 1
			elif entry == 2:
				white += 1
		changeScore(black, white)
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			exit()



	pygame.display.flip()

