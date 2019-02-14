all:
	g++ -std=c++11 -pthread square.cpp board.cpp agent.cpp learn.cpp genEval.cpp -o startLearn
	g++ -std=c++11 square.cpp board.cpp agent.cpp playGUI.cpp -o playGUI
	gcc clean_to_zero.c -o clean
	gcc doublefork.c -o df
genKifu:
	g++ -std=c++11 -pthread square.cpp board.cpp agent.cpp learn.cpp genKifu.cpp -o genKifu
playGUI:
	g++ -std=c++11 square.cpp board.cpp agent.cpp playGUI.cpp -o playGUI
clean:
	gcc clean_to_zero.c -o clean
df:
	gcc doublefork.c -o df
del:
	rm -f playGUI startLearn df clean
