/* tc-h8300.c -- Assemble code for the Hitachi h8/300
   Copyright (C) 1991 Free Software Foundation.

This file is part of GAS, the GNU Assembler.

GAS is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GAS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GAS; see the file COPYING.  If not, write to
the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.  */


/* 
  Written By Steve Chamberlain

  steve@cygnus.com
 */

#include <stdio.h>
#include "as.h"
#include "bfd.h"
#include "h8300-opcode.h"
#include <ctype.h>

char  comment_chars[]  = { ';',0 };

/* This table describes all the machine specific pseudo-ops the assembler
   has to support.  The fields are:
 	  pseudo-op name without dot
	  function to call to execute this pseudo-op
	  Integer arg to pass to the function
 */
const pseudo_typeS md_pseudo_table[] = {
	{ 0,		0,		0	}
};

int  md_reloc_size ;
 
const char EXP_CHARS[] = "eE";

/* Chars that mean this number is a floating point constant */
/* As in 0f12.456 */
/* or    0d1.2345e12 */
 char FLT_CHARS[] = "rRsSfFdDxXpP";


const relax_typeS md_relax_table[1];


static struct hash_control *opcode_hash_control;	/* Opcode mnemonics */
static struct hash_control *register_hash_control;	/* Register name hash table */


/*
 This function is called once, at assembler startup time.  This should
 set up all the tables, etc that the MD part of the assembler needs
*/

reloc_howto_type *r16;
reloc_howto_type *r8;
reloc_howto_type *r8ff;
reloc_howto_type *r8pcrel;

void md_begin () 
{
  bfd_arch_info_type *ai;
  const struct h8_opcode *opcode;

  opcode_hash_control = hash_new();
  for (opcode = h8_opcodes; opcode->name; opcode++) {
    hash_insert(opcode_hash_control, opcode->name, (char *)opcode);
  }

  ai = bfd_lookup_arch(bfd_arch_h8300,0);
  
  r16 = ai->reloc_type_lookup(ai, BFD_RELOC_16);
  r8 = ai->reloc_type_lookup(ai, BFD_RELOC_8);
  r8ff = ai->reloc_type_lookup(ai, BFD_RELOC_8_FFnn);
  r8pcrel = ai->reloc_type_lookup(ai, BFD_RELOC_8_PCREL);

  
}


struct h8_exp {
  char *e_beg;
  char *e_end;
  expressionS e_exp;
};
struct h8_op 
{
op_type mode;
  unsigned reg;
  expressionS exp;
};



/*
 parse operands	
 WREG r0,r1,r2,r3,r4,r5,r6,r7,fp,sp
 r0l,r0h,..r7l,r7h
 @WREG
 @WREG+
 @-WREG
 #const

*/

op_type r8_sord[] = {RS8, RD8};
op_type r16_sord[] = {RS16, RD16};
op_type rind_sord[] = {RSIND, RDIND};
op_type abs_sord[2] = {ABS16SRC, ABS16DST};
op_type disp_sord[] = {DISPSRC, DISPDST};
/* try and parse a reg name, returns number of chars consumed */
int DEFUN(parse_reg,(src, mode, reg, dst),
	  char *src AND
	  op_type *mode AND
	  unsigned int *reg AND
	  int dst)
{
  if (src[0]  == 's' && src[1] == 'p') {
    *mode = r16_sord[dst];
    *reg = 7;
    return 2;
  }
  if (src[0] == 'c' && src[1] == 'c' && src[2] == 'r') {
    *mode = CCR;
    *reg = 0;
    return 3;
  }
  if (src[0]  == 'f' && src[1] == 'p') {
    *mode = r16_sord[dst];
    *reg = 6;
    return 2;
  }
  if (src[0] == 'r') {
    if (src[1] >= '0' && src[1] <= '7') {
      if(src[2] == 'l') {
	*mode = r8_sord[dst];
	*reg = (src[1] - '0') + 8;
	return 3;
      }
      if(src[2] == 'h') {
	*mode = r8_sord[dst];
	*reg = (src[1] - '0')  ;
	return 3;
      }
      *mode = r16_sord[dst];
      *reg = (src[1] - '0');
      return 2;
    }
  }
  return 0;
}

char *
DEFUN(parse_exp,(s, op),
      char *s AND
      expressionS *op)
{
  char *save = input_line_pointer;
  char *new;
  segT seg;
  input_line_pointer = s;
  seg = expr(0,op);
  new = input_line_pointer;
  input_line_pointer = save;
  if (SEG_NORMAL(seg)) 
    return new;
  switch (seg) {
  case SEG_ABSOLUTE:
  case SEG_UNKNOWN:
  case SEG_DIFFERENCE:
  case SEG_BIG:
  case SEG_REGISTER:
    return new;
  case SEG_ABSENT:
    as_bad("Missing operand");
    return new;
  default:
    as_bad("Don't understand operand of type %s", segment_name (seg));
    return new;
  }
}


static void 
DEFUN(get_operand,(ptr, op, dst),
      char **ptr AND 
      struct h8_op *op AND 
      unsigned int dst)
{
  char *src = *ptr;
  op_type mode;
  unsigned   int num;
  unsigned  int len;
  op->mode = E;

  while (*src == ' ') src++;
  len = parse_reg(src, &op->mode, &op->reg, dst);
  if (len) {
    *ptr = src + len;
    return ;
  }
      
  if (*src == '@') {
    src++;
    if (*src == '-') {	
      src++;
      len = parse_reg(src, &mode, &num, dst);
      if (len == 0 || mode != r16_sord[dst]) {
	as_bad("@- needs word register");
      }
      op->mode = RDDEC;
      op->reg = num;
      *ptr = src + len;
      return;
    }
    if (*src == '(' && ')') {
      /* Disp */
      src++;
      src =      parse_exp(src, &op->exp);

      if (*src == ')') {
	src++;
	op->mode = abs_sord[dst];
	*ptr = src;
	return;
      }
      if (*src  != ',') {
	as_bad("expected @(exp, reg16)");
      }
      src++;
      len = parse_reg(src, &mode, &op->reg, dst);
      if (len == 0 || mode != r16_sord[dst])
	  {
	    as_bad("expected @(exp, reg16)");
	  }
      op->mode = disp_sord[dst];
      src += len;
      if (*src != ')' && '(') {
	as_bad("expected @(exp, reg16)");

      }
      *ptr = src +1;

      return;
    }
    len = parse_reg(src, &mode, &num, dst);

    if(len) {
      src += len;
      if (*src == '+') {
	src++;
	if (mode != RS16) {
	  as_bad("@Rn+ needs word register");
	}
	op->mode = RSINC;
	op->reg = num;
	*ptr = src;
	return;
      }
      if (mode != r16_sord[dst]) {
	as_bad("@Rn needs word register");
      }
      op->mode =rind_sord[dst];
      op->reg = num;
      *ptr = src;
      return;
    }
    else {
      /* must be a symbol */
      op->mode = abs_sord[dst];
      *ptr = parse_exp(src, &op->exp);
      return;
    }
  }

  
  if (*src == '#') {
    src++;
    op->mode = IMM16;
    *ptr = parse_exp(src, &op->exp);
    return;
  }
  else {
    *ptr = parse_exp(src, &op->exp);
    op->mode = DISP8;
  }
}

/* This is the guts of the machine-dependent assembler.  STR points to a
   machine dependent instruction.  This funciton is supposed to emit
   the frags/bytes it assembles to.
 */
 

void 
DEFUN(md_assemble,(str),
      char *str)
{
  char *op_start;
  char *op_end;
  unsigned int i;
  
  struct h8_opcode * opcode;
  /* Drop leading whitespace */
  while (*str == ' ')
   str++;


  /* find the op code end */
  for (op_start = op_end = str;
       *op_end != 0 && *op_end != ' ';
       op_end ++)
   ;

  if (op_end == op_start) {
      as_bad("can't find opcode ");
    }
  *op_end = 0;
  opcode = (struct h8_opcode *) hash_find(opcode_hash_control,
					  op_start);

  if (opcode == NULL) {
      as_bad("unknown opcode");
      return;
    }


{
  int ok = 1;
  int j,i;
  int dispreg = 0;
  struct       h8_op operand[2];
  char *ptr = op_end+1;
  if (opcode->noperands)
   get_operand(& ptr, &operand[0],0);
  else operand[0].mode = 0;
  if (opcode->noperands==2) {
      if (*ptr == ',') ptr++;
      get_operand(& ptr, &operand[1], 1);
    }
  else operand[1].mode = 0;



{
  struct h8_opcode *this_try ;
  int found = 0;
  for (j = 0; j < opcode->nopcodes && !found; j++) {
      this_try  = opcode + j;
      for (i = 0; i < opcode->noperands; i++) {
	  op_type op = (this_try->args.nib[i]) & ~(B30|B31);
	  switch (op) {
	    case Hex0:
	    case Hex1:
	    case Hex2:
	    case Hex3:
	    case Hex4:
	    case Hex5:
	    case Hex6:
	    case Hex7:
	    case Hex8:
	    case Hex9:
	    case HexA:
	    case HexB:
	    case HexC:
	    case HexD:
	    case HexE:
	    case HexF:
	      break;
	    case DISPSRC:
	    case DISPDST:
	      dispreg = operand[i].reg; 
	    case RD8:
	    case RS8:
	    case RDIND:
	    case RSIND:
	    case RD16:
	    case RS16:
	    case CCR:
	    case RSINC:
	    case RDDEC:
	      if (operand[i].mode != op) goto fail;
	      break;
	    case IMM8:
	      /* We have an expression, called IMM16, but we know we
		 want an 8 bit value here */
	      if (operand[i].mode != IMM16) goto fail;
	      operand[i].mode = IMM8;
	      break;
	    case KBIT:
	    case IMM16:
	    case IMM3:
	      if (operand[i].mode != IMM16) goto fail;
	      break;
	    case ABS16SRC:
	    case ABS8SRC:
	      if (operand[i].mode != ABS16SRC) goto fail;
	      break;
	    case ABS16DST:
	    case ABS8DST:
	      if (operand[i].mode != ABS16DST) goto fail;
		
	      break;
	    }
	}
      found =1;
    fail: ;
    }
  if (found == 0) 
   as_bad("illegal operands for opcode");


  /* Now we know what sort of opcodes etc, lets build the bytes -
     actually we know how big the instruction will be too. So we
     can get
     */
{
	char *output = frag_more(this_try->length);
	char *output_ptr = output;
	op_type *nibble_ptr = this_try->data.nib;
	char part;
	op_type c;
	char high;
	int nib;
 top: ;
	while (*nibble_ptr != E) {
		int nibble;
		for (nibble = 0; nibble <2; nibble++) {
			c = *nibble_ptr & ~(B30|B31);
			switch (c) {
			default:
				abort();
			case KBIT:
				switch  (operand[0].exp.X_add_number) {
				case 1:
					nib = 0;
					break;
				case 2:
					nib = 8;
					break;
				default:
					as_bad("Need #1 or #2 here");
					break;
				}
				/* stop it making a fix */
				operand[0].mode = 0;
				break;
			case 0:
			case 1:
			case 2: case 3: case 4: case 5: case  6:
			case 7: case 8: case 9: case 10: case 11: 
			case  12: case 13: case 14: case 15:
				nib = c;
				break;
			case DISPREG:
				nib = dispreg;
				break;
			case IMM8:
				nib = 0;
				break;

			case DISPDST:
				nib = 0;
				break;
			case IMM3: 
				if (operand[0].exp.X_add_symbol == 0) {
					operand[0].mode = 0; /* stop it making a fix */
					nib =  (operand[0].exp.X_add_number);
				}
				else as_bad("can't have symbol for bit number");
				break;

			case ABS16DST:
				nib = 0;
				break;
			case DISPSRC:		    
			case ABS16SRC:
			case IMM16:
				nib=0;
				break;


			case ABS8DST:
			case ABS8SRC:
			case IGNORE:


				nib = 0;
				break;
			case DISP8:
				nib = 0;
				break;
		      
		    
			case RS8:
			case RS16:
			case RSIND:
			case RSINC:

				nib=  operand[0].reg;
				break;
			case RD8:
			case RD16:
			case RDDEC:
			case RDIND:
				nib  = operand[1].reg;

				break;
			case E: 
				abort();
				break;
			}
			if (*nibble_ptr & B31) nib|=0x8;
			if (nibble == 0) {
				*output_ptr = nib << 4;
			}
			else {
				*output_ptr |= nib;
				output_ptr++;
			}
			nibble_ptr++;
		}

	}

	/* output any fixes */
	for (i = 0; i < 2; i++) 
	    {
		    switch (operand[i].mode) {
		    case 0:
			    break;
		    case DISP8:
			    fix_new(frag_now,
				    output - frag_now->fr_literal + 1, 
				    1,
				    operand[i].exp.X_add_symbol,
				    operand[i].exp.X_subtract_symbol,
				    operand[i].exp.X_add_number -1,
				    1,
				    (int)r8pcrel);
			    break;
		    case IMM8:
			    fix_new(frag_now,
				    output - frag_now->fr_literal + 1, 
				    1,
				    operand[i].exp.X_add_symbol,
				    operand[i].exp.X_subtract_symbol,
				    operand[i].exp.X_add_number,
				    0,
				    0);
			    break;
	  
		    case ABS16SRC:
		    case ABS16DST:
		    case IMM16:
		    case DISPSRC:
		    case DISPDST:
			    fix_new(frag_now,
				    output - frag_now->fr_literal + 2, 
				    2,
				    operand[i].exp.X_add_symbol,
				    operand[i].exp.X_subtract_symbol,
				    operand[i].exp.X_add_number,
				    0,
				    (int)r16);
			    break;
		    case RS8:
		    case RD8:
		    case RS16:
		    case RD16:
		    case RDDEC:	
		    case KBIT:
		    case RSINC:
		    case RDIND:
		    case RSIND:
			    break;
		    default:
			    abort();
		    }
	    }


}
}
}
}

void 
DEFUN(tc_crawl_symbol_chain, (headers),
object_headers *headers)
{
   printf("call to tc_crawl_symbol_chain \n");
}

symbolS *DEFUN(md_undefined_symbol,(name),
	       char *name)
{
return 0;
}

void 
DEFUN(tc_headers_hook,(headers),
      object_headers *headers)
{
  printf("call to tc_headers_hook \n"); 
}
void
DEFUN_VOID(md_end) 
{
}

/* Various routines to kill one day */
/* Equal to MAX_PRECISION in atof-ieee.c */
#define MAX_LITTLENUMS 6

/* Turn a string in input_line_pointer into a floating point constant of type
   type, and store the appropriate bytes in *litP.  The number of LITTLENUMS
   emitted is stored in *sizeP .  An error message is returned, or NULL on OK.
 */
char *
md_atof(type,litP,sizeP)
char type;
char *litP;
int *sizeP;
{
	int	prec;
	LITTLENUM_TYPE words[MAX_LITTLENUMS];
	LITTLENUM_TYPE *wordP;
	char	*t;
	char	*atof_ieee();

	switch(type) {
	case 'f':
	case 'F':
	case 's':
	case 'S':
		prec = 2;
		break;

	case 'd':
	case 'D':
	case 'r':
	case 'R':
		prec = 4;
		break;

	case 'x':
	case 'X':
		prec = 6;
		break;

	case 'p':
	case 'P':
		prec = 6;
		break;

	default:
		*sizeP=0;
		return "Bad call to MD_ATOF()";
	}
	t=atof_ieee(input_line_pointer,type,words);
	if(t)
		input_line_pointer=t;

	*sizeP=prec * sizeof(LITTLENUM_TYPE);
	for(wordP=words;prec--;) {
		md_number_to_chars(litP,(long)(*wordP++),sizeof(LITTLENUM_TYPE));
		litP+=sizeof(LITTLENUM_TYPE);
	}
	return "";	/* Someone should teach Dean about null pointers */
}

int
md_parse_option(argP, cntP, vecP)
    char **argP;
    int *cntP;
    char ***vecP;

  {abort();
  }

int md_short_jump_size;

void tc_aout_fix_to_chars () { printf("call to tc_aout_fix_to_chars \n");
			  abort(); }
void md_create_short_jump(ptr, from_addr, to_addr, frag, to_symbol)
char *ptr;
long from_addr;
long to_addr;
fragS *frag;
symbolS *to_symbol;
{
	as_fatal("failed sanity check.");
      }

void
md_create_long_jump(ptr,from_addr,to_addr,frag,to_symbol)
    char *ptr;
    long from_addr, to_addr;
    fragS *frag;
    symbolS *to_symbol;
{
	as_fatal("failed sanity check.");
}

void
md_convert_frag(headers, fragP)
object_headers *headers;
    fragS * fragP;

 { printf("call to md_convert_frag \n"); abort(); }

long
DEFUN(md_section_align,(seg, size),
	segT seg AND
	long size)
{
	return((size + (1 << section_alignment[(int) seg]) - 1) & (-1 << section_alignment[(int) seg]));

}

void
md_apply_fix(fixP, val)
	fixS *fixP;
	long val;
{
	char *buf = fixP->fx_where + fixP->fx_frag->fr_literal;

	switch(fixP->fx_size) {
	case 1:
		*buf++=val;
		break;
	case 2:
		*buf++=(val>>8);
		*buf++=val;
		break;
	case 4:
		*buf++=(val>>24);
		*buf++=(val>>16);
		*buf++=(val>>8);
		*buf++=val;
		break;
	default:
		abort();
		
	}
}

void DEFUN(md_operand, (expressionP),expressionS *expressionP) 
{ }

int  md_long_jump_size;
int
md_estimate_size_before_relax(fragP, segment_type)
     register fragS *fragP;
     register segT segment_type;
{ printf("call tomd_estimate_size_before_relax \n"); abort(); }
/* Put number into target byte order */

void DEFUN(md_number_to_chars,(ptr, use, nbytes),
	   char *ptr AND
	   long use AND
	   int nbytes)
{
  switch (nbytes) {
  case 4: *ptr++ = (use >> 24) & 0xff;
  case 3: *ptr++ = (use >> 16) & 0xff;
  case 2: *ptr++ = (use >> 8) & 0xff;
  case 1: *ptr++ = (use >> 0) & 0xff;
    break;
  default:
    abort();
  }
}
long md_pcrel_from(fixP) 
fixS *fixP; { abort(); }

void tc_coff_symbol_emit_hook() { }
