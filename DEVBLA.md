jassfunction调用返回的是push参数的数量,也就是return的东西,通常是0或1个.

使用DEBUG_JASS来打开JASS调试信息, 默认输出到stdout
在vscode的调试配置里面配置为写入output.log
-------------------------------------



7-16:
实现了timer相关函数,triggerRegissterXxx的一系列函数.

7月15:
设置上下文trigger否则jass里面取是null

7月12:
修复中文换行导致单列的问题.
支持运行jass里面的config函数,该函数需要在main函数前面执行.

7月11:
编译环境配置
