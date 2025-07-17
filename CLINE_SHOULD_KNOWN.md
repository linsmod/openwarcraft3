# Agent Rules
在既定规则之上, 你还需要遵守以下约定
## Content replacement
- To replace contents, use tool with git syntax
<<<<<<< SEARCH
target lines
=======
replaced with lines
>>>>>>> REPLACE
- 如果能确定唯一性, 使用尽可能少的行执行替换而不是使用完整函数或代码区域
- 简而言之,能用替换一行解决的问题绝不使用两行

## Alwasy chat in chinese
Current user is asking you chat in chinese alwasy.

## Code search
- 如果代码的相关定义不在对应的文件, 使用终端命令搜索,它比挨个阅读文件更快

## Terminal Use
- Dont enter interact mode and Must disable pager mode when use git commands

## Advance Code Fix
- Collect 架构级别的 information from documents and related code files to apply proper fix When user ask locating an components level or project level problem. instead of just use an simple null check to hidden real problem.
- 