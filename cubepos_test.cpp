#include "cubepos.h"
#include <unistd.h>
#include <bitset>
#include <iostream>
#include <string>

using namespace std;

vector<int> generate_moveseq(int size)
{
  vector<int> r;
  for (int i = -2; i < size; i++)
    r.push_back(NMOVES * drand48());
  return r;
}

void check(const cubepos &cp1, const cubepos &cp2, string msg)
{
  if (cp1 == cp2)
    return;
  for (int i = 0; i < 8; i++)
    cout << " " << (int)(cp1.c[i]) << " " << (int)(cp2.c[i]) << endl;
  for (int i = 0; i < 12; i++)
    cout << " " << (int)(cp1.e[i]) << " " << (int)(cp2.e[i]) << endl;
  cout << msg << endl;
}

void check(vector<int> &ms1, vector<int> &ms2, string msg)
{
  if (ms1 == ms2)
    return;
  cout << msg << endl;
}

int main(int argc, char *argv[])
{
  cubepos cp, cp2, cp3, cp4;

  // move movepc
  for (int i = 0; i < NMOVES; i++)
  {
    string m = cp.moves[i];
    cp.move(i);
    cp.movepc(i);
    check(cp, identity_cube, "movepc " + m + " should undo move" + m);
  }

  // move
  for (int i = 0; i < FACES; i++)
  {
    cp.move(i * TWISTS);
    cp.move(i * TWISTS);
    cp.move(i * TWISTS);
    cp.move(i * TWISTS);
    check(cp, identity_cube, "move 4 times should return to identity cube");
  }

  // invert_sequence invert_into
  for (int i = 0; i < 10; i++)
  {
    vector<int> ms = generate_moveseq(10);
    vector<int> ms_inverted = cubepos::invert_sequence(ms);
    cp = identity_cube;
    cp2 = identity_cube;
    for (int k = 0; k < ms.size(); ++k)
    {
      cp.move(ms[k]);
      cp2.move(ms_inverted[k]);
    }
    cp.invert_into(cp3);
    check(cp2, cp3, "invert cube from move seqs equals cube from inverted move seq");
  }

  // mul
  for (int i = 0; i < 10; i++)
  {
    vector<int> ms1 = generate_moveseq(10);
    vector<int> ms2 = generate_moveseq(10);
    cp = identity_cube;
    cp2 = identity_cube;
    cp3 = identity_cube;
    for (int j = 0; j < ms1.size(); ++j)
    {
      cp2.move(ms1[j]);
      cp.move(ms1[j]);
    }
    for (int j = 0; j < ms2.size(); ++j)
    {
      cp3.move(ms2[j]);
      cp.move(ms2[j]);
    }
    cubepos::mul(cp2, cp3, cp4);
    check(cp, cp4, "r from move seq a and b should be the same as mul(a,b,r)");
  }

  // mulpc
  for (int i = 0; i < 10; i++)
  {
    vector<int> ms1 = generate_moveseq(10);
    vector<int> ms2 = generate_moveseq(10);
    cp = identity_cube;
    cp2 = identity_cube;
    cp3 = identity_cube;
    for (int j = 0; j < ms1.size(); ++j)
    {
      cp2.movepc(ms1[j]);
      cp.movepc(ms1[j]);
    }
    for (int j = 0; j < ms2.size(); ++j)
    {
      cp3.movepc(ms2[j]);
      cp.movepc(ms2[j]);
    }
    cubepos::mulpc(cp2, cp3, cp4);
    check(cp, cp4, "cube from movepc seq a and b should be the same as mulpc(a,b,cube)");
  }

  // append_moveseq parse_moveseq
  for (int i = 0; i < 10; i++)
  {
    vector<int> ms1 = generate_moveseq(10);
    char buf[20]; // 10 steps took 20 char
    char *p1 = buf;
    cubepos::append_moveseq(p1, ms1);
    const char *p2 = buf;
    vector<int> ms2 = cubepos::parse_moveseq(p2);
    check(ms1, ms2, "append_moveseq convert move seq to char[], parse_moveseq should convert char[] back to move seq");
  }

  // randomize Singmaster_string parse_Singmaster
  for (int i = 0; i < 10; i++)
  {
    cp = identity_cube;
    cp.randomize();
    char buf[100];
    strcpy(buf, cp.Singmaster_string());
    cp2.parse_Singmaster(buf);
    check(cp, cp2, "parse_Singmaster should generate the same cube from result from Singmaster_string");
  }

  // move_map remap_into
  for (int i = 0; i < 10; i++)
  {
    int mirror_index = (int)(M * drand48());
    vector<int> ms = generate_moveseq(10);
    cp = identity_cube;
    cp2 = identity_cube;
    for (int j = 0; j < ms.size(); ++j)
    {
      cp.move(ms[j]);
      cp2.move(cubepos::move_map[mirror_index][ms[j]]);
    }
    cp.remap_into(mirror_index, cp3);
    check(cp2, cp3, "remap a cube after move seq equals apply remap on each move");
  }

  // canon_into48
  for (int i = 0; i < 10; i++)
  {
    cp = identity_cube;
    cp2 = identity_cube;
    vector<int> ms1 = generate_moveseq(10);
    for (int j = 0; j < ms1.size(); ++j)
    {
      cp.move(ms1[j]);
    }
    cp.canon_into48(cp2);

    for (int j = 0; j < 5; ++j)
    {
      int mirror_index = (int)(M * drand48());
      cp3 = identity_cube;
      cp4 = identity_cube;
      cp.remap_into(mirror_index, cp3);
      cp3.canon_into48(cp4);
      check(cp2, cp4, "remap a cube to any mirror should not change canon_into48 result");
    }
  }

  // canon_seq, canon_seq_mask
  // use mask to prune invalid move
  // for (int i = 0; i < 10; i++)
  // {
  //   vector<int> ms = generate_moveseq(20);
  //   int canon_state = CANONSEQSTART;
  //   for (int j = 0; j < ms.size(); ++j)
  //   {
  //     int mv = ms[j];
  //     int mask = cp.cs_mask(canon_state);
  //     long long unsigned c = mask;
  //     bitset<18> bitset1{(long long unsigned)mask};
  //     bitset<18> bitset2{(long long unsigned)(mask >> mv)};
  //     int valid = (mask >> mv) & 1;
  //     if (!valid)
  //     {
  //       break;
  //     }
  //     canon_state = cp.next_cs(canon_state, mv);
  //   }
  // }

  cout << "TEST SUCCESS" << endl;
}