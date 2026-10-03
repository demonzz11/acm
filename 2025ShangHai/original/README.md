# 原代码归档

本文件夹完整保存整理前 `2025ShangHai/` 中的四个文件：

- `G.cpp`
- `H.cpp`
- `G.debug.exe`
- `H.debug.exe`

文件只移动位置，没有改动内容。[SHA256.json](SHA256.json) 保存移动前的 SHA-256 摘要，`../verify.py` 每次运行会重新核对已有文件。缺失的本地调试程序会被跳过；两份原源码始终必须存在并通过核对。`../.gitattributes` 禁止 Git 转换原源码的换行，保证跨平台克隆后仍保留原字节。

原代码的算法复盘见 [G 题解](../solutions/G.md)、[H 题解](../solutions/H.md)，新参考实现放在 [std/](../std/)。没有根据文件名推断原代码已经在线 AC。

调试程序仍在本地保留，本目录 `.gitignore` 默认忽略它们，上传 GitHub 时不会自动包含二进制文件。
