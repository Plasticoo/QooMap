#include "AutoMapCell.h"
#include "Offset.h"

DWORD ReadFile(HANDLE hFile, LPVOID buf, DWORD len)
{
	DWORD numdone = 0;
	ReadFile(hFile, buf, len, &numdone, NULL);
	return numdone;
}

CellFile *InitCellFile(CellFile * File)
{
	if (File) D2Cmp::InitCellFile(File, &File, "?", 0, -1, "?");
	return File;
}

HANDLE OpenFileRead(LPSTR filename)
{
	return CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
}

BYTE *AllocReadFile(LPSTR FileName)
{
	HANDLE hFile = OpenFileRead(FileName);
	INT FileSize = GetFileSize(hFile, 0);
	
	if (FileSize <= 0)
		return 0;

	LPBYTE Buffer = new BYTE[FileSize];
	ReadFile(hFile, Buffer, FileSize);

	CloseHandle(hFile);
	return Buffer;
}

CellFile *LoadBMPCellFile(BYTE * Buffer, INT Width, INT Height)
{
	/*BYTE *buf2 = new BYTE[(Width*Height*2)+Height], *dest = buf2;

	for (int i = 0; i < Height; i++) {
		BYTE *src = Buffer+(i*((Width+3)&-4)), *limit = src+Width;
		while (src < limit) {
			BYTE *start = src, *limit2 = min(limit, src+0x7f), trans = !*src;
			do src++; while ((trans == (BYTE)!*src) && (src < limit2));
			if (!trans || (src < limit)) *dest++ = (trans?0x80:0)+(src-start);
			if (!trans) while (start < src) *dest++ = *start++;
		}
		*dest++ = 0x80;
	}

	static DWORD dc6head[] = {6, 1, 0, 0xeeeeeeee, 1, 1, 0x1c,  0, -1, -1, 0, 0, 0, -1, -1};
	dc6head[8] = Width;
	dc6head[9] = Height;
	BYTE *ret = new BYTE[dc6head[13] = sizeof(dc6head)+(dc6head[14] = dest-buf2)+3];
	memset(memcpy2(memcpy2(ret, dc6head, sizeof(dc6head)), buf2, dc6head[14]), 0xee, 3);
	delete buf2;

	return (CellFile *)ret;*/
	BYTE * Buffer2 = new BYTE[(Width * Height * 2) + Height], *Destination = Buffer2;

	for (INT i = 0; i < Height; i++)
	{
		BYTE * Source = Buffer + (i * ((Width + 3) & -4)), *Limit = Source + Width;
		
		while (Source < Limit)
		{
			BYTE * Start = Source, *Limit2 = min(Limit, Source + 0x7F), Transparency = !*Source;
			do Source++; while ((Transparency == (BYTE)!*Source) && (Source < Limit2));

			if (!Transparency || (Source < Limit)) *Destination++ = (BYTE)((Transparency ? 0x80 : 0) + (Source - Start));
			if (!Transparency) while (Start < Source) *Destination++ = *Start++;
		}
		
		*Destination++ = 0x80;
	}

	DWORD DC6[] = {6, 1, 0, 0xEEEEEEEE, 1, 1, 0x1C,  0, -1, -1, 0, 0, 0, -1, -1};
	DC6[8] = Width;
	DC6[9] = Height;

	BYTE * Return = new BYTE[DC6[13] = sizeof(DC6) + (DC6[14] = (DWORD)(Destination - Buffer2)) + 3];
	memset(memcpy2(memcpy2(Return, DC6, sizeof(DC6)), Buffer2, DC6[14]), 0xEE, 3);
	delete Buffer2;

	return (CellFile *)Return;
}

CellFile *LoadBMPCellFile(LPSTR FileName)
{
	/*BYTE *ret = 0;
		
	BYTE *buf1 = AllocReadFile(FileName);
	BITMAPFILEHEADER *bmphead1 = (BITMAPFILEHEADER *)buf1;
	BITMAPINFOHEADER *bmphead2 = (BITMAPINFOHEADER *)(buf1+sizeof(BITMAPFILEHEADER));
	if (buf1 && (bmphead1->bfType == 'MB') && (bmphead2->biBitCount == 8) && (bmphead2->biCompression == BI_RGB)) {
		ret = (BYTE *)LoadBMPCellFile(buf1+bmphead1->bfOffBits, bmphead2->biWidth, bmphead2->biHeight);
	}
	delete buf1;

	return (CellFile *)ret;*/
	LPBYTE Return = 0;
	LPBYTE Buffer = AllocReadFile(FileName);
	
	BITMAPFILEHEADER * BMP1 = (BITMAPFILEHEADER *)Buffer;
	BITMAPINFOHEADER * BMP2 = (BITMAPINFOHEADER *)(Buffer + sizeof(BITMAPFILEHEADER));
	
	if (Buffer && (BMP1->bfType == 'MB') && (BMP2->biBitCount == 8) && (BMP2->biCompression == BI_RGB))
		Return = (LPBYTE)LoadBMPCellFile(Buffer + BMP1->bfOffBits, BMP2->biWidth, BMP2->biHeight);

	delete Buffer;
	return (CellFile *)Return;
}

void * memcpy2(LPVOID Destination, CONST VOID * Source, size_t Count)
{
	return (LPSTR)memcpy(Destination, Source, Count) + Count;
}
