#ifndef __MAP_SCANNER_H__
#define __MAP_SCANNER_H__

#include "map_select.h"

// 从 MPQ 文件扫描地图
int ScanMapsFromMPQ(const char* mpq_path, map_info_t* maps, int max_maps);

// 从本地目录扫描地图
int ScanMapsFromDirectory(const char* dir_path, map_info_t* maps, int max_maps);

#endif // __MAP_SCANNER_H__
