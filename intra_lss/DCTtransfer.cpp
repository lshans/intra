

void DCTCore4x4(int16_t block[4][4], int16_t tblock[4][4])
{
	int16_t i, ii;
	int16_t tmp[16];
	int16_t *pTmp = tmp, *pblock;
	int16_t p0,p1,p2,p3;
	int16_t t0,t1,t2,t3;

	// Horizontal
	for (i=0; i < 0 + 4; i++)
	{
		pblock = &block[i][0];
		p0 = *(pblock++);
		p1 = *(pblock++);
		p2 = *(pblock++);
		p3 = *(pblock  );

		t0 = p0 + p3;
		t1 = p1 + p2;
		t2 = p1 - p2;
		t3 = p0 - p3;

		*(pTmp++) =  t0 + t1;
		*(pTmp++) = (t3 << 1) + t2;
		*(pTmp++) =  t0 - t1;    
		*(pTmp++) =  t3 - (t2 << 1);
	}

	// Vertical 
	for (i=0; i < 4; i++)
	{
		pTmp = tmp + i;
		p0 = *pTmp;
		p1 = *(pTmp += 4);
		p2 = *(pTmp += 4);
		p3 = *(pTmp += 4);

		t0 = p0 + p3;
		t1 = p1 + p2;
		t2 = p1 - p2;
		t3 = p0 - p3;

		ii = 0 + i;
		tblock[0    ][ii] = t0 +  t1;
		tblock[0 + 1][ii] = t2 + (t3 << 1);
		tblock[0 + 2][ii] = t0 -  t1;
		tblock[0 + 3][ii] = t3 - (t2 << 1);
	}
}

void IDCTCore4x4(int16_t tblock[4][4], int16_t block[4][4])
{
	int16_t i, ii;
	int16_t tmp[16];
	int16_t *pTmp = tmp, *pblock;
	int16_t p0,p1,p2,p3;
	int16_t t0,t1,t2,t3;

	// Horizontal
	for (i = 0; i < 0 + 4; i++)
	{
		pblock = &tblock[i][0];
		t0 = *(pblock++);
		t1 = *(pblock++);
		t2 = *(pblock++);
		t3 = *(pblock  );

		p0 =  t0 + t2;
		p1 =  t0 - t2;
		p2 = (t1 >> 1) - t3;
		p3 =  t1 + (t3 >> 1);

		*(pTmp++) = p0 + p3;
		*(pTmp++) = p1 + p2;
		*(pTmp++) = p1 - p2;
		*(pTmp++) = p0 - p3;
	}

	//  Vertical 
	for (i = 0; i < 4; i++)
	{
		pTmp = tmp + i;
		t0 = *pTmp;
		t1 = *(pTmp += 4);
		t2 = *(pTmp += 4);
		t3 = *(pTmp += 4);

		p0 = t0 + t2;
		p1 = t0 - t2;
		p2 =(t1 >> 1) - t3;
		p3 = t1 + (t3 >> 1);

		ii = i + 0;
		block[0    ][ii] = p0 + p3;
		block[0 + 1][ii] = p1 + p2;
		block[0 + 2][ii] = p1 - p2;
		block[0 + 3][ii] = p0 - p3;
	}
}

