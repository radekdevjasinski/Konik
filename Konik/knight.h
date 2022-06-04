#include "lists.h"
#define BoardSize 5 /*!< Ilosc pol szachownicy w pionie i poziomie */
int xKnightPos = 0, /*!< Aktualizowany indeks x konia na szachownicy (od 0 do 4) */
yKnightPos = 0; /*!< Aktualizowany indeks y konia na szachownicy (od 0 do 4) */
bool animate = false; /*!< Odpowiedzialny za animowanie konia */
bool solved = false; /*!< Opisuje czy udalo sie rozwiazac trase */
int movesFound = 0; /*!< Opisuje ilosc znalezionych ruchow (najwiecej 25) */
int recursionsMade = 0; /*!< Opisuje ilosc wykonanych rekurencji */
/**
*@brief Funkcja podaje kolejny ruch, po podanym z listy,
* bool true = x, bool false = y
*/
int find_next(struct movesList* front, int moveid, bool x)
{
	for (; NULL != front; front = front->next) {
		if (front->moveId > moveid)
		{
			if (x)
				return front->x;
			else
				return front->y;
		}
	}
	return -1;
}
/**
*@brief Funkcja sprawadza czy pole znajduje sie na liscie zajetych pol
*/
bool Visited(struct movesList* front, int i, int j)
{
	for (; NULL != front; front = front->next) {
		if (front->x == i && front->y == j)
		{
			return true;
		}
	}
	return false;
}
/**
*@brief Rekurencyjna funkcja szukajaca trasy konika szachowego
*/
int TryTour(int x, int y, int move, int tour[BoardSize][BoardSize],
	int xMove[BoardSize], int yMove[BoardSize], int best[BoardSize][BoardSize], int mostMoves)
{
	recursionsMade++;
	int next_x, next_y;
	//czy to rozwiazanie?
	if (move == BoardSize * BoardSize)
		return 1;
	//zapisywanie najlepszego podejscia
	if (move > mostMoves) {
		for (int x = 0; x < BoardSize; x++) {
			for (int y = 0; y < BoardSize; y++)
				best[x][y] = tour[x][y];
		}
	}
	//sprawdzanie kazdego mozliwego ruchu
	for (unsigned k = 0; k < 8; k++) {
		next_x = x + xMove[k];
		next_y = y + yMove[k];
		if (isLegal(next_x, next_y, tour)) {
			tour[next_x][next_y] = move;
			//jesli legalny, wejdz w rekurencje z tym ruchem
			if (TryTour(next_x, next_y, move + 1, tour, xMove, yMove, best, mostMoves) == 1) {
				return 1;
			}
			else
				tour[next_x][next_y] = -1; // backtracking
		}
	}
	return 0;
}

/**
*@brief Funkcja sprawdza czy ruch jest legalny i wczesniej nieodwiedzony
*/
int isLegal(int x, int y, int tour[BoardSize][BoardSize])
{
	return (x >= 0 && x < BoardSize&& y >= 0 && y < BoardSize
		&& tour[x][y] == -1);
}
/**
*@brief Funkcja do wypisywania rozwiazania
*/
void print2dList(int tour[BoardSize][BoardSize])
{
	for (int y = 0; y < BoardSize; y++) {
		for (int x = 0; x < BoardSize; x++) {
			if (tour[x][y] == -1)
				printf(" XX ");
			else
				printf(" %2d ", tour[x][y]);
			if (tour[x][y] > movesFound)
				movesFound = tour[x][y];

		}
		printf("\n");
	}
}
/**
*@brief Funkcja zapisuje rozwiazanie z tablicy dwuwymiarowej do listy ruchów
*/
void translateList(int tour[BoardSize][BoardSize]) {
	movesToAnimate = create_list(0, xKnightPos, yKnightPos);
	for (int x = 0; x < BoardSize; x++) {
		for (int y = 0; y < BoardSize; y++) {
			if (tour[x][y] != 0 & tour[x][y] != -1)
			{
				movesToAnimate = insert_node(movesToAnimate, tour[x][y], x, y);
			}
		}
	}
	print_list(movesToAnimate);
}
/**
*@brief Glowna funkcja algorytmu konika szachowego, przygotowuje potrzebne zmienne, listy i uruchamia rekurencyjna funkcje TryTour()
*/
int knightsTour()
{
	//inicjacja szachownic 2d dla 
	int tour[BoardSize][BoardSize], best[BoardSize][BoardSize];
	for (int x = 0; x < BoardSize; x++)
		for (int y = 0; y < BoardSize; y++) {
			tour[x][y] = -1;
			best[x][y] = -1;
		}
	int mostMoves = 0;
	//lista mozliwych ruchow konia w kierunku x i y
	int xMove[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };
	int yMove[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };

	recursionsMade = 0;
	movesFound = 0;
	remove_list(&movesToAnimate);

	// zapisywanie startowej pozycji
	tour[xKnightPos][yKnightPos] = 0;
	// odwiedzenie wszystkich mozliwych tras i znalezienie rozwiazania
	if (TryTour(xKnightPos, yKnightPos, 1, tour, xMove, yMove, best, mostMoves) == 0) {
		printf("Unsolved :( \n");
		printf("Recursions: %d \n", recursionsMade);
		print2dList(best);
		translateList(best);
		solved = false;
		return 0;
	}
	else {
		printf("Recursions: %d \n", recursionsMade);
		print2dList(tour);
		translateList(tour);
		solved = true;
	}
	return 1;
}

