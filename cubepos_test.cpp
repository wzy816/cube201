#include<iostream>
#include "cubepos.h"
#include <unistd.h>

using namespace std;

void print(cubepos &cp){
	cout << "  corner";
	for (int i = 0; i < 8; i++)
	{
		cout << (int)(cp.c[i]) << " ";
	}
	cout << "  edge ";
	for (int i = 0; i < 12; i++){
		cout << (int)(cp.e[i]) << " ";
	}
	cout << endl;
}

bool is_identity(cubepos &cp){

  for (int i = 0; i < 8; i++){
		if(cp.c[i] != identity_cube.c[i]){
			return false;
		}
	}
  for (int i = 0; i < 12; i++){
		if(cp.e[i] != identity_cube.e[i]){
			return false;
		}
	}
	return true;
}

int main(int argc, char *argv[]) {
  cubepos cp, cp2, cp3, cp4, identity_cube;

	if (lrand48() == 0)
		srand48(getpid() + time(0));

	cout << endl << "==== print cp ====" << endl;
	print(cp);

	cout << endl << "==== a movepc should undo a move ====" << endl;
	for (int i = 0;i<NMOVES; i++){
		cout << "== move " << cp.moves[i] << " ==" <<endl;
		cout << "before" << endl;
		print(cp);
		cp.move(i);
		cout << "after move" << endl;
		print(cp);
		cp.movepc(i);
		cout << "after movepc" << endl;
		print(cp);
	}

	cout << endl << "===== do same move 4 times will return it to the original ====" << endl;
	for (int i = 0;i<FACES; i++){
		cout << "before" << endl;
		print(cp);
		for (int j = 0; j < 4;j++) {
			cout << "move " << cp.moves[i * TWISTS] << endl;
			cp.move(i * TWISTS);
		}
		cout << "after" << endl;
		print(cp);
	}
}