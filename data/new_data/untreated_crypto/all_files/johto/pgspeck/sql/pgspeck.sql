CREATE EXTENSION pgspeck;

-- test vector from the original whitepaper
select to_hex(pgspeck_encrypt32((x'6574694c')::int8, (x'1918111009080100')::int8));

select u.val, k.key, pgspeck_decrypt32(pgspeck_encrypt32(u.val, k.key), k.key)
from
unnest(array[
 -- edge values
 0, 1, 4294967294, 4294967295,

 -- a randomly generated set of values
 340411100,3648991854,3883173854,2911300352,170217392,3281780748,2743510608,2227816056,4223735584,1560485918,
 3927159578,3284820474,1333891060,598245358,2408391506,2297414002,2228884466,1475149318,990315928,1065154076,
 4010714368,60823918,2570889252,816192832,3693574190,4127177550,2057598664,941954836,3111942606,3694457644,
 3392854510,4199690084,1837942636,3844069376,3854292914,70869752,3595719730,2272830742,1110902212,1059850778,
 3495696238,743094494,49703956,534620002,1341339854,2458095464,2832034006,3570224320,3933244782,382234993
]) u(val)
cross join (values
	(int8 '0'),
	('1'),
	('7371941163172890000'),
	('1589783502606012418'),
	('9223372036854775807'),
	('-9223372036854775808')
) k(key);


-- test vector from the original whitepaper

select to_hex(pgspeck_encrypt48((x'6d2073696874')::int8, (x'1a1918121110')::int8, (x'0a0908020100')::int8));

select
	u.val,
	k.key1,
	k.key2,
	pgspeck_encrypt48(u.val, k.key1, k.key2) as encrypted,
	pgspeck_decrypt48(pgspeck_encrypt48(u.val, k.key1, k.key2), k.key1, k.key2) as decrypted
from unnest(array[
	-- edge values
	0, 1, 281474976710654, 281474976710655,

	-- a randomly generated set of values
	340411100,3648991854,3883173854,2911300352,170217392,3281780748,2743510608,2227816056,4223735584,1560485918,
	3927159578,3284820474,1333891060,598245358,2408391506,2297414002,2228884466,1475149318,990315928,1065154076,
	4010714368,60823918,2570889252,816192832,3693574190,4127177550,2057598664,941954836,3111942606,3694457644,
	3392854510,4199690084,1837942636,3844069376,3854292914,70869752,3595719730,2272830742,1110902212,1059850778,
	3495696238,743094494,49703956,534620002,1341339854,2458095464,2832034006,3570224320,3933244782,382234993
]) u(val)
cross join (values
	(int8 '0', int8 '0'),
	('0', '1'),
	('1', '0'),
	('737194116','3172890000'),
	('158978350','2606012418'),
	('922337203','6854775807'),
	('281474976710655','281474976710655')
) k(key1, key2);

-- error conditions
select pgspeck_encrypt48(0, -1, 0);
select pgspeck_encrypt48(0, 0, -1);
select pgspeck_encrypt48(0, 9223372036854775807, 0);
select pgspeck_encrypt48(0, 0, 9223372036854775807);

select pgspeck_decrypt48(0, -1, 0);
select pgspeck_decrypt48(0, 0, -1);
select pgspeck_decrypt48(0, 9223372036854775807, 0);
select pgspeck_decrypt48(0, 0, 9223372036854775807);
