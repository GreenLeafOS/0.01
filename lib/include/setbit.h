/*
 * setbit.h
 *
 *  Created on: 2012-8-6
 *      Author: greenleaf
 */

#ifndef SETBIT_H_
#define SETBIT_H_

#define CHAR_BIT  (32)
#define BITMASK(b) (1 << ((b) % CHAR_BIT))
#define BITSLOT(b) ((b) / CHAR_BIT)
#define BITSET(a, b) ((a)[BITSLOT(b)] |= BITMASK(b))
#define BITCLEAR(a, b) ((a)[BITSLOT(b)] &= ~BITMASK(b))
#define BITTEST(a, b) ((a)[BITSLOT(b)] & BITMASK(b))
#define BITNSLOTS(nb) ((nb + CHAR_BIT - 1) / CHAR_BIT)


#endif /* SETBIT_H_ */
