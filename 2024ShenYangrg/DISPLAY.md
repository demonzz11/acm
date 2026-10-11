# GitHub 显示检查

[返回题单](README.md)

## 使用的 Markdown 规范

- 所有标题、表格和列表按标准 GitHub Markdown 编写，列表与前后正文之间留空行。
- 行内公式使用 GitHub 的美元符号包裹代码语法，独立公式使用 `math` 围栏。
- 数学比较符用 `\lt`、`\gt`，函数名用 `\mathrm{...}`；矩阵、分段式和对齐换行保留原始反斜杠。
- 代码、样例与命令使用带语言标记的围栏，站内文档和源码使用相对路径。
- 不将原始 PDF 双栏抽取文本直接当成排版后的题解，权威题面另有官方 PDF 链接。

## 检查方法

逐篇通过 GitHub 的 Markdown API 渲染，比较源文件与返回 HTML 中的数学内容、表格数量和数据行数，检查本地相对链接及锚点。

发布后用真实浏览器打开 GitHub 的各篇文档，等待 GitHub 自带数学组件完成渲染，检查所有公式是否生成 MathML、是否出现错误节点，并检查页面中的表格与标题。检查摘要保存在 [sources/display-report.json](sources/display-report.json)。

显示检查与算法验证分别记录；算法检查见 [VALIDATION.md](VALIDATION.md)。
