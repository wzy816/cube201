#include "cubepos.h"

#include <unistd.h>

#include <bitset>
#include <iostream>

using namespace std;

// generate random move sequence
moveseq random_moveseq(int size) {
  moveseq r;
  for (int i = -2; i < size; i++) r.push_back(NMOVES * drand48());
  return r;
}

// print move seq
void print_ms(moveseq &ms) {
  cout << "\t";
  cout << "moveseq ";
  for (int i = 0; i < ms.size(); ++i) {
    cout << ms[i] << " ";
  }
  cout << endl;
}

int main(int argc, char *argv[]) {
  cubepos cp, cp2, cp3, cp4;

  // cp.move
  // cp.movepc
  cout << endl << "== a movepc should undo a move ==" << endl;
  for (int i = 0; i < NMOVES; i++) {
    cout << endl << "== move " << cp.moves[i] << " ==" << endl;
    cout << "cube" << endl;
    cp.show();
    cp.move(i);
    cout << "cube after move" << endl;
    cp.show();
    cp.movepc(i);
    cout << "cube after movepc" << endl;
    cp.show();
  }

  // cp.move
  cout << endl
       << "== do same move 4 times will return it to the original ==" << endl;
  for (int i = 0; i < FACES; i++) {
    cout << endl << "== case " << i << " ==" << endl;
    cout << "cube before move" << endl;
    cp.show();
    for (int j = 0; j < 4; j++) {
      cout << "move " << cp.moves[i * TWISTS] << endl;
      cp.move(i * TWISTS);
    }
    cout << "cube after 4 moves" << endl;
    cp.show();
  }

  // cubepos::invert_sequence
  // cp.invert_into
  cout << endl
       << "== invert cube from move seqs equals cube from inverted move seq =="
       << endl;
  for (int i = 0; i < 10; i++) {
    cout << endl << "== random case " << i << " ==" << endl;
    moveseq ms = random_moveseq(10);
    moveseq ms_inverted = cubepos::invert_sequence(ms);
    print_ms(ms);
    cout << "inverted move sequence" << endl;
    print_ms(ms_inverted);
    cp = identity_cube;
    cp2 = identity_cube;
    for (int k = 0; k < ms.size(); ++k) {
      cp.move(ms[k]);
      cp2.move(ms_inverted[k]);
    }
    cp.invert_into(cp3);
    cout << "cube after move sequence" << endl;
    cp.show();
    cout << "invert cube after move sequence" << endl;
    cp3.show();
    cout << "cube after inverted move sequence" << endl;
    cp2.show();
  }

  // cubepos::mul
  cout << endl
       << "== cube move from seq a and b equals r from mul(a,b,r) ==" << endl;
  for (int i = 0; i < 10; i++) {
    cout << endl << "== random case " << i << " ==" << endl;
    moveseq ms1 = random_moveseq(10);
    print_ms(ms1);
    moveseq ms2 = random_moveseq(10);
    print_ms(ms2);
    cp = identity_cube;
    cp2 = identity_cube;
    cp3 = identity_cube;
    for (int j = 0; j < ms1.size(); ++j) {
      cp2.move(ms1[j]);
      cp.move(ms1[j]);
    }
    cout << "cube after ms1" << endl;
    cp.show();
    for (int j = 0; j < ms2.size(); ++j) {
      cp3.move(ms2[j]);
      cp.move(ms2[j]);
    }
    cout << "cube after ms2" << endl;
    cp.show();
    cubepos::mul(cp2, cp3, cp4);
    cout << "cube from mul cp2 from ms1 and cp3 from ms2 separately" << endl;
    cp4.show();
  }

  // cubepos::mulpc
  cout << endl
       << "== cube movepc from seq a and b equals r from mulpc(a,b,r) =="
       << endl;
  for (int i = 0; i < 10; i++) {
    cout << endl << "== random case " << i << " ==" << endl;
    moveseq ms1 = random_moveseq(10);
    print_ms(ms1);
    moveseq ms2 = random_moveseq(10);
    print_ms(ms2);
    cp = identity_cube;
    cp2 = identity_cube;
    cp3 = identity_cube;
    for (int j = 0; j < ms1.size(); ++j) {
      cp2.movepc(ms1[j]);
      cp.movepc(ms1[j]);
    }
    cout << "cube after ms1" << endl;
    cp.show();
    for (int j = 0; j < ms2.size(); ++j) {
      cp3.movepc(ms2[j]);
      cp.movepc(ms2[j]);
    }
    cout << "cube after ms2" << endl;
    cp.show();
    cubepos::mulpc(cp2, cp3, cp4);
    cout << "cube from mul cp2 from ms1 and cp3 from ms2 separately" << endl;
    cp4.show();
  }

  // cubepos::append_moveseq
  // cubepos::parse_moveseq
  cout << endl
       << "== use append to convert move seq to char[], use parse to convert "
          "char[] back to move seq=="
       << endl;
  for (int i = 0; i < 10; i++) {
    cout << endl << "== random case " << i << " ==" << endl;
    moveseq ms1 = random_moveseq(10);
    cout << "original move seq" << endl;
    print_ms(ms1);
    char buf[20];  // 10 steps took 20 char
    char *p1 = buf;
    cubepos::append_moveseq(p1, ms1);
    cout << "print move string" << endl;
    for (int j = 0; j < 20; ++j) {
      cout << buf[j] << " ";
    }
    cout << endl;
    const char *p2 = buf;
    cout << "move seq from parse" << endl;
    moveseq ms2 = cubepos::parse_moveseq(p2);
    print_ms(ms2);
  }
  // cp.Singmaster_string
  // cp.parse_Singmaster
  cout << endl
       << "== testing generating and parsing singamster string ==" << endl;
  for (int i = 0; i < 10; i++) {
    cout << endl << "== random case " << i << " ==" << endl;
    cp = identity_cube;
    cp.randomize();
    cout << "print cp after randomize" << endl;
    char buf[100];
    strcpy(buf, cp.Singmaster_string());
    cp.show();
    cout << "print cp singamaster string after randomize" << endl;
    for (int j = 0; j < 100; ++j) {
      cout << buf[j] << " ";
    }
    cout << endl;
    cp2.parse_Singmaster(buf);
    cout << "cp2 from parseing cp's singamaster string" << endl;
    cp2.show();
  }

  // cubepos::move_map
  // cp.remap_into
  cout << endl
       << "== remap a cube after move seq equals apply remap on each move =="
       << endl;
  for (int i = 0; i < 10; i++) {
    int mirror_index = (int)(M * drand48());
    cout << endl
         << "== case " << i << " mirror index=" << mirror_index
         << " ==" << endl;
    moveseq ms = random_moveseq(10);
    print_ms(ms);
    cp = identity_cube;
    cp2 = identity_cube;
    for (int j = 0; j < ms.size(); ++j) {
      cp.move(ms[j]);
      cp2.move(cubepos::move_map[mirror_index][ms[j]]);
    }
    cout << "cp2 after remap each move" << endl;
    cp2.show();

    cout << "cp before remap into" << endl;
    cp.show();
    cout << "cp after remap into" << endl;
    cp.remap_into(mirror_index, cp3);
    cp3.show();
  }

  // cp.canon_into48
  cout << endl
       << "== remap a cube to any mirror won't change canon_into48 or "
          "canon_into96 result =="
       << endl;
  for (int i = 0; i < 10; i++) {
    cout << endl << "== random case " << i << " ==" << endl;
    cp = identity_cube;
    cp2 = identity_cube;
    cp3 = identity_cube;
    cp4 = identity_cube;
    moveseq ms1 = random_moveseq(10);
    print_ms(ms1);
    for (int j = 0; j < ms1.size(); ++j) {
      cp.move(ms1[j]);
    }
    cout << "cp after move" << endl;
    cp.show();
    cp.canon_into48(cp2);
    cout << "cp after canon_into48" << endl;
    cp2.show();
    cp2 = identity_cube;
    cp.canon_into96(cp2);
    cout << "cp after canon_into96" << endl;
    cp2.show();
    for (int j = 0; j < 5; ++j) {
      int mirror_index = (int)(M * drand48());
      cp3 = identity_cube;
      cp4 = identity_cube;
      cp.remap_into(mirror_index, cp3);
      cout << "cp remapped to mirror_index=" << mirror_index << endl;
      cp3.show();
      cp3.canon_into48(cp4);
      cout << "after cannon_into48" << endl;
      cp4.show();
      cp4 = identity_cube;
      cp3.canon_into96(cp4);
      cout << "after cannon_into96" << endl;
      cp4.show();
    }
  }

  // canon_seq, canon_seq_mask
  cout << endl << "== show canon state arry ==" << endl;
  cp = identity_cube;
  cout << "canon_seq" << endl;
  for (int i = 0; i < CANONSEQSTATES; ++i) {
    for (int j = 0; j < NMOVES; ++j) {
      cout << (int)cubepos::canon_seq[i][j] << " ";
    }
    cout << endl;
  }
  cout << endl;
  cout << "canon_seq_mask" << endl;
  for (int i = 0; i < CANONSEQSTATES; ++i) {
    long long unsigned c = cubepos::canon_seq_mask[i];
    bitset<18> bitset1{c};
    cout << bitset1 << endl;
  }
  cout << endl;

  // (mask>>mv) & 1
  cout << endl
       << "== verify logic to use mask to prune invalid move ==" << endl;
  for (int i = 0; i < 10; i++) {
    cout << endl << "== case " << i;
    moveseq ms = random_moveseq(20);
    cout << endl;
    print_ms(ms);
    int canon_state = CANONSEQSTART;
    for (int j = 0; j < ms.size(); ++j) {
      int mv = ms[j];
      cout << "move " << mv << " " << cp.moves[mv] << " canon state "
           << canon_state << ", ";
      int mask = cp.cs_mask(canon_state);
      long long unsigned c = mask;
      bitset<18> bitset1{(long long unsigned)mask};
      cout << "mask " << bitset1 << ", ";
      bitset<18> bitset2{(long long unsigned)(mask >> mv)};
      cout << "mask>>mv => " << bitset2 << ", ";
      int valid = (mask >> mv) & 1;
      if (!valid) {
        cout << "(mask>>mv)&1 => " << valid << " => invalid" << endl;
        break;
      }
      cout << endl;
      canon_state = cp.next_cs(canon_state, mv);
    }
  }

  // cp.show
  cout << endl << "== test show ==" << endl;
  cout << "set to identity" << endl;
  int mv = 0;
  cp = identity_cube;
  cp.show();
  cout << cp.moves[mv] << endl;
  cp.move(mv);
  cout << "after move" << endl;
  cp.show();
}
