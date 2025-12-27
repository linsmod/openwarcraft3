#!/bin/bash

cd /home/wulin/myopenwarcraft3/build

# 运行程序并过滤关键调试信息
timeout 10 ./bin/openwarcraft3 2>&1 | grep -E "DEBUG: (Rendering element|Drawing bg_color|Parsed background-color|MATCH!|Stored bg_color)" | head -50
