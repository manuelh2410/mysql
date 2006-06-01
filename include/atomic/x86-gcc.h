/* Copyright (C) 2006 MySQL AB

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

/*
  XXX 64-bit atomic operations can be implemented using
  cmpxchg8b, if necessary
*/

/* fix -ansi errors while maintaining readability */
#define asm __asm__

#define make_atomic_add_body8					\
  asm volatile (LOCK "xadd %0, %1;" : "+r" (v) , "+m" (a->val))
#define make_atomic_swap_body8					\
  asm volatile ("xchg %0, %1;" : "+r" (v) , "+m" (a->val))
#define make_atomic_cas_body8					\
  asm volatile (LOCK "cmpxchg %3, %0; setz %2;"			\
               : "+m" (a->val), "+a" (*cmp), "=q" (ret): "r" (set))

#ifdef MY_ATOMIC_MODE_DUMMY
#define make_atomic_load_body8   ret=a->val
#define make_atomic_store_body8	 a->val=v
#else
/*
  Actually 32-bit reads/writes are always atomic on x86
  But we add LOCK here anyway to force memory barriers
*/
#define make_atomic_load_body8					\
  ret=0;							\
  asm volatile (LOCK "cmpxchg %2, %0"				\
               : "+m" (a->val), "+a" (ret): "r" (ret))
#define make_atomic_store_body8					\
  asm volatile ("xchg %0, %1;" : "+m" (a->val) : "r" (v))
#endif

#define make_atomic_add_body16   make_atomic_add_body8
#define make_atomic_add_body32   make_atomic_add_body8
#define make_atomic_cas_body16   make_atomic_cas_body8
#define make_atomic_cas_body32   make_atomic_cas_body8
#define make_atomic_load_body16  make_atomic_load_body8
#define make_atomic_load_body32  make_atomic_load_body8
#define make_atomic_store_body16 make_atomic_store_body8
#define make_atomic_store_body32 make_atomic_store_body8
#define make_atomic_swap_body16  make_atomic_swap_body8
#define make_atomic_swap_body32  make_atomic_swap_body8

