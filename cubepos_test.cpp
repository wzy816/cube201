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
	cubepos cp, cp2, cp3, cp4;

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
		cout << endl << "== case " << i << " ==" << endl;
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
		cout << endl << "== random case " << i << " ==" << endl;
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

	cout << endl << "== cube move from seq a and b equals r from mul(a,b,r) ==" << endl;
	cout << "== testing cubepos::mul == " << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << endl << "== random case " << i << " ==" << endl;
		moveseq ms1 = cubepos::random_moveseq(10);
		print_ms(ms1);
		moveseq ms2 = cubepos::random_moveseq(10);
		print_ms(ms2);
		cp = identity_cube;
		cp2 = identity_cube;
		cp3 = identity_cube;
		for (int j = 0; j < ms1.size();++j){
			cp2.move(ms1[j]);
			cp.move(ms1[j]);
		}
		cout << "cube after ms1" << endl;
		print_cp(cp);
		for (int j = 0; j < ms2.size(); ++j)
		{
			cp3.move(ms2[j]);
			cp.move(ms2[j]);
		}
		cout << "cube after ms2" << endl;
		print_cp(cp);
		cubepos::mul(cp2, cp3, cp4);
		cout << "cube from mul cp2 from ms1 and cp3 from ms2 separately" << endl;
		print_cp(cp4);
	}

	cout << endl << "== cube movepc from seq a and b equals r from mulpc(a,b,r) ==" << endl;
	cout << "== testing cubepos::mulpc == " << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << endl << "== random case " << i << " ==" << endl;
		moveseq ms1 = cubepos::random_moveseq(10);
		print_ms(ms1);
		moveseq ms2 = cubepos::random_moveseq(10);
		print_ms(ms2);
		cp = identity_cube;
		cp2 = identity_cube;
		cp3 = identity_cube;
		for (int j = 0; j < ms1.size();++j){
			cp2.movepc(ms1[j]);
			cp.movepc(ms1[j]);
		}
		cout << "cube after ms1" << endl;
		print_cp(cp);
		for (int j = 0; j < ms2.size(); ++j)
		{
			cp3.movepc(ms2[j]);
			cp.movepc(ms2[j]);
		}
		cout << "cube after ms2" << endl;
		print_cp(cp);
		cubepos::mulpc(cp2, cp3, cp4);
		cout << "cube from mul cp2 from ms1 and cp3 from ms2 separately" << endl;
		print_cp(cp4);
	}

	cout << endl << "== use append to convert move seq to char[], use parse to convert char[] back to move seq==" << endl;
	cout << "== testing cubepos::append_moveseq, cubepos::parse_moveseq ==" << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << endl << "== random case " << i << " ==" << endl;
		moveseq ms1 = cubepos::random_moveseq(10);
		cout << "original move seq" << endl;
		print_ms(ms1);
		char buf[20]; // 10 steps took 20 char
		char *p1 = buf;
		cubepos::append_moveseq(p1, ms1);
		cout << "print move string" << endl;
		for (int j = 0; j < 20;++j){
			cout << buf[j] << " ";
		}
		cout << endl;
		const char *p2 = buf;
		cout << "move seq from parse" << endl;
		moveseq ms2 = cubepos::parse_moveseq(p2);
		print_ms(ms2);
	}

	cout << endl << "== testing generating and parsing singamster string ==" << endl;
	cout << "== testing cp.Singmaster_string, cp.parse_Singmaster ==" << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << endl << "== random case " << i << " ==" << endl;
		cp = identity_cube;
		cp.randomize();
		cout << "print cp after randomize" << endl;
		char buf[100];
		strcpy(buf, cp.Singmaster_string());
		print_cp(cp);
		cout << "print cp singamaster string after randomize" << endl;
		for (int j = 0; j < 100;++j){
			cout << buf[j] << " ";
		}
		cout << endl;
		cp2.parse_Singmaster(buf);
		cout << "cp2 from parseing cp's singamaster string" << endl;
		print_cp(cp2);
	}
	
	cout << endl << "== remap a cube after move seq equals apply remap on each move ==" <<endl;
	cout << "== testing cubepos::move_map, cp.remap_into ==" << endl;
	for (int i = 0; i < 10; i++)
	{
		int mirror_index = (int)(M * drand48());
		cout << endl << "== case " << i << " mirror index=" << mirror_index << " ==" << endl;
		moveseq ms = cubepos::random_moveseq(10);
		print_ms(ms);
		cp = identity_cube;
		cp2 = identity_cube;
		for (int j = 0; j < ms.size();++j){
			cp.move(ms[j]);
			cp2.move(cubepos::move_map[mirror_index][ms[j]]);
		}
		cout << "cp2 after remap each move" << endl;
		print_cp(cp2);

		cout << "cp before remap into" << endl;
		print_cp(cp);
		cout << "cp after remap into" << endl;
		cp.remap_into(mirror_index, cp3);
		print_cp(cp3);
	}
}
