#ifndef __r_mdx_compat_h__
#define __r_mdx_compat_h__

#include "r_mdx.h"
#include <StormLib.h>

// 使用 KNOWN_AS 宏定义 LPxxx 和 LPCxxx 类型
// 这些类型是为了兼容 r_mdx_load.c 文件中使用的类型

// 模型类型
typedef mdxModel_t MODEL;
typedef mdxModel_t *LPMODEL;
typedef const mdxModel_t *LPCMODEL;

// 几何集类型
typedef mdxGeoset_t MODELGEOSET;
typedef mdxGeoset_t *LPMODELGEOSET;
typedef const mdxGeoset_t *LPCMODELGEOSET;

// 材质类型
typedef mdxMaterial_t MODELMATERIAL;
typedef mdxMaterial_t *LPMODELMATERIAL;
typedef const mdxMaterial_t *LPCMODELMATERIAL;

// 材质层类型
typedef mdxMaterialLayer_t MODELMATERIALLAYER;
typedef mdxMaterialLayer_t *LPMODELMATERIALLAYER;
typedef const mdxMaterialLayer_t *LPCMODELMATERIALLAYER;

// 骨骼类型
typedef mdxBone_t MODELBONE;
typedef mdxBone_t *LPMODELBONE;
typedef const mdxBone_t *LPCMODELBONE;

// 辅助对象类型
typedef mdxHelper_t MODELHELPER;
typedef mdxHelper_t *LPMODELHELPER;
typedef const mdxHelper_t *LPCMODELHELPER;

// 几何集动画类型
typedef mdxGeosetAnim_t MODELGEOSETANIM;
typedef mdxGeosetAnim_t *LPMODELGEOSETANIM;
typedef const mdxGeosetAnim_t *LPCMODELGEOSETANIM;

// 事件类型
typedef mdxEvent_t MODELEVENT;
typedef mdxEvent_t *LPMODELEVENT;
typedef const mdxEvent_t *LPCMODELEVENT;

// 节点类型
typedef mdxNode_t MODELNODE;
typedef mdxNode_t *LPMODELNODE;
typedef const mdxNode_t *LPCMODELNODE;

// 关键帧轨道类型
typedef mdxKeyTrack_t MODELKEYTRACK;
typedef mdxKeyTrack_t *LPMODELKEYTRACK;
typedef const mdxKeyTrack_t *LPCMODELKEYTRACK;

// 序列类型
typedef mdxSequence_t MODELSEQUENCE;
typedef mdxSequence_t *LPMODELSEQUENCE;
typedef const mdxSequence_t *LPCMODELSEQUENCE;

// 全局序列类型
typedef mdxGlobalSequence_t MODELGLOBALSEQUENCE;
typedef mdxGlobalSequence_t *LPMODELGLOBALSEQUENCE;
typedef const mdxGlobalSequence_t *LPCMODELGLOBALSEQUENCE;

// 纹理类型
typedef mdxTexture_t MODELTEXTURE;
typedef mdxTexture_t *LPMODELTEXTURE;
typedef const mdxTexture_t *LPCMODELTEXTURE;

// 边界类型
typedef mdxBounds_t MODELBOUNDS;
typedef mdxBounds_t *LPMODELBOUNDS;
typedef const mdxBounds_t *LPCMODELBOUNDS;

// 模型信息类型
typedef mdxInfo_t MODELINFO;
typedef mdxInfo_t *LPMODELINFO;
typedef const mdxInfo_t *LPCMODELINFO;

// 枚举类型兼容性定义
typedef enum {
    kModelKeyTrackDataTypeFloat = 0,
    kModelKeyTrackDataTypeVector3,
    kModelKeyTrackDataTypeQuaternion,
    kModelKeyTrackDataTypeInteger
} MODELKEYTRACKDATATYPE;

// 宏定义兼容性
#define ID_MDLX MAKEFOURCC('M', 'D', 'L', 'X')
#define ID_VERS MAKEFOURCC('V', 'E', 'R', 'S')
#define ID_MODL MAKEFOURCC('M', 'O', 'D', 'L')
#define ID_SEQS MAKEFOURCC('S', 'E', 'Q', 'S')
#define ID_GLBS MAKEFOURCC('G', 'L', 'B', 'S')
#define ID_MTLS MAKEFOURCC('M', 'T', 'L', 'S')
#define ID_TEXS MAKEFOURCC('T', 'E', 'X', 'S')
#define ID_GEOS MAKEFOURCC('G', 'E', 'O', 'S')
#define ID_GEOA MAKEFOURCC('G', 'E', 'O', 'A')
#define ID_BONE MAKEFOURCC('B', 'O', 'N', 'E')
#define ID_HELP MAKEFOURCC('H', 'E', 'L', 'P')
#define ID_PIVT MAKEFOURCC('P', 'I', 'V', 'T')
#define ID_EVTS MAKEFOURCC('E', 'V', 'T', 'S')
#define ID_VRTX MAKEFOURCC('V', 'R', 'T', 'X')
#define ID_NRMS MAKEFOURCC('N', 'R', 'M', 'S')
#define ID_UVBS MAKEFOURCC('U', 'V', 'B', 'S')
#define ID_PTYP MAKEFOURCC('P', 'T', 'Y', 'P')
#define ID_PCNT MAKEFOURCC('P', 'C', 'N', 'T')
#define ID_PVTX MAKEFOURCC('P', 'V', 'T', 'X')
#define ID_GNDX MAKEFOURCC('G', 'N', 'D', 'X')
#define ID_MTGC MAKEFOURCC('M', 'T', 'G', 'C')
#define ID_UVAS MAKEFOURCC('U', 'V', 'A', 'S')
#define ID_MATS MAKEFOURCC('M', 'A', 'T', 'S')
#define ID_LAYS MAKEFOURCC('L', 'A', 'Y', 'S')
#define ID_KGAO MAKEFOURCC('K', 'G', 'A', 'O')
#define ID_KEVT MAKEFOURCC('K', 'E', 'V', 'T')

// 其他常量定义
#define MODELTRACKINFOSIZE (sizeof(DWORD) * 4)

// 类型别名
typedef struct tFileBlock {
    DWORD id;
    DWORD size;
} tFileBlock;

// 文件处理函数
typedef void* HANDLE;
#define SFileReadFile(file, buffer, size, bytesRead, overlapped) memcpy(buffer, file, size)
#define FileReadInt32(file) (*(int*)(file))
#define FileReadBlock(file) (*(tFileBlock*)(file))

#endif // __r_mdx_compat_h__