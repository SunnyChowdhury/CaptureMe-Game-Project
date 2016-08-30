//--------------------------------------------------                                                
// This project creates a game called CaptureMe 
// where two game pieces "Black and White" try to 
// trap each other and winner is decided when either 
// one completely traps the other or both pieces are 
// unable to move */
//--------------------------------------------------
//  Author:  Sunny Chowdhury    
//--------------------------------------------------


#include <iostream>	
#include <iomanip>
using namespace std;

#define MAX_ARRAY_SIZE 24
#define MIN_ARRAY_SIZE 8	

const int NOPLAYER = 0;
const int WHITEPLAYER = 1;
const int BLACKPLAYER = 2;
const int FIRST_PIECE = 1;
const int SECOND_PIECE = 2;

bool InitializeBoard(int** CaptureMeBoard, int numRowsInBoard);
bool DisplayBoard(int** CaptureMeBoard, int numRowsInBoard);
bool PlacePiece(int** CaptureMeBoard, int numRowsInBoard, int player, int squarePlayed, int whichPiece, int& numberCaptured);
bool CheckLine(int** CaptureMeBoard, int numRowsInBoard, int player, int squarePlayed, int xDirection, int yDirection, int& captured);
bool IsMove(int **CaptureMeBoard, int numRowsInBoard, int player, int whichPiece);
bool CheckSquare(int** CaptureMeBoard, int numRowsInBoard, int player, int iLocation, int jLocation, int xDirection, int yDirection);

int main()
{
	int **myCaptureBoard = NULL;
	int numRowsInBoard = 0;
	int numTries = 0;
	const int MAXTRIES = 2;
	int player = 1;
	const int MAX_TURNS = 3;
	int index = 0;
	int squareChosen = 0;
	int numCaptured = 0;
	char answer = 'N';

	while (numRowsInBoard < MIN_ARRAY_SIZE || numRowsInBoard > MAX_ARRAY_SIZE || numRowsInBoard % 2 != 0)
	{
		if (numTries > MAXTRIES)
		{
			cerr << "Too many errors entering the size of the board" << endl;
			system("pause");
			return 1;
		}
		else if (numTries != 0)
		{
			cout << MIN_ARRAY_SIZE << " <= number of squares <= " << MAX_ARRAY_SIZE << endl;
		}
		cout << "Enter the number of squares along each edge of the board: ";
		if (!(cin >> numRowsInBoard))
		{
			cerr << "ERROR: Board size is not an integer" << endl;
			cin.clear();
			cin.ignore(512, '\n');
		}
		else if (numRowsInBoard % 2 != 0)
		{
			cerr << "ERROR: Board size odd" << endl;
		}
		else if (numRowsInBoard < MIN_ARRAY_SIZE)
		{
			cerr << "ERROR: Board size too small" << endl;
		}
		else if (numRowsInBoard > MAX_ARRAY_SIZE)
		{
			cerr << "ERROR: Board size too large" << endl;
		}
		numTries++;
	}

	myCaptureBoard = NULL;
	myCaptureBoard = new int*[numRowsInBoard];

	for (int i = 0; i < numRowsInBoard; i++)
	{
		myCaptureBoard[i] = new int[numRowsInBoard];
	}

	if (!(InitializeBoard(myCaptureBoard, numRowsInBoard)))
	{
		cerr << "ERROR: Could not initialize the game board" << endl;
		system("pause");
		return 1;
	}

	if (!(DisplayBoard(myCaptureBoard, numRowsInBoard)))
	{
		cerr << "ERROR: Could not print the game board" << endl;
		system("pause");
		return 1;
	}

	for (int move = 1; move < numRowsInBoard*numRowsInBoard; move++)
	{
		// White makes a move
		if (move % 2 == 1)
		{
			player = 1;
			if (IsMove(myCaptureBoard, numRowsInBoard, player, FIRST_PIECE))
				cout << "White takes a turn " << endl;
			else
			{
				cout << "White is unable to move " << endl;
				if (!(IsMove(myCaptureBoard, numRowsInBoard, 2, FIRST_PIECE)))
				{
					cout << "Black is unable to move" << endl;
					cout << "GAME OVER" << endl;
					cout << "Enter any character to close the game";
					cin.get();
					exit(0);
				}
				continue;
			}
		}
		else
		{
			player = 2;
			if (IsMove(myCaptureBoard, numRowsInBoard, player, FIRST_PIECE))
				cout << "Black takes a turn " << endl;
			else
			{
				cout << "Black is unable to move " << endl;
				if (!(IsMove(myCaptureBoard, numRowsInBoard, 1, FIRST_PIECE)))
				{
					cout << "White is unable to move" << endl;
					cout << "GAME OVER" << endl;
					cout << "Enter any character to close the game";
					cin.get();
					exit(0);
				}
				continue;
			}
		}

		index = 0;
		while (index < MAXTRIES)
		{
			squareChosen = -1;
			numCaptured = 0;
			cout << "Enter square number where you want to put your first piece: ";
			if (!(cin >> squareChosen))
			{
				cin.clear();
				cin.sync();
				index++;
				cerr << "illegal move" << endl;
				continue;
			}
			else if (squareChosen > numRowsInBoard*numRowsInBoard || squareChosen < 0)
			{
				cerr << "ERROR: The chosen square is not on the board." << endl;
				cout << "Try again" << endl;
			}
			else if (myCaptureBoard[squareChosen / numRowsInBoard][squareChosen%numRowsInBoard] != 0)
			{
				cerr << "ERROR: The chosen square is already occupied" << endl;
				cout << "Try again" << endl;
			}
			else if ((PlacePiece(myCaptureBoard, numRowsInBoard, player, squareChosen, FIRST_PIECE, numCaptured)))
				break;

			index++;
		}
		if (index == MAXTRIES)
		{
			cerr << "Too many illegal moves" << endl;
			if (player == WHITEPLAYER)
				cout << "White has forfeited a turn" << endl;
			else if (player == BLACKPLAYER)
				cout << "Black has forfeited a turn" << endl;
			if (!(DisplayBoard(myCaptureBoard, numRowsInBoard)))
				cerr << "Board did not display correctly" << endl;

			continue;
		}

		if (!(DisplayBoard(myCaptureBoard, numRowsInBoard)))
			cerr << "Board did not display correctly" << endl;

		if (index <= MAXTRIES && numCaptured > 1)
		{
			numCaptured = 0;
			cout << "Do you wish to play a second piece?  y/n: ";
			cin >> answer;

			if (answer == 'y')
			{
				if (move % 2 == 1)
				{
					player = 1;
					if (!(IsMove(myCaptureBoard, numRowsInBoard, player, SECOND_PIECE)))
					{
						cout << "White is unable to move " << endl;
						if (!(IsMove(myCaptureBoard, numRowsInBoard, 2, SECOND_PIECE)))
						{
							cout << "Black is unable to move" << endl;
							cout << "GAME OVER" << endl;
							cout << "Enter any character to close the game";
							cin.get();
							exit(0);
						}
						continue;
					}
				}
				else
				{
					player = 2;
					if (!(IsMove(myCaptureBoard, numRowsInBoard, player, SECOND_PIECE)))
					{
						cout << "Black is unable to move " << endl;
						if (!(IsMove(myCaptureBoard, numRowsInBoard, 1, SECOND_PIECE)))
						{
							cout << "White is unable to move" << endl;
							cout << "GAME OVER" << endl;
							cout << "Enter any character to close the game";
							cin.get();
							exit(0);
						}
						continue;
					}
				}
				index = 0;
				while (index < MAXTRIES)
				{
					squareChosen = -1;
					numCaptured = 0;
					cout << "Enter square number where you want to put your second piece: ";
					if (!(cin >> squareChosen))
					{
						cin.clear();
						cin.sync();
						index++;
						continue;
					}
					else if (squareChosen > numRowsInBoard*numRowsInBoard || squareChosen < 0)
					{
						cerr << "ERROR: The chosen square is not on the board." << endl;
						cout << "Try again" << endl;
					}
					else if (myCaptureBoard[squareChosen / numRowsInBoard][squareChosen%numRowsInBoard] != 0)
					{
						cerr << "ERROR: The chosen square is already occupied" << endl;
						cout << "Try again" << endl;
					}
					else if ((PlacePiece(myCaptureBoard, numRowsInBoard,
						player, squareChosen, SECOND_PIECE, numCaptured)))
						break;

					index++;
				}
				if (index == MAXTRIES)
				{
					cerr << "Too many illegal moves\n";
					if (player == WHITEPLAYER)
						cout << "White has forfeited a turn" << endl;
					else if (player == BLACKPLAYER)
						cout << "Black has forfeited a turn" << endl;
				}

			}
			if (!(DisplayBoard(myCaptureBoard, numRowsInBoard)))
			{
				cerr << "Board did not display correctly" << endl;
			}
		}
		if (player == 1)
			player = 2;
		else
			player = 1;
	}

	// deallocate memory
	for (int i = 1; i < numRowsInBoard; i++)
	{
		delete[] myCaptureBoard[i];
	}
	delete[] myCaptureBoard;
	myCaptureBoard = NULL;


	system("pause");
	return 0;
}

bool InitializeBoard(int** CaptureMeBoard, int numRowsInBoard)
{
	if (numRowsInBoard < MIN_ARRAY_SIZE || numRowsInBoard > MAX_ARRAY_SIZE || numRowsInBoard % 2 != 0)
	{
		return false;
	}

	for (int row = 0; row < numRowsInBoard; row++)
	{
		for (int col = 0; col < numRowsInBoard; col++)
		{
			CaptureMeBoard[row][col] = 0;
		}
	}

	int midway = numRowsInBoard / 2;
	CaptureMeBoard[midway - 1][midway - 1] = WHITEPLAYER;
	CaptureMeBoard[midway - 1][midway] = BLACKPLAYER;
	CaptureMeBoard[midway][midway - 1] = BLACKPLAYER;
	CaptureMeBoard[midway][midway] = WHITEPLAYER;
	return true;
}

bool DisplayBoard(int** CaptureMeBoard, int numRowsInBoard)
{
	if (numRowsInBoard < MIN_ARRAY_SIZE || numRowsInBoard > MAX_ARRAY_SIZE || numRowsInBoard % 2 != 0)
	{
		cerr << "illegal number of rows in board" << endl;
		return false;
	}

	for (int row = 0; row < numRowsInBoard; row++)
	{
		for (int col = 0; col < numRowsInBoard; col++)
		{
			if (CaptureMeBoard[row][col] == 0)
			{
				cout << setw(4) << row*numRowsInBoard + col;
			}
			else if (CaptureMeBoard[row][col] == 2)
			{
				cout << setw(4) << "B";
			}
			else
			{
				cout << setw(4) << "W";
			}
		}
		cout << endl;
	}
	cout << endl << endl << endl;
	return true;
}

bool PlacePiece(int** CaptureMeBoard, int numRowsInBoard, int player, int squarePlayed, int whichPiece, int& numberCaptured)
{
	// Places game pieces and makes sure that the pieces are placed properly
	bool ifLegal = false;
	bool ifAdjacent = false;
	int numNotCaptured = 0;

	if (whichPiece == FIRST_PIECE)
	{
		// placing pieces in the vertical and horizontal locations
		ifAdjacent = CheckLine(CaptureMeBoard, numRowsInBoard, player, squarePlayed, 1, 0, numberCaptured);
		ifLegal = ifLegal || ifAdjacent;
		ifAdjacent = CheckLine(CaptureMeBoard, numRowsInBoard, player, squarePlayed, -1, 0, numberCaptured);
		ifLegal = ifLegal || ifAdjacent;
		ifAdjacent = CheckLine(CaptureMeBoard, numRowsInBoard, player, squarePlayed, 0, 1, numberCaptured);
		ifLegal = ifLegal || ifAdjacent;
		ifAdjacent = CheckLine(CaptureMeBoard, numRowsInBoard, player, squarePlayed, 0, -1, numberCaptured);
		ifLegal = ifLegal || ifAdjacent;
		if (ifLegal)
		{
			if (player == WHITEPLAYER)
				cout << "White has placed the first piece" << endl;
			if (player == BLACKPLAYER)
				cout << "Black has placed the first piece" << endl;
			return true;
		}
		else
		{
			cout << "Illegal move" << endl;
			return false;
		}
	}
	else
	{
		// placing pieces in diagonal positions
		ifAdjacent = CheckLine(CaptureMeBoard, numRowsInBoard, player, squarePlayed, 1, 1, numberCaptured);
		ifLegal = ifLegal || ifAdjacent;
		ifAdjacent = CheckLine(CaptureMeBoard, numRowsInBoard, player, squarePlayed, -1, 1, numberCaptured);
		ifLegal = ifLegal || ifAdjacent;
		ifAdjacent = CheckLine(CaptureMeBoard, numRowsInBoard, player, squarePlayed, 1, -1, numberCaptured);
		ifLegal = ifLegal || ifAdjacent;
		ifAdjacent = CheckLine(CaptureMeBoard, numRowsInBoard, player, squarePlayed, -1, -1, numberCaptured);
		ifLegal = ifLegal || ifAdjacent;
		if (ifLegal)
		{
			if (player == WHITEPLAYER)
				cout << "White has placed the second piece" << endl;
			if (player == BLACKPLAYER)
				cout << "Black has placed the second piece" << endl;
			return true;
		}
		else
		{
			cout << "Illegal move" << endl;
			return false;
		}
	}
}

bool CheckLine(int** CaptureMeBoard, int numRowsInBoard, int player, int squarePlayed, int xDirection, int yDirection, int& captured)
{
	// Index variable used for finding squares next the piece to be played
	int xIndex = 0;
	int yIndex = 0;
	// Coordiate variable used for finding the location of the piece to be placed
	int xCoordinate = 0;
	int yCoordinate = 0;

	int otherPlayer = 0;
	bool edge = true;

	otherPlayer = WHITEPLAYER;
	if (player == WHITEPLAYER)
	{
		otherPlayer = BLACKPLAYER;
	}

	// Row and column where the next piece is to be placed
	xCoordinate = squarePlayed % numRowsInBoard;
	yCoordinate = squarePlayed / numRowsInBoard;

	// Square next to the location of the piece to be played
	xIndex = xCoordinate + xDirection;
	yIndex = yCoordinate + yDirection;

	while ((xIndex < numRowsInBoard && yIndex < numRowsInBoard) && (xIndex >= 0 && yIndex >= 0))
	{
		// Need to switch the x and y indices to get the next location of the piece
		if (CaptureMeBoard[yIndex][xIndex] == NOPLAYER || CaptureMeBoard[yIndex][xIndex] == player)
		{
			return false;
		}
		else
		{
			edge = true;
			while ((xIndex < numRowsInBoard && yIndex < numRowsInBoard) && (xIndex >= 0 && yIndex >= 0))
			{
				edge = false;
				if (CaptureMeBoard[yIndex][xIndex] == NOPLAYER)
				{
					return false;
				}
				if (CaptureMeBoard[yIndex][xIndex] == player)
				{
					break;
				}
				else
				{
					xIndex += xDirection;
					yIndex += yDirection;
					edge = true;
				}
			}

			if (edge == true)
			{
				// the edge of the board has been reached 
				return false;
			}

			// legal move found, place the game piece
			CaptureMeBoard[yCoordinate][xCoordinate] = player;
			xIndex = xCoordinate + xDirection;
			yIndex = yCoordinate + yDirection;

			// change the color of the captured game pieces
			while (CaptureMeBoard[yIndex][xIndex] == otherPlayer)
			{
				CaptureMeBoard[yIndex][xIndex] = player;
				xIndex += xDirection;
				yIndex += yDirection;
				captured++;
			}
			return true;
		}
	}

	// Reached the edge of the board, not a legal move
	return false;
}

bool IsMove(int** CaptureMeBoard, int numRowsInBoard, int player, int whichPiece)
{
	if (whichPiece == FIRST_PIECE)
	{
		for (int row = 0; row < numRowsInBoard; row++)
		{
			for (int col = 0; col < numRowsInBoard; col++)
			{
				// checking the horizontal and vertical locations
				if (CheckSquare(CaptureMeBoard, numRowsInBoard, player, col, row, 0, 1))
					return true;
				if (CheckSquare(CaptureMeBoard, numRowsInBoard, player, col, row, 0, -1))
					return true;
				if (CheckSquare(CaptureMeBoard, numRowsInBoard, player, col, row, 1, 0))
					return true;
				if (CheckSquare(CaptureMeBoard, numRowsInBoard, player, col, row, -1, 0))
					return true;
			}
		}
	}
	else
	{
		for (int row = 0; row < numRowsInBoard; row++)
		{
			for (int col = 0; col < numRowsInBoard; col++)
			{
				// checking the diagonal locations
				if (CheckSquare(CaptureMeBoard, numRowsInBoard, player, col, row, 1, 1))
					return true;
				if (CheckSquare(CaptureMeBoard, numRowsInBoard, player, col, row, -1, 1))
					return true;
				if (CheckSquare(CaptureMeBoard, numRowsInBoard, player, col, row, 1, -1))
					return true;
				if (CheckSquare(CaptureMeBoard, numRowsInBoard, player, col, row, -1, -1))
					return true;
			}
		}
	}
	return false;
}

bool CheckSquare(int** CaptureMeBoard, int numRowsInBoard, int player, int iLocation, int jLocation, int xDirection, int yDirection)
{
	// Index variable used for finding squares next the piece to be played
	int xIndex = 0;
	int yIndex = 0;

	int otherPlayer = 0;
	bool edge = true;

	otherPlayer = WHITEPLAYER;
	if (player == WHITEPLAYER)
	{
		otherPlayer = BLACKPLAYER;
	}

	xIndex = iLocation + xDirection;
	yIndex = jLocation + yDirection;

	while ((xIndex < numRowsInBoard && yIndex < numRowsInBoard) && (xIndex >= 0 && yIndex >= 0))
	{
		// Need to switch the x and y indices to get the next location of the piece
		if (CaptureMeBoard[yIndex][xIndex] == NOPLAYER || CaptureMeBoard[yIndex][xIndex] == player)
		{
			return false;
		}
		else
		{
			edge = true;
			while ((xIndex < numRowsInBoard && yIndex < numRowsInBoard) && (xIndex >= 0 && yIndex >= 0))
			{
				edge = false;
				if (CaptureMeBoard[yIndex][xIndex] == NOPLAYER)
				{
					return false;
				}
				if (CaptureMeBoard[yIndex][xIndex] == player)
				{
					break;
				}
				else
				{
					xIndex += xDirection;
					yIndex += yDirection;
					edge = true;
				}
			}

			if (edge == true)
			{
				// the edge of the board has been reached 
				return false;
			}
			return true;
		}
	}
	// Reached the edge of the board, not a legal move
	return false;
}



