#include <iostream>
#include "cubepos.h"
#include <unistd.h>

using namespace std;

void print_cp(cubepos &cp){
	cout << "\t";
	cout << "corner";
	for (int i = 0; i < 8; i++)
	{  
		cout << (int)(cp.c[i]) << " ";
	}
	cout << "edge ";
	for (int i = 0; i < 12; i++){
		cout << (int)(cp.e[i]) << " ";
	}
	cout << endl;
}

void print_ms(moveseq &ms){
	cout << "\t";
	cout << "moveseq ";
	for (int i = 0; i < ms.size(); ++i)
	{
		cout << ms[i] << " ";
	}
	cout << endl;
}

int main(int argc, char *argv[]) {
	cubepos cp, cp2,cp3;

	if (lrand48() == 0)
		srand48(getpid() + time(0));

	cout << endl << "== a movepc should undo a move ==" << endl;
	cout << "== testing cp.move, cp.movepc ==" << endl;
	for (int i = 0; i < NMOVES; i++)
	{
		cout << endl << "== move " << cp.moves[i] << " ==" <<endl;
		cout << "cube" << endl;
		print_cp(cp);
		cp.move(i);
		cout << "cube after move" << endl;
		print_cp(cp);
		cp.movepc(i);
		cout << "cube after movepc" << endl;
		print_cp(cp);
	}

	cout << endl << "== do same move 4 times will return it to the original ==" << endl;
	cout << "== testing cp.move ==" << endl;
	for (int i = 0;i<FACES; i++){
		cout << endl << "== case " << i << "==" << endl;
		cout << "cube before move" << endl;
		print_cp(cp);
		for (int j = 0; j < 4;j++) {
			cout << "move " << cp.moves[i * TWISTS] << endl;
			cp.move(i * TWISTS);
		}
		cout << "cube after 4 moves" << endl;
		print_cp(cp);
	}

	cout << endl << "== invert cube from move seqs equals cube from inverted move seq ==" << endl;
	cout << "== testing cubepos::random_moveseq, cubepos::invert_sequence cp.invert_into ==" << endl;
	for (int i = 0; i < 10;i++){
		cout << endl << "== random case " << i << "==" << endl;
		moveseq ms = cubepos::random_moveseq(10);
		moveseq ms_inverted = cubepos::invert_sequence(ms);
		print_ms(ms);
		cout << "inverted move sequence" << endl;
		print_ms(ms_inverted);
		cp = identity_cube;
		cp2 = identity_cube;
		for (int k = 0; k < ms.size(); ++k)
		{
			cp.move(ms[k]);
			cp2.move(ms_inverted[k]);
		}
		cp.invert_into(cp3);
		cout << "cube after move sequence" << endl;
		print_cp(cp);
		cout << "invert cube after move sequence" << endl;
		print_cp(cp3);
		cout << "cube after inverted move sequence" << endl;
		print_cp(cp2);
	}
}
