
#include <iostream>

#include "cubepos.h"
using namespace std;

//
int main() {
  cubepos cp, cp1;
  cp = identity_cube;

  char buf[] = "U2R2D2B1U2B3F1D3B3R3D1U2B2F2R3D3B1U3F3R2";
  const char *p = buf;
  cp.show();

  moveseq mv = cubepos::parse_moveseq(p);
  for (int i = 0; i < mv.size(); ++i) {
    cout << cp.moves[mv[i]] << endl;
    cp.move(mv[i]);
  }
  cp.show();

  for (int i = 1; i < 24; ++i) {
    cp.set_color(i);
    cout << "color scheme " << i << ": " << cp.color_schemes[i] << endl;
    cp.show();
  }
}