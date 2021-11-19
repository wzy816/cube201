
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "cubepos.h"
#include "progressbar.hpp"

using namespace std;

int main()
{
  map<string, string> m;
  cubepos cp;
  progressbar bar1(1029362);

  int step_size;
  cout << "step size = ";
  cin >> step_size;
  ifstream input("./data/htm.txt");

  if (input.is_open())
  {
    string line;
    while (getline(input, line))
    {
      bar1.update();
      // cout << line << '\n';
      const char *p = line.c_str();
      vector<int> mv = cubepos::parse_moveseq(p);

      cp = identity_cube;
      // cp.color_unfold(c);
      for (int i = 0; i < step_size; ++i)
      {
        cp.move(mv[i]);
      }
      for (int c = 1; c < 24; ++c)
      {
        const char *p = cp.pair_unfold(c);
        string pair(p);
        string color = to_string(c);
        color.insert(0, 2 - color.length(), '0');
        string step = color + ':' + line.substr(0, step_size * 2);

        map<string, string>::iterator it = m.find(pair);
        if (it == m.end())
        {
          m[pair] = step;
        }
      }
    }
  }
  input.close();
  cout << endl;

  progressbar bar2(m.size());

  ofstream output;
  int file_index = 0;
  int j = 0;
  output.open("./data/pair" + to_string(step_size) + "_" + to_string(file_index) + ".txt", ios::trunc);
  map<string, string>::iterator itStart = m.begin();
  map<string, string>::iterator itEnd = m.end();
  while (itStart != itEnd)
  {
    if (j >= 1000000)
    {
      output.close();
      ++file_index;
      output.open("./data/pair" + to_string(step_size) + "_" + to_string(file_index) + ".txt", ios::trunc);
      j = 0;
    }
    output << itStart->first << ',' << itStart->second << endl;
    itStart++;
    j++;
    bar2.update();
  }
  output.close();
}
