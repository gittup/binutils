#as:
#objdump: -dr
#name: i860 bitwise

.*: +file format .*

Disassembly of section \.text:

00000000 <\.text>:
   0:	00 00 22 c0 	and	%r0,%r1,%sp
   4:	00 18 85 c0 	and	%fp,%r4,%r5
   8:	00 30 e8 c0 	and	%r6,%r7,%r8
   c:	00 48 4b c1 	and	%r9,%r10,%r11
  10:	00 60 ae c1 	and	%r12,%r13,%r14
  14:	00 78 11 c2 	and	%r15,%r16,%r17
  18:	00 90 74 c2 	and	%r18,%r19,%r20
  1c:	00 a8 d7 c2 	and	%r21,%r22,%r23
  20:	00 c0 3a c3 	and	%r24,%r25,%r26
  24:	00 d8 9d c3 	and	%r27,%r28,%r29
  28:	00 f0 e0 c3 	and	%r30,%r31,%r0
  2c:	00 00 22 d0 	andnot	%r0,%r1,%sp
  30:	00 18 85 d0 	andnot	%fp,%r4,%r5
  34:	00 30 e8 d0 	andnot	%r6,%r7,%r8
  38:	00 48 4b d1 	andnot	%r9,%r10,%r11
  3c:	00 60 ae d1 	andnot	%r12,%r13,%r14
  40:	00 78 11 d2 	andnot	%r15,%r16,%r17
  44:	00 90 74 d2 	andnot	%r18,%r19,%r20
  48:	00 a8 d7 d2 	andnot	%r21,%r22,%r23
  4c:	00 c0 3a d3 	andnot	%r24,%r25,%r26
  50:	00 d8 9d d3 	andnot	%r27,%r28,%r29
  54:	00 f0 e0 d3 	andnot	%r30,%r31,%r0
  58:	00 00 22 e0 	or	%r0,%r1,%sp
  5c:	00 18 85 e0 	or	%fp,%r4,%r5
  60:	00 30 e8 e0 	or	%r6,%r7,%r8
  64:	00 48 4b e1 	or	%r9,%r10,%r11
  68:	00 60 ae e1 	or	%r12,%r13,%r14
  6c:	00 78 11 e2 	or	%r15,%r16,%r17
  70:	00 90 74 e2 	or	%r18,%r19,%r20
  74:	00 a8 d7 e2 	or	%r21,%r22,%r23
  78:	00 c0 3a e3 	or	%r24,%r25,%r26
  7c:	00 d8 9d e3 	or	%r27,%r28,%r29
  80:	00 f0 e0 e3 	or	%r30,%r31,%r0
  84:	00 00 22 f0 	xor	%r0,%r1,%sp
  88:	00 18 85 f0 	xor	%fp,%r4,%r5
  8c:	00 30 e8 f0 	xor	%r6,%r7,%r8
  90:	00 48 4b f1 	xor	%r9,%r10,%r11
  94:	00 60 ae f1 	xor	%r12,%r13,%r14
  98:	00 78 11 f2 	xor	%r15,%r16,%r17
  9c:	00 90 74 f2 	xor	%r18,%r19,%r20
  a0:	00 a8 d7 f2 	xor	%r21,%r22,%r23
  a4:	00 c0 3a f3 	xor	%r24,%r25,%r26
  a8:	00 d8 9d f3 	xor	%r27,%r28,%r29
  ac:	00 f0 e0 f3 	xor	%r30,%r31,%r0
  b0:	00 00 22 c4 	and	0x0000,%r1,%sp
  b4:	00 20 85 c4 	and	0x2000,%r4,%r5
  b8:	f5 13 e8 c4 	and	0x13f5,%r7,%r8
  bc:	00 80 4b c5 	and	0x8000,%r10,%r11
  c0:	e8 fd ae c5 	and	0xfde8,%r13,%r14
  c4:	ff ff 11 c6 	and	0xffff,%r16,%r17
  c8:	ff ff 74 c6 	and	0xffff,%r19,%r20
  cc:	cd ab d7 c6 	and	0xabcd,%r22,%r23
  d0:	34 12 3a c7 	and	0x1234,%r25,%r26
  d4:	00 00 9d c7 	and	0x0000,%r28,%r29
  d8:	03 00 e0 c7 	and	0x0003,%r31,%r0
  dc:	01 00 22 cc 	andh	0x0001,%r1,%sp
  e0:	01 20 85 cc 	andh	0x2001,%r4,%r5
  e4:	f6 13 e8 cc 	andh	0x13f6,%r7,%r8
  e8:	01 80 4b cd 	andh	0x8001,%r10,%r11
  ec:	e9 fd ae cd 	andh	0xfde9,%r13,%r14
  f0:	ff ff 11 ce 	andh	0xffff,%r16,%r17
  f4:	ff ff 74 ce 	andh	0xffff,%r19,%r20
  f8:	cd ab d7 ce 	andh	0xabcd,%r22,%r23
  fc:	34 12 3a cf 	andh	0x1234,%r25,%r26
 100:	00 00 9d cf 	andh	0x0000,%r28,%r29
 104:	03 00 e0 cf 	andh	0x0003,%r31,%r0
 108:	00 00 22 d4 	andnot	0x0000,%r1,%sp
 10c:	00 20 85 d4 	andnot	0x2000,%r4,%r5
 110:	f5 13 e8 d4 	andnot	0x13f5,%r7,%r8
 114:	00 80 4b d5 	andnot	0x8000,%r10,%r11
 118:	e8 fd ae d5 	andnot	0xfde8,%r13,%r14
 11c:	ff ff 11 d6 	andnot	0xffff,%r16,%r17
 120:	ff ff 74 d6 	andnot	0xffff,%r19,%r20
 124:	cd ab d7 d6 	andnot	0xabcd,%r22,%r23
 128:	34 12 3a d7 	andnot	0x1234,%r25,%r26
 12c:	00 00 9d d7 	andnot	0x0000,%r28,%r29
 130:	03 00 e0 d7 	andnot	0x0003,%r31,%r0
 134:	01 00 22 dc 	andnoth	0x0001,%r1,%sp
 138:	01 20 85 dc 	andnoth	0x2001,%r4,%r5
 13c:	f6 13 e8 dc 	andnoth	0x13f6,%r7,%r8
 140:	01 80 4b dd 	andnoth	0x8001,%r10,%r11
 144:	e9 fd ae dd 	andnoth	0xfde9,%r13,%r14
 148:	ff ff 11 de 	andnoth	0xffff,%r16,%r17
 14c:	ff ff 74 de 	andnoth	0xffff,%r19,%r20
 150:	cd ab d7 de 	andnoth	0xabcd,%r22,%r23
 154:	34 12 3a df 	andnoth	0x1234,%r25,%r26
 158:	00 00 9d df 	andnoth	0x0000,%r28,%r29
 15c:	03 00 e0 df 	andnoth	0x0003,%r31,%r0
 160:	00 00 22 e4 	or	0x0000,%r1,%sp
 164:	01 00 85 e4 	or	0x0001,%r4,%r5
 168:	02 00 e8 e4 	or	0x0002,%r7,%r8
 16c:	03 00 4b e5 	or	0x0003,%r10,%r11
 170:	e8 fd ae e5 	or	0xfde8,%r13,%r14
 174:	ff ff 11 e6 	or	0xffff,%r16,%r17
 178:	ff ff 74 e6 	or	0xffff,%r19,%r20
 17c:	cd ab d7 e6 	or	0xabcd,%r22,%r23
 180:	34 12 3a e7 	or	0x1234,%r25,%r26
 184:	00 00 9d e7 	or	0x0000,%r28,%r29
 188:	03 00 e0 e7 	or	0x0003,%r31,%r0
 18c:	00 00 22 ec 	orh	0x0000,%r1,%sp
 190:	01 00 85 ec 	orh	0x0001,%r4,%r5
 194:	02 00 e8 ec 	orh	0x0002,%r7,%r8
 198:	03 00 4b ed 	orh	0x0003,%r10,%r11
 19c:	e8 fd ae ed 	orh	0xfde8,%r13,%r14
 1a0:	ff ff 11 ee 	orh	0xffff,%r16,%r17
 1a4:	ff ff 74 ee 	orh	0xffff,%r19,%r20
 1a8:	cd ab d7 ee 	orh	0xabcd,%r22,%r23
 1ac:	34 12 3a ef 	orh	0x1234,%r25,%r26
 1b0:	00 00 9d ef 	orh	0x0000,%r28,%r29
 1b4:	03 00 e0 ef 	orh	0x0003,%r31,%r0
 1b8:	00 00 22 f4 	xor	0x0000,%r1,%sp
 1bc:	01 00 85 f4 	xor	0x0001,%r4,%r5
 1c0:	02 00 e8 f4 	xor	0x0002,%r7,%r8
 1c4:	03 00 4b f5 	xor	0x0003,%r10,%r11
 1c8:	e8 fd ae f5 	xor	0xfde8,%r13,%r14
 1cc:	ff ff 11 f6 	xor	0xffff,%r16,%r17
 1d0:	ff ff 74 f6 	xor	0xffff,%r19,%r20
 1d4:	cd ab d7 f6 	xor	0xabcd,%r22,%r23
 1d8:	34 12 3a f7 	xor	0x1234,%r25,%r26
 1dc:	00 00 9d f7 	xor	0x0000,%r28,%r29
 1e0:	03 00 e0 f7 	xor	0x0003,%r31,%r0
 1e4:	00 00 22 fc 	xorh	0x0000,%r1,%sp
 1e8:	01 00 85 fc 	xorh	0x0001,%r4,%r5
 1ec:	02 00 e8 fc 	xorh	0x0002,%r7,%r8
 1f0:	03 00 4b fd 	xorh	0x0003,%r10,%r11
 1f4:	e8 fd ae fd 	xorh	0xfde8,%r13,%r14
 1f8:	ff ff 11 fe 	xorh	0xffff,%r16,%r17
 1fc:	ff ff 74 fe 	xorh	0xffff,%r19,%r20
 200:	cd ab d7 fe 	xorh	0xabcd,%r22,%r23
 204:	34 12 3a ff 	xorh	0x1234,%r25,%r26
 208:	00 00 9d ff 	xorh	0x0000,%r28,%r29
 20c:	03 00 e0 ff 	xorh	0x0003,%r31,%r0
