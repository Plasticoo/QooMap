#pragma once
#include "D2Structs.h"

CellFile *LoadBMPCellFile(BYTE * Buffer, INT Width, INT Height);
CellFile *LoadBMPCellFile(LPSTR FileName);
void * memcpy2(LPVOID Destination, CONST VOID * Source, size_t Count);
CellFile *InitCellFile(CellFile * File);