/*
  ppmpl.h  -  preprocessor meta programming library

  Copyright (C)         Lumiera.org
    2008,               Christian Thaeter <ct@pipapo.org>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation; either version 2 of
  the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef PPMPL_H
#define PPMPL_H

/**
 * @file
 * Preprocessor metaprogramming library. We define some useful preprocessor
 * tricks here.
 */


/**
 * Iterate over a list of macros.
 * @param p used to disambiguate up to three passes, use _, _P1_ or _P2_
 * @param ... list of macros to be expanded. The user has to supply a definition
 * in the form of PPMPL_FOREACH##p##macroname which shall expand to the desired text.
 *
 * This user defined macro shall be undefed after use.
 *
 * @example
 * #define PPMPL_FOREACH_P1_FOO(arg) arg,
 *
 * {PPMPL_FOREACH(P1, FOO(1), FOO(2), FOO(3)), -1}
 *
 * #undef PPMPL_FOREACH_P1_FOO
 *
 * Would expand to the sequence:
 * {1, 2, 3, -1}
 *
 * One can not recursively nest preprocessor macros. To allow this we define PPMPL_FOREACH_L1
 * to PPMPL_FOREACH_L2 with the same semantics as PPMPL_FOREACH, This allowes to nest the
 * FOREACH loop up to three nesting levels.
 */
#define PPMPL_FOREACH(p, ...) PPMPL_FOREACH0(p, __VA_ARGS__, PPMPL_FOREACH_NIL))

/* internal */
#define PPMPL_FOREACH0(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH1(p, __VA_ARGS__)
#define PPMPL_FOREACH1(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH2(p, __VA_ARGS__)
#define PPMPL_FOREACH2(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH3(p, __VA_ARGS__)
#define PPMPL_FOREACH3(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH4(p, __VA_ARGS__)
#define PPMPL_FOREACH4(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH5(p, __VA_ARGS__)
#define PPMPL_FOREACH5(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH6(p, __VA_ARGS__)
#define PPMPL_FOREACH6(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH7(p, __VA_ARGS__)
#define PPMPL_FOREACH7(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH8(p, __VA_ARGS__)
#define PPMPL_FOREACH8(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH9(p, __VA_ARGS__)
#define PPMPL_FOREACH9(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH10(p, __VA_ARGS__)
#define PPMPL_FOREACH10(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH11(p, __VA_ARGS__)
#define PPMPL_FOREACH11(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH12(p, __VA_ARGS__)
#define PPMPL_FOREACH12(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH13(p, __VA_ARGS__)
#define PPMPL_FOREACH13(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH14(p, __VA_ARGS__)
#define PPMPL_FOREACH14(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH15(p, __VA_ARGS__)
#define PPMPL_FOREACH15(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH16(p, __VA_ARGS__)
#define PPMPL_FOREACH16(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH17(p, __VA_ARGS__)
#define PPMPL_FOREACH17(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH18(p, __VA_ARGS__)
#define PPMPL_FOREACH18(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH19(p, __VA_ARGS__)
#define PPMPL_FOREACH19(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH20(p, __VA_ARGS__)
#define PPMPL_FOREACH20(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH21(p, __VA_ARGS__)
#define PPMPL_FOREACH21(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH22(p, __VA_ARGS__)
#define PPMPL_FOREACH22(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH23(p, __VA_ARGS__)
#define PPMPL_FOREACH23(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH24(p, __VA_ARGS__)
#define PPMPL_FOREACH24(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH25(p, __VA_ARGS__)
#define PPMPL_FOREACH25(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH26(p, __VA_ARGS__)
#define PPMPL_FOREACH26(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH27(p, __VA_ARGS__)
#define PPMPL_FOREACH27(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH28(p, __VA_ARGS__)
#define PPMPL_FOREACH28(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH29(p, __VA_ARGS__)
#define PPMPL_FOREACH29(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH30(p, __VA_ARGS__)
#define PPMPL_FOREACH30(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH31(p, __VA_ARGS__)
#define PPMPL_FOREACH31(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH32(p, __VA_ARGS__)
#define PPMPL_FOREACH32(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH33(p, __VA_ARGS__)
#define PPMPL_FOREACH33(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH34(p, __VA_ARGS__)
#define PPMPL_FOREACH34(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH35(p, __VA_ARGS__)
#define PPMPL_FOREACH35(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH36(p, __VA_ARGS__)
#define PPMPL_FOREACH36(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH37(p, __VA_ARGS__)
#define PPMPL_FOREACH37(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH38(p, __VA_ARGS__)
#define PPMPL_FOREACH38(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH39(p, __VA_ARGS__)
#define PPMPL_FOREACH39(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH40(p, __VA_ARGS__)
#define PPMPL_FOREACH40(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH41(p, __VA_ARGS__)
#define PPMPL_FOREACH41(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH42(p, __VA_ARGS__)
#define PPMPL_FOREACH42(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH43(p, __VA_ARGS__)
#define PPMPL_FOREACH43(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH44(p, __VA_ARGS__)
#define PPMPL_FOREACH44(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH45(p, __VA_ARGS__)
#define PPMPL_FOREACH45(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH46(p, __VA_ARGS__)
#define PPMPL_FOREACH46(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH47(p, __VA_ARGS__)
#define PPMPL_FOREACH47(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH48(p, __VA_ARGS__)
#define PPMPL_FOREACH48(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH49(p, __VA_ARGS__)
#define PPMPL_FOREACH49(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH50(p, __VA_ARGS__)
#define PPMPL_FOREACH50(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH51(p, __VA_ARGS__)
#define PPMPL_FOREACH51(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH52(p, __VA_ARGS__)
#define PPMPL_FOREACH52(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH53(p, __VA_ARGS__)
#define PPMPL_FOREACH53(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH54(p, __VA_ARGS__)
#define PPMPL_FOREACH54(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH55(p, __VA_ARGS__)
#define PPMPL_FOREACH55(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH56(p, __VA_ARGS__)
#define PPMPL_FOREACH56(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH57(p, __VA_ARGS__)
#define PPMPL_FOREACH57(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH58(p, __VA_ARGS__)
#define PPMPL_FOREACH58(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH59(p, __VA_ARGS__)
#define PPMPL_FOREACH59(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH60(p, __VA_ARGS__)
#define PPMPL_FOREACH60(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH61(p, __VA_ARGS__)
#define PPMPL_FOREACH61(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH62(p, __VA_ARGS__)
#define PPMPL_FOREACH62(p, h, ...) PPMPL_FOREACH##p##h PPMPL_FOREACH63(p, __VA_ARGS__)
#define PPMPL_FOREACH63(p, h, ...) PPMPL_FOREACH##p##h
#define PPMPL_FOREACH_
#define PPMPL_FOREACH_P1_
#define PPMPL_FOREACH_P2_
#define PPMPL_FOREACH_PPMPL_FOREACH_NIL PPMPL_FOREACH_FINAL(
#define PPMPL_FOREACH_P1_PPMPL_FOREACH_NIL PPMPL_FOREACH_FINAL(
#define PPMPL_FOREACH_P2_PPMPL_FOREACH_NIL PPMPL_FOREACH_FINAL(
#define PPMPL_FOREACH_FINAL(...)




#define PPMPL_FOREACH_L1(p, ...) PPMPL_FOREACH_L1_0(p, __VA_ARGS__, PPMPL_FOREACH_L1_NIL))

/* internal */
#define PPMPL_FOREACH_L1_0(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_1(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_1(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_2(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_2(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_3(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_3(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_4(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_4(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_5(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_5(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_6(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_6(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_7(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_7(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_8(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_8(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_9(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_9(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_10(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_10(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_11(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_11(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_12(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_12(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_13(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_13(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_14(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_14(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_15(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_15(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_16(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_16(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_17(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_17(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_18(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_18(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_19(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_19(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_20(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_20(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_21(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_21(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_22(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_22(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_23(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_23(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_24(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_24(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_25(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_25(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_26(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_26(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_27(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_27(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_28(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_28(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_29(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_29(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_30(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_30(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_31(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_31(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_32(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_32(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_33(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_33(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_34(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_34(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_35(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_35(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_36(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_36(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_37(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_37(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_38(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_38(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_39(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_39(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_40(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_40(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_41(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_41(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_42(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_42(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_43(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_43(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_44(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_44(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_45(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_45(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_46(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_46(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_47(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_47(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_48(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_48(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_49(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_49(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_50(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_50(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_51(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_51(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_52(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_52(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_53(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_53(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_54(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_54(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_55(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_55(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_56(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_56(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_57(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_57(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_58(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_58(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_59(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_59(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_60(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_60(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_61(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_61(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_62(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_62(p, h, ...) PPMPL_FOREACH_L1##p##h PPMPL_FOREACH_L1_63(p, __VA_ARGS__)
#define PPMPL_FOREACH_L1_63(p, h, ...) PPMPL_FOREACH_L1##p##h
#define PPMPL_FOREACH_L1_
#define PPMPL_FOREACH_L1_P1_
#define PPMPL_FOREACH_L1_P2_
#define PPMPL_FOREACH_L1_PPMPL_FOREACH_L1_NIL PPMPL_FOREACH_L1_FINAL(
#define PPMPL_FOREACH_L1_P1_PPMPL_FOREACH_L1_NIL PPMPL_FOREACH_L1_FINAL(
#define PPMPL_FOREACH_L1_P2_PPMPL_FOREACH_L1_NIL PPMPL_FOREACH_L1_FINAL(
#define PPMPL_FOREACH_L1_FINAL(...)




#define PPMPL_FOREACH_L2(p, ...) PPMPL_FOREACH_L2_0(p, __VA_ARGS__, PPMPL_FOREACH_L2_NIL))

/* internal */
#define PPMPL_FOREACH_L2_0(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_1(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_1(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_2(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_2(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_3(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_3(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_4(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_4(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_5(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_5(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_6(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_6(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_7(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_7(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_8(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_8(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_9(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_9(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_10(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_10(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_11(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_11(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_12(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_12(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_13(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_13(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_14(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_14(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_15(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_15(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_16(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_16(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_17(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_17(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_18(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_18(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_19(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_19(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_20(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_20(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_21(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_21(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_22(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_22(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_23(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_23(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_24(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_24(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_25(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_25(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_26(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_26(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_27(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_27(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_28(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_28(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_29(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_29(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_30(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_30(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_31(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_31(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_32(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_32(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_33(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_33(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_34(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_34(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_35(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_35(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_36(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_36(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_37(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_37(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_38(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_38(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_39(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_39(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_40(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_40(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_41(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_41(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_42(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_42(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_43(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_43(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_44(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_44(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_45(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_45(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_46(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_46(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_47(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_47(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_48(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_48(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_49(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_49(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_50(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_50(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_51(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_51(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_52(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_52(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_53(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_53(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_54(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_54(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_55(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_55(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_56(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_56(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_57(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_57(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_58(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_58(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_59(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_59(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_60(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_60(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_61(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_61(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_62(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_62(p, h, ...) PPMPL_FOREACH_L2##p##h PPMPL_FOREACH_L2_63(p, __VA_ARGS__)
#define PPMPL_FOREACH_L2_63(p, h, ...) PPMPL_FOREACH_L2##p##h
#define PPMPL_FOREACH_L2_
#define PPMPL_FOREACH_L2_P1_
#define PPMPL_FOREACH_L2_P2_
#define PPMPL_FOREACH_L2_PPMPL_FOREACH_L2_NIL PPMPL_FOREACH_L2_FINAL(
#define PPMPL_FOREACH_L2_P1_PPMPL_FOREACH_L2_NIL PPMPL_FOREACH_L2_FINAL(
#define PPMPL_FOREACH_L2_P2_PPMPL_FOREACH_L2_NIL PPMPL_FOREACH_L2_FINAL(
#define PPMPL_FOREACH_L2_FINAL(...)

/**
 * Canonical preprocessor concat implementation which evaluates its arguments
 */
#define PPMPL_CAT(a,b) PPMPL_CAT_(a,b)
#define PPMPL_CAT_(a,b) a##b


#define PPMPL_STRINGIFY(a) PPMPL_STRINGIFY_(a)
#define PPMPL_STRINGIFY_(a) #a

#endif
/*
// Local Variables:
// mode: C
// c-file-style: "gnu"
// indent-tabs-mode: nil
// End:
*/
