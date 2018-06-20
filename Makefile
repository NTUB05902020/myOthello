all:
	g++ -std=c++11 -pthread learn.cpp agent.cpp board.cpp square.cpp -o learn
	g++ -std=c++11 playGUI.cpp agent.cpp board.cpp square.cpp -o playGUI
learn:
	g++ -std=c++11 learn.cpp agent.cpp board.cpp square.cpp -o learn
playGUI:
	g++ -std=c++11 playGUI.cpp agent.cpp board.cpp square.cpp -o playGUI
clean:
	rm -f playGUI learn
