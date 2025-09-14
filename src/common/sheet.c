#include <StormLib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "../common/common.h"
#include "common/shared.h"
#include "parser.h"

#define MAX_INI_LINE 1024
#define MAX_SHEET_COLUMNS 256

typedef struct SheetCell {
  LPSTR text;
  USHORT column;
  USHORT row;
  LPSHEET next;
} sheetCell_t;

// TODO: allocate these as needed, this is only PoC and will only work for 1
// level
static sheetCell_t cells[1024 * 1024] = {0};
static sheetRow_t rows[1024 * 1024] = {0};
static sheetField_t fields[1024 * 1024] = {0};
static char text_buffer[8 * 1024 * 1024] = {0};
static LPSTR current_text = text_buffer;
static LPSHEET current_cell = cells;
static LPSHEET previous_cell = cells;
static sheetRow_t *current_row = rows;
static sheetField_t *current_field = fields;

static DWORD SheetParseTokens(TCHAR *buffer) {
  DWORD numtokens = 1;
  for (TCHAR *it = buffer; *it != '\0'; it++) {
    if (*it == ';') {
      numtokens++;
      *it = '\0';
    }
  }
  return numtokens;
}

static TCHAR *GetToken(TCHAR *buffer, DWORD index) {
  while (index > 0) {
    buffer += strlen(buffer) + 1;
    index--;
  }
  return buffer;
}

// int text_size = 0;

static void FS_FillSheetCell(DWORD x, DWORD y, LPSTR text) {
  current_cell->column = x;
  current_cell->row = y;
  current_cell->next = current_cell + 1;
  current_cell->text = current_text;
  for (char *instr = text; *instr; instr++) {
    if (*instr == '"' || *instr == '\r' || *instr == '\n')
      continue;
    *(current_text++) = *instr;
  }
  *(current_text++) = '\0';
  previous_cell = current_cell;
  current_cell++;
  //    if (y == 2) {
  //    if (strstr(cell->text, "hhou")) {
  //        printf("\t%d %s\n", x, cell->text);
  //    }
}

sheetRow_t *FS_MakeRowsFromSheet(LPSHEET sheet) {
  LPCSTR columns[256] = {0};
  DWORD num_rows = 0;
  sheetRow_t *start = current_row;
  FOR_EACH_LIST(SHEET, cell, sheet) {
    if (cell->row == 1) {
      columns[cell->column] = cell->text;
    }
    num_rows = MAX(num_rows, cell->row);
  }
  sheetRow_t *previous_row = NULL;
  FOR_LOOP(row, num_rows) {
    FOR_EACH_LIST(SHEET, cell, sheet) {
      if (cell->row != row + 1)
        continue;
      if (cell->column == 1) {
        current_row->name = cell->text;
      } else if (columns[cell->column]) {
        current_field->name = columns[cell->column];
        current_field->value = cell->text;
        ADD_TO_LIST(current_field, current_row->fields);
        current_field++;
      }
    }
    if (current_row->name) {
      previous_row = current_row;
      previous_row->next = ++current_row;
    }
  }
  if (previous_row) {
    previous_row->next = NULL;
    return start;
  } else {
    return start;
  }
}

sheetRow_t *FS_ParseSLK(LPCSTR fileName) {
  HANDLE file = FS_OpenFile(fileName);
  if (!file) {
    fprintf(stderr, "ENO_FILE %s\n", fileName);
    return NULL;
  }
  DWORD fileSize = SFileGetFileSize(file, NULL);
  TCHAR czBuffer[MAX_SHEET_LINE] = {0};
  TCHAR ch = 0;
  DWORD X = 1, Y = 1;
  LPSHEET start = current_cell;
  for (DWORD read = 0, cur = 0; read < fileSize; read++) {
    SFileReadFile(file, &ch, 1, NULL, NULL);
    if (ch == '\n') {
      DWORD const numTokens = SheetParseTokens(czBuffer);
      if (czBuffer[0] == 'C' || czBuffer[0] == 'F') {
        for (DWORD i = 1; i < numTokens; i++) {
          TCHAR *token = GetToken(czBuffer, i);
          switch (*token) {
          case 'X':
            X = atoi(token + 1);
            break;
          case 'Y':
            Y = atoi(token + 1);
            break;
          case 'K':
            FS_FillSheetCell(X, Y, token + 1);
            break;
          }
        }
      }

      memset(czBuffer, 0, MAX_SHEET_LINE);
      cur = 0;
    } else {
      czBuffer[cur++] = ch;
    }
  }
  FS_CloseFile(file);
  if (start != current_cell) { // close the table
    previous_cell->next = NULL;
    return FS_MakeRowsFromSheet(start);
  } else {
    return NULL;
  }
}

LPCSTR FS_FindSheetCell(sheetRow_t *sheet, LPCSTR row, LPCSTR column) {
  FOR_EACH_LIST(sheetRow_t const, srow, sheet) {
    if (strcmp(srow->name, row))
      continue;
    FOR_EACH_LIST(sheetField_t const, scolumn, srow->fields) {
      if (strcasecmp(scolumn->name, column))
        continue;
      return scolumn->value;
    }
  }
  return NULL;
}

static LPCSTR skipBOM(LPCSTR p) {
  if (p != NULL && p[0] == '\xEF' && p[1] == '\xBB' && p[2] == '\xBF') {
    return p + 3; // 跳过 UTF-8 BOM
  }
  return p; // 无 BOM，返回原指针
}

static sheetRow_t *FS_ParseINI_Buffer(LPCSTR buffer) {
  LPCSTR p = skipBOM(buffer);
  sheetRow_t *start = current_row;
  sheetRow_t *section = NULL;
  while (true) {
    while (*p && isspace(*p))
      p++;
    if (!*p)
      break;
    if (p[0] == '/' && p[1] == '/') {
      for (; *p != '\n' && *p != '\0'; p++)
        ;
    } else if (*p == '[') {
      p++;
      if (section) {
        section->next = current_row;
      }
      section = current_row++;
      section->name = current_text;
      for (; *p != ']'; current_text++, p++) {
        *current_text = *p;
      }
      *(current_text++) = '\0';
      p++;
    } else {
      char line[MAX_INI_LINE] = {0};
      for (LPSTR w = line; *p != '\n' && *p != '\r' && *p != '\0'; p++, w++) {
        *w = *p;
      }
      LPSTR eq = strstr(line, "=");
      if (eq && section) {
        *eq = '\0';
        for (LPSTR s = eq; s > line; s--) {
          if (*s == ' ')
            *s = '\0';
          else
            break;
        }
        eq++;
        for (; *eq == ' '; eq++)
          ;
        //                printf("%s.%s %s\n", currentSec, line, eq);
        sheetField_t *field = current_field++;
        field->name = current_text;
        strcpy(current_text, line);
        current_text += strlen(line) + 1;
        field->value = current_text;
        strcpy(current_text, eq);
        current_text += strlen(eq) + 1;
        ADD_TO_LIST(field, section->fields);
      }
    }
  }
  return current_row != start ? start : NULL;
}
static void FS_GenHeaderINI(LPCSTR filename, sheetRow_t *config) {
  char structname[256] = {0};
  const char *p = filename;
  int pos = 0;

  // 复制文件名，去掉 .txt 后缀
  while (*p && pos < sizeof(structname) - 1) {
    if (*p == '.') {
      if (strcmp(p, ".txt") == 0) {
        break;
      }
    }
    structname[pos++] = *p;
    p++;
  }

  // 将路径中的反斜杠替换为下划线
  char *slash = structname;
  while ((slash = strchr(slash, '\\')) != NULL) {
    *slash = '_';
  }

  // 生成输出文件名
  char output_filename[512];
  snprintf(output_filename, sizeof(output_filename), "../src/generated/%s.h",
           structname);

  // 直接打开文件写入
  FILE *file = fopen(output_filename, "w");
  if (!file) {
    fprintf(stderr, "Failed to create file: %s\n", output_filename);
    return;
  }

  // === 开始写入内容 ===

  // 结构体定义
  fprintf(file, "#pragma once\n");
  fprintf(file, "#include \"../common/common.h\"\n");
  fprintf(file, "typedef struct %s {\n", structname);

  // 为每一行生成 union
  FOR_EACH_LIST(sheetRow_t, row, config) {
    if (row->name && strlen(row->name) > 0) {
      fprintf(file, "    union {\n");
      FOR_EACH_LIST(sheetField_t, field, row->fields) {
          // number maybe
          char *value = strdup(field->value);
          char *space = strchr(value, ' ');
          char* comment = "";
          if(space){
            comment = space;
            int v = strlen(field->value)+ (value-space);
            while (v>0 && comment[0]!='/') {
                comment+=1;
                v-=1;
            }
            space[0]='\0';
          }
          if(strlen(comment))
            fprintf(file, "        %s\n", comment);
          
          if(isdigit(value[0])){
            if(strchr(value, ',') && strstr(field->name, "Color")){
              fprintf(file, "        LPCOLOR32 %s;\n", field->name);
            }
            else if(strchr(value, ',')){
              fprintf(file, "        LPVECTOR3 %s;\n", field->name);
            }else{
              fprintf(file, "        float %s;\n", field->name);
            }
          }
          else if(value[0]=='\''){
            fprintf(file, "        char %s;\n", field->name);
          }
          else if(strstr(value, "\",\"")){
            fprintf(file, "        LPCSTR %s[];\n", field->name);
          }
          else{
            fprintf(file, "        LPCSTR %s;\n", field->name);
          }
      }
      fprintf(file, "    } %s;\n", row->name);
    }
  }

  fprintf(file, "} %s;\n\n", structname);

  // 函数声明
  fprintf(file, "void Init_%s(%s *config);\n", structname, structname);
  fprintf(file, "void Shutdown_%s(%s *config);\n\n", structname, structname);

  // 初始化函数
  fprintf(file, "void Init_%s(%s *config) {\n", structname, structname);

  FOR_EACH_LIST(sheetRow_t, row, config) {
    if (row->name && strlen(row->name) > 0) {
      // fprintf(file, "    config->%s = MemAlloc(sizeof(%s));\n", row->name,
      // row->name);
      FOR_EACH_LIST(sheetField_t, field, row->fields) {
        char *value = field->value ? strdup(field->value) : "";
        int len = strlen(value);
        int p = 0;
        // str
        // else if(strstr(value, "\",\"")){
        if (value[0] == '"') {
          int n = 0;
          char items[128][512];
          LPCSTR endpos = 0;
          PARSE_LIST(value, item, parse_segment2) {
            strncpy(items[n],item,strlen(item));
            n++;
          }
          if(n>1){
            n=3*n; // "xxxx",
          }
          char* comment = strchr(value+n, '/');
          
          fprintf(file, "    config->%s.%s = ", row->name, field->name);
          if(n>1){
            fputc('{', file);
          }
          FOR_LOOP(i, 128){
            for (const char* s = items[i]; *s; s++) {
              if (*s == '\\') {
                fputc('\\', file);
              }
              fputc(*s, file);
            }
          }
          if(n>1){
            fputc('}', file);
          }
          fprintf(file, ";");
          fprintf(file, " %s\n", comment);
        }
        else {
            // number maybe
          char *space = strchr(value, ' ');
          char* comment = "";
          if(space){
            comment = space;
            int v = strlen(value)+ (value-space);
            while (v>0 && comment[0]!='/') {
                comment+=1;
                v-=1;
            }
            space[0]='\0';
          }
          
          
          if(isdigit(value[0])){
            if(strchr(value, ',') && strstr(field->name, "Color")){
              fprintf(file, "    config->%s.%s = &MAKE(COLOR32,%s);", row->name, field->name,value);
            }
            else if(strchr(value, ',')){
              fprintf(file, "    config->%s.%s = &MAKE(VECTOR3,%s);", row->name, field->name,value);
            }
            else
            fprintf(file, "    config->%s.%s = %s;", row->name, field->name,value);
          }
          else if(value[0]=='\'') {
            
              fprintf(file, "    config->%s.%s = %s;", row->name, field->name,value);
          }
          else{
            fprintf(file, "    config->%s.%s = \"", row->name, field->name);
            for (const char *s = value; *s; s++) {
              if (*s == '\\' || *s == '"') {
                fputc('\\', file);
              }
              fputc(*s, file);
            }
            fprintf(file, "\";");
          }
          fprintf(file, " %s\n", comment);
        }
      }
    }
  }
  fprintf(file, "}\n\n");

  // 清理函数
  // fprintf(file, "void Shutdown_%s(%s *config) {\n", structname, structname);
  // FOR_EACH_LIST(sheetRow_t, row, config) {
  //   if (row->name && strlen(row->name) > 0) {
  //     fprintf(file, "    if (config->%s) {\n", row->name);
  //     fprintf(file, "        MemFree(config->%s);\n", row->name);
  //     fprintf(file, "        config->%s = NULL;\n", row->name);
  //     fprintf(file, "    }\n");
  //   }
  // }
  // fprintf(file, "}\n");

  fclose(file);
  printf("Generated header file: %s\n", output_filename);
}

sheetRow_t *FS_ParseINI(LPCSTR fileName) {
  LPSTR buffer = FS_ReadFileIntoString(fileName);
  if (!buffer) {
    return NULL;
  }
  //    printf("%")
  sheetRow_t *config = FS_ParseINI_Buffer(buffer);
  if (!config) {
    fprintf(stderr, "Failed to parse %s\n", fileName);
  } else {
    FS_GenHeaderINI(fileName, config);
  }
  MemFree(buffer);
  return config;
}