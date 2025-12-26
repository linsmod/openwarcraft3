#include "map_scanner.h"
#include "../common/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

// 从本地目录扫描地图
int ScanMapsFromDirectory(const char* dir_path, map_info_t* maps, int max_maps) {
    int count = 0;
    DIR *dir;
    struct dirent *entry;
    
    dir = opendir(dir_path);
    if (dir == NULL) {
        printf("Failed to open directory: %s\n", dir_path);
        return 0;
    }
    
    while ((entry = readdir(dir)) != NULL && count < max_maps) {
        // 检查文件扩展名
        const char* ext = strrchr(entry->d_name, '.');
        if (ext && (strcmp(ext, ".w3m") == 0 || strcmp(ext, ".w3x") == 0)) {
            // 保存地图信息
            strncpy(maps[count].filename, entry->d_name, sizeof(maps[count].filename) - 1);
            strncpy(maps[count].name, entry->d_name, sizeof(maps[count].name) - 1);
            // TODO: 解析地图文件获取详细信息
            strcpy(maps[count].author, "Unknown");
            strcpy(maps[count].description, "Custom map");
            strcpy(maps[count].recommended, "?");
            count++;
        }
    }
    
    closedir(dir);
    printf("Found %d maps in %s\n", count, dir_path);
    return count;
}
