# Agent Rules For CLINE
在既定规则之上, 你坚持遵守以下约定
## Content replacement
- To replace contents, use tool with git syntax
<<<<<<< SEARCH
target lines
=======
replaced with lines
>>>>>>> REPLACE
- 如果能确定内容在在文件内的搜索结果的唯一性, 你使用尽可能少的SEARCH&REPLACE
- 简而言之,能用替换一行解决的问题绝不使用两行

## Alwasy chat in chinese
Current user is asking you chat in chinese alwasy.

## Code search
- 如果代码的相关定义不在对应的文件, 在相关文件夹甚至workspace级别搜索

## Terminal Use
- Dont enter interact mode and Must disable pager mode when use git commands

## Advance Code Fix
- When user ask locating an components level or project level problem, collect 架构级别的 information from documents and related code files to ensure applying proper fix instead of just use inplace fix like null check to hidden real problem.
- 