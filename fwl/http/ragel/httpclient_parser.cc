
#line 1 "../fwl/http/ragel/httpclient_parser.rl"
/**
 *
 * Copyright (c) 2010, Zed A. Shaw and Mongrel2 Project Contributors.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 * 
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 * 
 *     * Neither the name of the Mongrel2 Project, Zed A. Shaw, nor the names
 *       of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "httpclient_parser.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
//#include "dbg.h"

#define LEN(AT, FPC) (FPC - buffer - parser->AT)
#define MARK(M,FPC) (parser->M = (FPC) - buffer)
#define PTR_TO(F) (buffer + parser->F)
#define check(A,M,...) if(!(A)) { goto error; }	//delete dgb.h

/** machine **/

#line 155 "../fwl/http/ragel/httpclient_parser.rl"


/** Data **/

#line 58 "../fwl/http/ragel/httpclient_parser.cc"
static const char _httpclient_parser_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 5, 1, 6, 1, 7, 1, 
	8, 1, 9, 1, 10, 1, 11, 1, 
	12, 2, 2, 3, 2, 3, 1, 2, 
	3, 6, 2, 4, 6, 2, 5, 1, 
	2, 5, 12, 2, 6, 11, 2, 6, 
	12, 2, 10, 12, 3, 2, 3, 6, 
	4, 2, 3, 6, 12
};

static const short _httpclient_parser_key_offsets[] = {
	0, 0, 7, 16, 17, 32, 51, 66, 
	84, 85, 86, 87, 88, 90, 93, 95, 
	98, 100, 103, 104, 106, 127, 143, 148, 
	150, 151, 169, 187, 207, 225, 243, 261, 
	279, 297, 315, 331, 338, 362, 367, 369, 
	371, 373, 375, 377, 398, 416, 434, 452, 
	470, 488, 506, 524, 540, 558, 576, 594, 
	612, 630, 648, 666, 684, 700, 707, 731, 
	736, 738, 740, 742, 744, 746, 748, 750, 
	751, 757, 777, 795, 813, 831, 849, 867, 
	885, 891, 895, 899, 903, 907, 911, 915, 
	917, 918, 924, 944, 962, 980, 998, 1016, 
	1022, 1026, 1030, 1034, 1038, 1040, 1058, 1076, 
	1094, 1110, 1128, 1146, 1164, 1182, 1200, 1218, 
	1234, 1241, 1265, 1270, 1274, 1280, 1298, 1304, 
	1308, 1308, 1313, 1318
};

static const char _httpclient_parser_trans_keys[] = {
	72, 48, 57, 65, 70, 97, 102, 10, 
	13, 59, 48, 57, 65, 70, 97, 102, 
	10, 33, 124, 126, 35, 39, 42, 43, 
	45, 46, 48, 57, 65, 90, 94, 122, 
	10, 13, 33, 59, 61, 124, 126, 35, 
	39, 42, 43, 45, 46, 48, 57, 65, 
	90, 94, 122, 33, 124, 126, 35, 39, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	94, 122, 10, 13, 33, 59, 124, 126, 
	35, 39, 42, 43, 45, 46, 48, 57, 
	65, 90, 94, 122, 84, 84, 80, 47, 
	48, 57, 46, 48, 57, 48, 57, 32, 
	48, 57, 48, 57, 32, 48, 57, 10, 
	10, 13, 10, 13, 33, 67, 84, 99, 
	116, 124, 126, 35, 39, 42, 43, 45, 
	46, 48, 57, 65, 90, 94, 122, 33, 
	58, 124, 126, 35, 39, 42, 43, 45, 
	46, 48, 57, 65, 90, 94, 122, 10, 
	13, 32, 9, 12, 10, 13, 10, 33, 
	58, 79, 111, 124, 126, 35, 39, 42, 
	43, 45, 46, 48, 57, 65, 90, 94, 
	122, 33, 58, 78, 110, 124, 126, 35, 
	39, 42, 43, 45, 46, 48, 57, 65, 
	90, 94, 122, 33, 58, 78, 84, 110, 
	116, 124, 126, 35, 39, 42, 43, 45, 
	46, 48, 57, 65, 90, 94, 122, 33, 
	58, 69, 101, 124, 126, 35, 39, 42, 
	43, 45, 46, 48, 57, 65, 90, 94, 
	122, 33, 58, 67, 99, 124, 126, 35, 
	39, 42, 43, 45, 46, 48, 57, 65, 
	90, 94, 122, 33, 58, 84, 116, 124, 
	126, 35, 39, 42, 43, 45, 46, 48, 
	57, 65, 90, 94, 122, 33, 58, 73, 
	105, 124, 126, 35, 39, 42, 43, 45, 
	46, 48, 57, 65, 90, 94, 122, 33, 
	58, 79, 111, 124, 126, 35, 39, 42, 
	43, 45, 46, 48, 57, 65, 90, 94, 
	122, 33, 58, 78, 110, 124, 126, 35, 
	39, 42, 43, 45, 46, 48, 57, 65, 
	90, 94, 122, 33, 58, 124, 126, 35, 
	39, 42, 43, 45, 46, 48, 57, 65, 
	90, 94, 122, 10, 13, 32, 67, 99, 
	9, 12, 10, 13, 32, 33, 67, 84, 
	99, 116, 124, 126, 9, 12, 35, 39, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	94, 122, 32, 67, 99, 9, 13, 76, 
	108, 79, 111, 83, 115, 69, 101, 10, 
	13, 10, 13, 33, 67, 84, 99, 116, 
	124, 126, 35, 39, 42, 43, 45, 46, 
	48, 57, 65, 90, 94, 122, 33, 58, 
	82, 114, 124, 126, 35, 39, 42, 43, 
	45, 46, 48, 57, 65, 90, 94, 122, 
	33, 58, 65, 97, 124, 126, 35, 39, 
	42, 43, 45, 46, 48, 57, 66, 90, 
	94, 122, 33, 58, 78, 110, 124, 126, 
	35, 39, 42, 43, 45, 46, 48, 57, 
	65, 90, 94, 122, 33, 58, 83, 115, 
	124, 126, 35, 39, 42, 43, 45, 46, 
	48, 57, 65, 90, 94, 122, 33, 58, 
	70, 102, 124, 126, 35, 39, 42, 43, 
	45, 46, 48, 57, 65, 90, 94, 122, 
	33, 58, 69, 101, 124, 126, 35, 39, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	94, 122, 33, 58, 82, 114, 124, 126, 
	35, 39, 42, 43, 45, 46, 48, 57, 
	65, 90, 94, 122, 33, 45, 46, 58, 
	124, 126, 35, 39, 42, 43, 48, 57, 
	65, 90, 94, 122, 33, 58, 69, 101, 
	124, 126, 35, 39, 42, 43, 45, 46, 
	48, 57, 65, 90, 94, 122, 33, 58, 
	78, 110, 124, 126, 35, 39, 42, 43, 
	45, 46, 48, 57, 65, 90, 94, 122, 
	33, 58, 67, 99, 124, 126, 35, 39, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	94, 122, 33, 58, 79, 111, 124, 126, 
	35, 39, 42, 43, 45, 46, 48, 57, 
	65, 90, 94, 122, 33, 58, 68, 100, 
	124, 126, 35, 39, 42, 43, 45, 46, 
	48, 57, 65, 90, 94, 122, 33, 58, 
	73, 105, 124, 126, 35, 39, 42, 43, 
	45, 46, 48, 57, 65, 90, 94, 122, 
	33, 58, 78, 110, 124, 126, 35, 39, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	94, 122, 33, 58, 71, 103, 124, 126, 
	35, 39, 42, 43, 45, 46, 48, 57, 
	65, 90, 94, 122, 33, 58, 124, 126, 
	35, 39, 42, 43, 45, 46, 48, 57, 
	65, 90, 94, 122, 10, 13, 32, 67, 
	99, 9, 12, 10, 13, 32, 33, 67, 
	84, 99, 116, 124, 126, 9, 12, 35, 
	39, 42, 43, 45, 46, 48, 57, 65, 
	90, 94, 122, 32, 67, 99, 9, 13, 
	72, 104, 85, 117, 78, 110, 75, 107, 
	69, 101, 68, 100, 10, 13, 10, 10, 
	32, 67, 99, 9, 13, 33, 58, 72, 
	79, 104, 111, 124, 126, 35, 39, 42, 
	43, 45, 46, 48, 57, 65, 90, 94, 
	122, 33, 58, 85, 117, 124, 126, 35, 
	39, 42, 43, 45, 46, 48, 57, 65, 
	90, 94, 122, 33, 58, 78, 110, 124, 
	126, 35, 39, 42, 43, 45, 46, 48, 
	57, 65, 90, 94, 122, 33, 58, 75, 
	107, 124, 126, 35, 39, 42, 43, 45, 
	46, 48, 57, 65, 90, 94, 122, 33, 
	58, 69, 101, 124, 126, 35, 39, 42, 
	43, 45, 46, 48, 57, 65, 90, 94, 
	122, 33, 58, 68, 100, 124, 126, 35, 
	39, 42, 43, 45, 46, 48, 57, 65, 
	90, 94, 122, 10, 13, 33, 58, 124, 
	126, 35, 39, 42, 43, 45, 46, 48, 
	57, 65, 90, 94, 122, 10, 32, 67, 
	99, 9, 13, 10, 13, 72, 104, 10, 
	13, 85, 117, 10, 13, 78, 110, 10, 
	13, 75, 107, 10, 13, 69, 101, 10, 
	13, 68, 100, 10, 13, 10, 10, 32, 
	67, 99, 9, 13, 33, 58, 76, 79, 
	108, 111, 124, 126, 35, 39, 42, 43, 
	45, 46, 48, 57, 65, 90, 94, 122, 
	33, 58, 79, 111, 124, 126, 35, 39, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	94, 122, 33, 58, 83, 115, 124, 126, 
	35, 39, 42, 43, 45, 46, 48, 57, 
	65, 90, 94, 122, 33, 58, 69, 101, 
	124, 126, 35, 39, 42, 43, 45, 46, 
	48, 57, 65, 90, 94, 122, 10, 13, 
	33, 58, 124, 126, 35, 39, 42, 43, 
	45, 46, 48, 57, 65, 90, 94, 122, 
	10, 32, 67, 99, 9, 13, 10, 13, 
	76, 108, 10, 13, 79, 111, 10, 13, 
	83, 115, 10, 13, 69, 101, 10, 13, 
	33, 58, 69, 101, 124, 126, 35, 39, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	94, 122, 33, 58, 78, 110, 124, 126, 
	35, 39, 42, 43, 45, 46, 48, 57, 
	65, 90, 94, 122, 33, 58, 84, 116, 
	124, 126, 35, 39, 42, 43, 45, 46, 
	48, 57, 65, 90, 94, 122, 33, 45, 
	46, 58, 124, 126, 35, 39, 42, 43, 
	48, 57, 65, 90, 94, 122, 33, 58, 
	76, 108, 124, 126, 35, 39, 42, 43, 
	45, 46, 48, 57, 65, 90, 94, 122, 
	33, 58, 69, 101, 124, 126, 35, 39, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	94, 122, 33, 58, 78, 110, 124, 126, 
	35, 39, 42, 43, 45, 46, 48, 57, 
	65, 90, 94, 122, 33, 58, 71, 103, 
	124, 126, 35, 39, 42, 43, 45, 46, 
	48, 57, 65, 90, 94, 122, 33, 58, 
	84, 116, 124, 126, 35, 39, 42, 43, 
	45, 46, 48, 57, 65, 90, 94, 122, 
	33, 58, 72, 104, 124, 126, 35, 39, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	94, 122, 33, 58, 124, 126, 35, 39, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	94, 122, 10, 13, 32, 9, 12, 48, 
	57, 10, 13, 32, 33, 67, 84, 99, 
	116, 124, 126, 9, 12, 35, 39, 42, 
	43, 45, 46, 48, 57, 65, 90, 94, 
	122, 32, 9, 13, 48, 57, 10, 13, 
	48, 57, 10, 32, 9, 13, 48, 57, 
	10, 13, 33, 58, 124, 126, 35, 39, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	94, 122, 10, 32, 9, 13, 48, 57, 
	10, 13, 48, 57, 32, 67, 99, 9, 
	13, 32, 67, 99, 9, 13, 32, 9, 
	13, 48, 57, 0
};

static const char _httpclient_parser_single_lengths[] = {
	0, 1, 3, 1, 3, 7, 3, 6, 
	1, 1, 1, 1, 0, 1, 0, 1, 
	0, 1, 1, 2, 9, 4, 3, 2, 
	1, 6, 6, 8, 6, 6, 6, 6, 
	6, 6, 4, 5, 10, 3, 2, 2, 
	2, 2, 2, 9, 6, 6, 6, 6, 
	6, 6, 6, 6, 6, 6, 6, 6, 
	6, 6, 6, 6, 4, 5, 10, 3, 
	2, 2, 2, 2, 2, 2, 2, 1, 
	4, 8, 6, 6, 6, 6, 6, 6, 
	4, 4, 4, 4, 4, 4, 4, 2, 
	1, 4, 8, 6, 6, 6, 6, 4, 
	4, 4, 4, 4, 2, 6, 6, 6, 
	6, 6, 6, 6, 6, 6, 6, 4, 
	3, 10, 1, 2, 2, 6, 2, 2, 
	0, 3, 3, 1
};

static const char _httpclient_parser_range_lengths[] = {
	0, 3, 3, 0, 6, 6, 6, 6, 
	0, 0, 0, 0, 1, 1, 1, 1, 
	1, 1, 0, 0, 6, 6, 1, 0, 
	0, 6, 6, 6, 6, 6, 6, 6, 
	6, 6, 6, 1, 7, 1, 0, 0, 
	0, 0, 0, 6, 6, 6, 6, 6, 
	6, 6, 6, 5, 6, 6, 6, 6, 
	6, 6, 6, 6, 6, 1, 7, 1, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	1, 6, 6, 6, 6, 6, 6, 6, 
	1, 0, 0, 0, 0, 0, 0, 0, 
	0, 1, 6, 6, 6, 6, 6, 1, 
	0, 0, 0, 0, 0, 6, 6, 6, 
	5, 6, 6, 6, 6, 6, 6, 6, 
	2, 7, 2, 1, 2, 6, 2, 1, 
	0, 1, 1, 2
};

static const short _httpclient_parser_index_offsets[] = {
	0, 0, 5, 12, 14, 24, 38, 48, 
	61, 63, 65, 67, 69, 71, 74, 76, 
	79, 81, 84, 86, 89, 105, 116, 121, 
	124, 126, 139, 152, 167, 180, 193, 206, 
	219, 232, 245, 256, 263, 281, 286, 289, 
	292, 295, 298, 301, 317, 330, 343, 356, 
	369, 382, 395, 408, 420, 433, 446, 459, 
	472, 485, 498, 511, 524, 535, 542, 560, 
	565, 568, 571, 574, 577, 580, 583, 586, 
	588, 594, 609, 622, 635, 648, 661, 674, 
	687, 693, 698, 703, 708, 713, 718, 723, 
	726, 728, 734, 749, 762, 775, 788, 801, 
	807, 812, 817, 822, 827, 830, 843, 856, 
	869, 881, 894, 907, 920, 933, 946, 959, 
	970, 976, 994, 998, 1002, 1007, 1020, 1025, 
	1029, 1030, 1035, 1040
};

static const unsigned char _httpclient_parser_indicies[] = {
	2, 0, 0, 0, 1, 3, 4, 6, 
	5, 5, 5, 1, 7, 1, 8, 8, 
	8, 8, 8, 8, 8, 8, 8, 1, 
	9, 10, 11, 12, 13, 11, 11, 11, 
	11, 11, 11, 11, 11, 1, 14, 14, 
	14, 14, 14, 14, 14, 14, 14, 1, 
	15, 16, 17, 18, 17, 17, 17, 17, 
	17, 17, 17, 17, 1, 19, 1, 20, 
	1, 21, 1, 22, 1, 23, 1, 24, 
	23, 1, 25, 1, 26, 25, 1, 27, 
	1, 28, 29, 1, 1, 30, 32, 33, 
	31, 7, 34, 35, 36, 37, 36, 37, 
	35, 35, 35, 35, 35, 35, 35, 35, 
	1, 38, 39, 38, 38, 38, 38, 38, 
	38, 38, 38, 1, 42, 43, 41, 41, 
	40, 45, 46, 44, 47, 1, 38, 39, 
	48, 48, 38, 38, 38, 38, 38, 38, 
	38, 38, 1, 38, 39, 49, 49, 38, 
	38, 38, 38, 38, 38, 38, 38, 1, 
	38, 39, 50, 51, 50, 51, 38, 38, 
	38, 38, 38, 38, 38, 38, 1, 38, 
	39, 52, 52, 38, 38, 38, 38, 38, 
	38, 38, 38, 1, 38, 39, 53, 53, 
	38, 38, 38, 38, 38, 38, 38, 38, 
	1, 38, 39, 54, 54, 38, 38, 38, 
	38, 38, 38, 38, 38, 1, 38, 39, 
	55, 55, 38, 38, 38, 38, 38, 38, 
	38, 38, 1, 38, 39, 56, 56, 38, 
	38, 38, 38, 38, 38, 38, 38, 1, 
	38, 39, 57, 57, 38, 38, 38, 38, 
	38, 38, 38, 38, 1, 38, 58, 38, 
	38, 38, 38, 38, 38, 38, 38, 1, 
	60, 61, 59, 62, 62, 59, 40, 64, 
	65, 63, 35, 66, 37, 66, 37, 35, 
	35, 63, 35, 35, 35, 35, 35, 35, 
	1, 63, 67, 67, 63, 1, 68, 68, 
	1, 69, 69, 1, 70, 70, 1, 71, 
	71, 1, 72, 73, 1, 74, 75, 76, 
	77, 78, 77, 78, 76, 76, 76, 76, 
	76, 76, 76, 76, 1, 38, 39, 79, 
	79, 38, 38, 38, 38, 38, 38, 38, 
	38, 1, 38, 39, 80, 80, 38, 38, 
	38, 38, 38, 38, 38, 38, 1, 38, 
	39, 81, 81, 38, 38, 38, 38, 38, 
	38, 38, 38, 1, 38, 39, 82, 82, 
	38, 38, 38, 38, 38, 38, 38, 38, 
	1, 38, 39, 83, 83, 38, 38, 38, 
	38, 38, 38, 38, 38, 1, 38, 39, 
	84, 84, 38, 38, 38, 38, 38, 38, 
	38, 38, 1, 38, 39, 85, 85, 38, 
	38, 38, 38, 38, 38, 38, 38, 1, 
	38, 86, 38, 39, 38, 38, 38, 38, 
	38, 38, 38, 1, 38, 39, 87, 87, 
	38, 38, 38, 38, 38, 38, 38, 38, 
	1, 38, 39, 88, 88, 38, 38, 38, 
	38, 38, 38, 38, 38, 1, 38, 39, 
	89, 89, 38, 38, 38, 38, 38, 38, 
	38, 38, 1, 38, 39, 90, 90, 38, 
	38, 38, 38, 38, 38, 38, 38, 1, 
	38, 39, 91, 91, 38, 38, 38, 38, 
	38, 38, 38, 38, 1, 38, 39, 92, 
	92, 38, 38, 38, 38, 38, 38, 38, 
	38, 1, 38, 39, 93, 93, 38, 38, 
	38, 38, 38, 38, 38, 38, 1, 38, 
	39, 94, 94, 38, 38, 38, 38, 38, 
	38, 38, 38, 1, 38, 95, 38, 38, 
	38, 38, 38, 38, 38, 38, 1, 97, 
	98, 96, 99, 99, 96, 40, 101, 102, 
	100, 35, 103, 37, 103, 37, 35, 35, 
	100, 35, 35, 35, 35, 35, 35, 1, 
	100, 104, 104, 100, 1, 105, 105, 1, 
	106, 106, 1, 107, 107, 1, 108, 108, 
	1, 109, 109, 1, 110, 110, 1, 111, 
	112, 1, 113, 1, 101, 100, 104, 104, 
	100, 1, 38, 39, 114, 48, 114, 48, 
	38, 38, 38, 38, 38, 38, 38, 38, 
	1, 38, 39, 115, 115, 38, 38, 38, 
	38, 38, 38, 38, 38, 1, 38, 39, 
	116, 116, 38, 38, 38, 38, 38, 38, 
	38, 38, 1, 38, 39, 117, 117, 38, 
	38, 38, 38, 38, 38, 38, 38, 1, 
	38, 39, 118, 118, 38, 38, 38, 38, 
	38, 38, 38, 38, 1, 38, 39, 119, 
	119, 38, 38, 38, 38, 38, 38, 38, 
	38, 1, 111, 112, 38, 39, 38, 38, 
	38, 38, 38, 38, 38, 38, 1, 120, 
	100, 104, 104, 100, 1, 45, 46, 121, 
	121, 44, 45, 46, 122, 122, 44, 45, 
	46, 123, 123, 44, 45, 46, 124, 124, 
	44, 45, 46, 125, 125, 44, 45, 46, 
	126, 126, 44, 111, 112, 44, 72, 1, 
	64, 63, 67, 67, 63, 1, 38, 39, 
	127, 48, 127, 48, 38, 38, 38, 38, 
	38, 38, 38, 38, 1, 38, 39, 128, 
	128, 38, 38, 38, 38, 38, 38, 38, 
	38, 1, 38, 39, 129, 129, 38, 38, 
	38, 38, 38, 38, 38, 38, 1, 38, 
	39, 130, 130, 38, 38, 38, 38, 38, 
	38, 38, 38, 1, 72, 73, 38, 39, 
	38, 38, 38, 38, 38, 38, 38, 38, 
	1, 131, 63, 67, 67, 63, 1, 45, 
	46, 132, 132, 44, 45, 46, 133, 133, 
	44, 45, 46, 134, 134, 44, 45, 46, 
	135, 135, 44, 136, 137, 44, 38, 39, 
	138, 138, 38, 38, 38, 38, 38, 38, 
	38, 38, 1, 38, 39, 139, 139, 38, 
	38, 38, 38, 38, 38, 38, 38, 1, 
	38, 39, 140, 140, 38, 38, 38, 38, 
	38, 38, 38, 38, 1, 38, 141, 38, 
	39, 38, 38, 38, 38, 38, 38, 38, 
	1, 38, 39, 142, 142, 38, 38, 38, 
	38, 38, 38, 38, 38, 1, 38, 39, 
	143, 143, 38, 38, 38, 38, 38, 38, 
	38, 38, 1, 38, 39, 144, 144, 38, 
	38, 38, 38, 38, 38, 38, 38, 1, 
	38, 39, 145, 145, 38, 38, 38, 38, 
	38, 38, 38, 38, 1, 38, 39, 146, 
	146, 38, 38, 38, 38, 38, 38, 38, 
	38, 1, 38, 39, 147, 147, 38, 38, 
	38, 38, 38, 38, 38, 38, 1, 38, 
	148, 38, 38, 38, 38, 38, 38, 38, 
	38, 1, 150, 151, 149, 149, 152, 40, 
	154, 155, 153, 35, 36, 37, 36, 37, 
	35, 35, 153, 35, 35, 35, 156, 35, 
	35, 1, 153, 153, 157, 1, 158, 159, 
	160, 1, 154, 153, 153, 157, 1, 158, 
	159, 38, 39, 38, 38, 38, 38, 38, 
	161, 38, 38, 1, 162, 153, 153, 157, 
	1, 158, 159, 163, 44, 1, 100, 104, 
	104, 100, 1, 63, 67, 67, 63, 1, 
	153, 153, 157, 1, 0
};

static const char _httpclient_parser_trans_targs[] = {
	2, 0, 8, 120, 3, 2, 4, 120, 
	5, 120, 3, 5, 4, 6, 7, 120, 
	3, 7, 4, 9, 10, 11, 12, 13, 
	14, 15, 16, 17, 18, 17, 19, 19, 
	20, 24, 3, 21, 25, 44, 21, 22, 
	23, 22, 20, 24, 23, 20, 24, 20, 
	26, 27, 28, 101, 29, 30, 31, 32, 
	33, 34, 35, 35, 36, 95, 96, 37, 
	122, 89, 90, 38, 39, 40, 41, 42, 
	43, 88, 120, 3, 21, 25, 44, 45, 
	46, 47, 48, 49, 50, 51, 52, 53, 
	54, 55, 56, 57, 58, 59, 60, 61, 
	61, 62, 80, 81, 63, 121, 72, 73, 
	64, 65, 66, 67, 68, 69, 70, 20, 
	71, 20, 74, 75, 76, 77, 78, 79, 
	62, 82, 83, 84, 85, 86, 87, 91, 
	92, 93, 94, 36, 97, 98, 99, 100, 
	43, 88, 102, 103, 104, 105, 106, 107, 
	108, 109, 110, 111, 112, 112, 113, 118, 
	119, 114, 123, 116, 117, 115, 20, 24, 
	115, 117, 113, 119
};

static const char _httpclient_parser_trans_actions[] = {
	1, 0, 1, 49, 19, 0, 19, 23, 
	3, 56, 52, 0, 52, 25, 7, 46, 
	11, 0, 11, 0, 0, 0, 0, 0, 
	0, 0, 17, 1, 15, 0, 1, 0, 
	13, 13, 0, 3, 3, 3, 0, 5, 
	7, 7, 31, 31, 0, 11, 11, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 5, 7, 31, 31, 7, 0, 
	23, 0, 3, 0, 0, 0, 0, 0, 
	0, 0, 40, 9, 37, 37, 37, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 5, 
	7, 31, 31, 7, 0, 23, 0, 28, 
	7, 0, 0, 0, 0, 0, 0, 43, 
	11, 21, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	11, 11, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 5, 7, 31, 31, 
	7, 0, 23, 0, 28, 7, 34, 34, 
	0, 0, 0, 0
};

static const int httpclient_parser_start = 1;
static const int httpclient_parser_first_final = 120;
static const int httpclient_parser_error = 0;

static const int httpclient_parser_en_main = 1;


#line 159 "../fwl/http/ragel/httpclient_parser.rl"

int httpclient_parser_init(httpclient_parser *parser)  {
    int cs = 0;

    
#line 510 "../fwl/http/ragel/httpclient_parser.cc"
	{
	cs = httpclient_parser_start;
	}

#line 164 "../fwl/http/ragel/httpclient_parser.rl"

    parser->cs = cs;
    parser->body_start = 0;
    parser->content_len = -1;
    parser->chunked = 0;
    parser->chunks_done = 0;
    parser->mark = 0;
    parser->nread = 0;
    parser->field_len = 0;
    parser->field_start = 0;    
    parser->close = 0;

    return(1);
}


/** exec **/
int httpclient_parser_execute(httpclient_parser *parser, const char *buffer, size_t len, size_t off)  
{
    const char *p, *pe;
    int cs = parser->cs;

    assert(off <= len && "offset past end of buffer");

    p = buffer+off;
    pe = buffer+len;

    assert(*pe == '\0' && "pointer does not end on NUL");
    assert(pe - p == (int)len - (int)off && "pointers aren't same distance");


    
#line 548 "../fwl/http/ragel/httpclient_parser.cc"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _httpclient_parser_trans_keys + _httpclient_parser_key_offsets[cs];
	_trans = _httpclient_parser_index_offsets[cs];

	_klen = _httpclient_parser_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _httpclient_parser_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _httpclient_parser_indicies[_trans];
	cs = _httpclient_parser_trans_targs[_trans];

	if ( _httpclient_parser_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _httpclient_parser_actions + _httpclient_parser_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 52 "../fwl/http/ragel/httpclient_parser.rl"
	{MARK(mark, p); }
	break;
	case 1:
#line 54 "../fwl/http/ragel/httpclient_parser.rl"
	{ MARK(field_start, p); }
	break;
	case 2:
#line 56 "../fwl/http/ragel/httpclient_parser.rl"
	{ 
        parser->field_len = LEN(field_start, p);
    }
	break;
	case 3:
#line 60 "../fwl/http/ragel/httpclient_parser.rl"
	{ MARK(mark, p); }
	break;
	case 4:
#line 62 "../fwl/http/ragel/httpclient_parser.rl"
	{ 
        parser->content_len = strtol(PTR_TO(mark), NULL, 10);
    }
	break;
	case 5:
#line 66 "../fwl/http/ragel/httpclient_parser.rl"
	{
        parser->close = 1;
    }
	break;
	case 6:
#line 70 "../fwl/http/ragel/httpclient_parser.rl"
	{ 
        if(parser->http_field != NULL) {
            parser->http_field(parser->data, PTR_TO(field_start), parser->field_len, PTR_TO(mark), LEN(mark, p));
        }
    }
	break;
	case 7:
#line 76 "../fwl/http/ragel/httpclient_parser.rl"
	{ 
        if(parser->reason_phrase != NULL)
            parser->reason_phrase(parser->data, PTR_TO(mark), LEN(mark, p));
    }
	break;
	case 8:
#line 81 "../fwl/http/ragel/httpclient_parser.rl"
	{ 
        parser->status = strtol(PTR_TO(mark), NULL, 10);

        if(parser->status_code != NULL)
            parser->status_code(parser->data, PTR_TO(mark), LEN(mark, p));
    }
	break;
	case 9:
#line 88 "../fwl/http/ragel/httpclient_parser.rl"
	{	
        if(parser->http_version != NULL)
            parser->http_version(parser->data, PTR_TO(mark), LEN(mark, p));
    }
	break;
	case 10:
#line 93 "../fwl/http/ragel/httpclient_parser.rl"
	{
        parser->chunked = 1;
		parser->content_len = strtol(PTR_TO(mark), NULL, 16);
        parser->chunks_done = parser->content_len <= 0;

        if(parser->chunks_done && parser->last_chunk) {
            parser->last_chunk(parser->data, PTR_TO(mark), LEN(mark, p));
        } else if(parser->chunk_size != NULL) {
            parser->chunk_size(parser->data, PTR_TO(mark), LEN(mark, p));
        } // else skip it
    }
	break;
	case 11:
#line 105 "../fwl/http/ragel/httpclient_parser.rl"
	{
        parser->chunked = 1;
    }
	break;
	case 12:
#line 109 "../fwl/http/ragel/httpclient_parser.rl"
	{ 
        parser->body_start = p - buffer + 1; 
        if(parser->header_done != NULL)
            parser->header_done(parser->data, p + 1, pe - p - 1);
        {p++; goto _out; }
    }
	break;
#line 712 "../fwl/http/ragel/httpclient_parser.cc"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

#line 196 "../fwl/http/ragel/httpclient_parser.rl"

    parser->cs = cs;
    parser->nread += p - (buffer + off);

    assert(p <= pe && "buffer overflow after parsing execute");
    assert(parser->nread <= len && "nread longer than length");
    assert(parser->body_start <= len && "body starts after buffer end");
    check(parser->mark < len, "mark is after buffer end");
    check(parser->field_len <= len, "field has length longer than whole buffer");
    check(parser->field_start < len, "field starts after buffer end");

    //if(parser->body_start) {
    //    /* final \r\n combo encountered so stop right here */
    //    parser->nread++;
    //}

    return(parser->nread);

error:
    return -1;
}

int httpclient_parser_finish(httpclient_parser *parser)
{
    int cs = parser->cs;

    parser->cs = cs;

    if (httpclient_parser_has_error(parser) ) {
        return -1;
    } else if (httpclient_parser_is_finished(parser) ) {
        return 1;
    } else {
        return 0;
    }
}

int httpclient_parser_has_error(httpclient_parser *parser) {
    return parser->cs == httpclient_parser_error;
}

int httpclient_parser_is_finished(httpclient_parser *parser) {
    return parser->cs == httpclient_parser_first_final;
}
