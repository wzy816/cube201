/**
 * http://cube20.org/src/cubepos.pdf
 * http://www.geometer.org/rubik/
 */

#ifndef CUBEPOS_H
#define CUBEPOS_H

#include <cstring>
#include <cstdlib>
#include <stddef.h>
#include <vector>
#include <algorithm>
#include <sys/time.h>
using namespace std;

/**
 * number of moves
 * 6 faces * 3 twists 
 */
const int NMOVES = 18;

/**
 * number of corner orientation
 * clockwise, half turn, counterclockwise
 */
const int TWISTS = 3;

/**
 * number of cube faces
 */
const int FACES = 6;

/** 
 * number of automorphism
 * ? should be 8 distinct corner * 3 global rotation = 24, why reflection
 */
const int M = 48;

/**
 * number of states
 * 
 * for corner that is 8 slots * 3 orientations
 * for edges that is 12 slots * 2 orientations
 */
const int CUBIES = 24;

/**
 * solved cube
 */
extern const class cubepos identity_cube;

typedef vector<int> moveseq;


const int ALLMOVEMASK = (1 << NMOVES) - 1;
const int ALLMOVEMASK_EXT = (1 << NMOVES) - 1;

const int CANONSEQSTATES = FACES + 1;
const int CANONSEQSTART = 0;

/**
 * util
 */
void error(const char *s);
inline double myrand() { return drand48(); }
inline int random_move() { return (int) (NMOVES*myrand()); }
inline int random_move_ext() { return (int) (NMOVES*myrand()); }
double walltime();
double duration();


class cubepos {
 public:

  /**
   * compare function
   */
  inline bool operator<(const cubepos &cp) const {
    return memcmp(this, &cp, sizeof(cp)) < 0;
  }
  inline bool operator==(const cubepos &cp) const {
    return memcmp(this, &cp, sizeof(cp))==0;
  }
  inline bool operator!=(const cubepos &cp) const {
    return memcmp(this, &cp, sizeof(cp))!=0;
  }

  /**
   * corner representation
   * 
   * indexing by layer
   *  0 - 1    - - -    4 - 5
   *  - - -    - - -    - - - 
   *  2 - 3    - - -    6 - 7
   *   top     middle   bottom
   * 
   * bit structure
   * [-,-,-, twist, twist, slot, slot, slot]
   */
  unsigned char c[8];

  /**
   * corner orientation array
   */ 
  static unsigned char corner_ori_inc[CUBIES], corner_ori_dec[CUBIES],
      corner_ori_neg_strip[CUBIES], mod24[2*CUBIES];

  /**
   * corner permutation and orientation functions 
   */
  /* get 1-3 low bit */
  static inline int corner_perm(int cubieval) { return cubieval & 7; }
  /* get 4-8 high bit, 0=no twist, 1=clockwise, 2=counterclockwise */
  static inline int corner_ori(int cubieval) { return cubieval >> 3; }
  /* combine perm and ori to char */
  static inline int corner_val(int perm, int ori) { return ori*8 + perm; }
  /* */
  static inline int corner_ori_add(int cv1, int cv2) { return mod24[cv1 + (cv2 & 0x18)]; }
  /* */
  static inline int corner_ori_sub(int cv1, int cv2) { return cv1 + corner_ori_neg_strip[cv2]; }

  /**
   * edge representation
   * 
   * indexing by layer
   *  - 0 -    4 - 5 .  - 8 - 
   *  1 - 2    - - -    9 - 10
   *  - 3 -    6 - 7 .  - 11 -
   *   top     middle   bottom
   * 
   * bit structure
   * [-,-,-, slot, slot, slot, slot, flip]
   */
  unsigned char e[12];

  /**
   * edge permutation and orientation functions 
   */
  /* get 2-8 high bit */
  static inline int edge_perm(int cubieval) { return cubieval >> 1; }
  /* get 1 low bit, 1=flip, 0=not flip */
  static inline int edge_ori(int cubieval) { return cubieval & 1; }
  /* XOR on 1 low bit */
  static inline int edge_flip(int cubieval) { return cubieval ^ 1; }
  /* combine perm and ori to char */
  static inline int edge_val(int perm, int ori) { return perm*2 + ori; }
  /* */
  static inline int edge_ori_add(int cv1, int cv2) { return cv1 ^ edge_ori(cv2); }


  /**
   * init
   */
  static void init();

  /**
   * constructor
   */
  inline cubepos(const cubepos &cp = identity_cube) { *this = cp; }
  cubepos(int, int, int);

  void move(int mov);

  static int invert_move(int mv) { return inv_move[mv]; }

  static moveseq invert_sequence(const moveseq &sequence);

  void invert_into(cubepos &dst) const;

  void movepc(int mov);

  static void mul(const cubepos &a, const cubepos &b, cubepos &r);

  inline static void mulpc(const cubepos &a, const cubepos &b, cubepos &r) {
    mul(b, a, r);
  }


  /**
   * parsing and printing
   */
  static void skip_whitespace(const char *&p);
  static int parse_face(const char *&p);
  static int parse_face(char f);
  static void append_face(char *&p, int f) { *p++ = faces[f]; }
  static int parse_move(const char *&p);
  static void append_move(char *&p, int mv);
  static moveseq parse_moveseq(const char *&p);
  static void append_moveseq(char *&p, const moveseq &seq);
  static char *moveseq_string(const moveseq &seq);


  /**
   * read and write of singmaster notation
   */
  const char *parse_Singmaster(const char *p);
  char *Singmaster_string() const;

  void remap_into(int m, cubepos &dst) const;
  void canon_into48(cubepos &dst) const;
  void canon_into48_aux(cubepos &dst) const;
  void canon_into96(cubepos &dst) const;

  void randomize();

  static char const *moves[NMOVES];
  static char faces[FACES];

  static unsigned char edge_trans[NMOVES][CUBIES],
      corner_trans[NMOVES][CUBIES];

  static unsigned char inv_move[NMOVES];

  static unsigned char face_map[M][FACES], move_map[M][NMOVES];
  static unsigned char invm[M], mm[M][M];
  static unsigned char rot_edge[M][CUBIES], rot_corner[M][CUBIES];

      /**
   * canon
   */
      static unsigned char canon_seq[CANONSEQSTATES][NMOVES];
  static int canon_seq_mask[CANONSEQSTATES];
  static int canon_seq_mask_ext[CANONSEQSTATES];

  /**
   * canon seq util
   */
  static inline int next_cs(int cs, int mv) { return canon_seq[cs][mv]; }
  static inline int cs_mask(int cs) { return canon_seq_mask[cs]; }
  static inline int cs_mask_ext(int cs) { return canon_seq_mask_ext[cs]; }
};

static cubepos cubepos_initialization_hack(1, 2, 3);


#endif