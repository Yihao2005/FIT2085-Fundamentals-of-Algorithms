# FIT2085 · Study Atlas

个人数据结构与算法笔记站：九周笔记、作业报告与 C++ 源码。

## 直接使用

不需要安装任何工具：打开 `dist/offline.html`。请保留整个 `dist/` 目录；PDF、图片及下载链接依赖其旁边的 `resources/`。如内置预览器未显示 PDF，点击“打开”或“下载”。

`dist/index.html` 是正常托管入口，需要通过 HTTP 服务打开；不要直接双击它。

## 本地开发

安装 Node.js 22 或更高版本，并在项目目录运行：

```sh
npm install
npm run dev
```

浏览器访问终端打印的 `http://127.0.0.1:4173`。修改 `src/` 或 `content/` 后自动重新构建，刷新浏览器即可查看。

```sh
npm run build    # 导入内容、TypeScript 严格检查、生成 dist/
npm run preview  # 预览已构建版本
```

本次环境无法下载 Vue/Vite，因此实际首版使用原生 TypeScript、CSS 和 Node.js 构建脚本。没有 Vue/Vite 依赖，没有运行时 CDN 或外部字体请求。Markdown 用 marked 处理，代码高亮在构建时完成。后续可以保留 content/ 和导入逻辑，迁入 Vue。

## 内容维护

```text
content/
  weeks/week1.md ... week9.md  # 笔记唯一来源
  assignments/P1/...          # 作业报告、源码和附件
src/
  main.ts                     # 路由、搜索、页面与交互
  style.css                   # 样式和响应式布局
scripts/
  build.mjs                   # 读取资料并生成站点
  serve.mjs                   # 本地静态服务器
  dev.mjs                     # 开发时监听变化并重建
site.config.json              # 站点信息
```

- 原始 `docs/` 留在桌面的 FIT2085 仓库中，本站不读取、不修改、不发布它。
- 当前项目的 content/ 是桌面 content/ 的独立副本；之后请选定这个项目的 content/ 作为网站维护来源，不会自动同步桌面副本。
- Week 2–9 原文保留；Week 1 在原代码前增加已明确标注的排序原理和复杂度说明。
- 允许导入 PDF、DOCX、XLSX、CPP、H、HPP、Markdown、PNG 和 JPG；排除隐藏目录、可执行程序及 dSYM。
- 新增作业时，在 content/assignments/ 下新增文件夹，重新构建即可出现在资料库。
- PDF 支持浏览器预览；C++ 支持高亮、复制、下载；Word 和 Excel 提供下载。
- 搜索覆盖所有周笔记全文及作业文件名，不包含 PDF/Word 正文。
- 周标题和主题分类配置在 scripts/build.mjs 的 specs 中。
- 文件列表按文件名排序；P2 多版本报告均保留，可以自行重命名，标明推荐版本。

## 部署

构建后把整个 dist/ 上传到静态托管服务即可。路由使用 hash，支持子目录托管。部署资料以 content/ 为准，勿把原始 docs/ 整个上传。

本次没有加入自动化测试、CI/CD、学习进度或后端。

## 本次验证范围

构建和 TypeScript 严格检查通过；确认导入 9 周笔记、18 组作业、69 份资源，并检查资源文件完整性。当前环境不允许启动本地服务器，浏览器也不允许 file: 页面，因此未完成浏览器视觉及交互验证。

在线发布未完成：环境无法解析 git.chatgpt-team.site，源代码同步失败。已创建的私人 Site 尚未发布，不能作为在线网站使用。
