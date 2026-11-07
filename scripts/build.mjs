import fs from 'node:fs';
import path from 'node:path';
import { spawnSync } from 'node:child_process';
import { createRequire } from 'node:module';
import { Marked } from 'marked';
const require = createRequire(import.meta.url);
const hljs = require('highlight.js/lib/core');
hljs.registerLanguage('cpp', require('highlight.js/lib/languages/cpp'));
hljs.registerLanguage('python', require('highlight.js/lib/languages/python'));
hljs.registerLanguage('javascript', require('highlight.js/lib/languages/javascript'));
const root = path.resolve(import.meta.dirname, '..');
process.chdir(root);
const dist = path.join(root, 'dist');
fs.rmSync(dist, {recursive:true, force:true});
fs.mkdirSync(path.join(dist, 'assets'), {recursive:true});
const esc = s => String(s).replace(/[&<>"']/g, c => ({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[c]));
let headings = [];
const md = new Marked({gfm:true, breaks:false, renderer:{
 html({text}) { return esc(text); },
 heading({tokens,depth}) {
  const title = this.parser.parseInline(tokens);
  const id = `section-${headings.length+1}`;
  headings.push({id, text:title.replace(/<[^>]+>/g,''), level:depth});
  return `<h${depth} id="${id}">${title}</h${depth}>`;
 },
 code({text, lang}) {
  let language = (lang || 'text').split(/\s/)[0].toLowerCase();
  if (['c++','c','h','hpp'].includes(language)) language='cpp';
  const html = hljs.getLanguage(language) ? hljs.highlight(text, {language,ignoreIllegals:true}).value : esc(text);
  return `<div class="code-block"><div class="code-toolbar"><span>${esc(language==='cpp'?'C++':language)}</span><button class="copy-code" type="button">复制代码</button></div><pre><code class="hljs language-${esc(language)}">${html}</code></pre></div>`;
 },
 link({href,title,tokens}) {
  const safe = /^(https?:|mailto:|#)/i.test(href) ? href : '#';
  return `<a href="${esc(safe)}"${title?` title="${esc(title)}"`:''}${/^https?:/i.test(safe)?' target="_blank" rel="noopener noreferrer"':''}>${this.parser.parseInline(tokens)}</a>`;
 }
}});
const render = text => {headings=[];const html=md.parse(text);return {html, headings:[...headings]};};
const textify = text => text.replace(/```[\s\S]*?```/g,' ').replace(/[#*`>|_]/g,' ').replace(/\s+/g,' ').trim();
const specs = [
 ['Sorting foundations','三种基础排序','Selection Sort · Insertion Sort · Bubble Sort','sorting'],
 ['Pointers & linked lists','指针与链表','Custom types · Memory · Linked lists','memory'],
 ['Time complexity','时间复杂度','Big-O · Asymptotic analysis · Complexity','analysis'],
 ['Stacks & queues','栈与队列','Pointers · Stack · Queue','structures'],
 ['Recursion & sorting','递归与进阶排序','Recursion · Merge Sort · Quick Sort','sorting'],
 ['Algorithm correctness','算法正确性','Loop invariants · Proofs · Correctness','analysis'],
 ['Trees & BSTs','树与二叉搜索树','Binary trees · BST · Key-value pairs','structures'],
 ['Heaps & priority queues','堆与优先队列','Heapify · Heap Sort · Priority queues','structures'],
 ['Hashing & hash tables','哈希与哈希表','Hash functions · Collisions · Separate chaining','structures']
];
const weeks = specs.map(([title,zh,description,category],i)=>{
 const file=path.join(root, 'content/weeks',`week${i+1}.md`);
 if(!fs.existsSync(file)) throw new Error(`Missing week ${i+1}`);
 const raw = fs.readFileSync(file,'utf8');
 const rendered = render(raw);
 const data = {id:`week-${i+1}`, number:i+1, title, zh, description, category, ...rendered};
 fs.writeFileSync(path.join(dist,'assets',`week-${i+1}.json`),JSON.stringify(data));
 return {...data,html:undefined,headings:undefined,text:textify(raw),sections:rendered.headings.length};
});
const allowed=new Set(['.pdf','.docx','.xlsx','.cpp','.h','.hpp','.md','.png','.jpg','.jpeg']);
const groups=[];
function walk(folder) {
 return fs.readdirSync(folder,{withFileTypes:true}).flatMap(d=>{
  if(d.name.startsWith('.') || d.name.endsWith('.dSYM'))return [];
  const p=path.join(folder,d.name);return d.isDirectory()?walk(p):d.isFile()&&allowed.has(path.extname(p).toLowerCase())?[p]:[];
 });
}
const base=path.join(root,'content/assignments');
for(const dir of fs.readdirSync(base,{withFileTypes:true}).filter(d=>d.isDirectory()).sort((a,b)=>a.name.localeCompare(b.name,undefined,{numeric:true}))) {
 const files=walk(path.join(base,dir.name)).map((file,i)=>{
  const rel=path.relative(base,file).split(path.sep).join('/');
  const ext=path.extname(file).toLowerCase();
  const dest=path.join(dist,'resources',rel);
  fs.mkdirSync(path.dirname(dest),{recursive:true});fs.copyFileSync(file,dest);
  const raw=['.cpp','.h','.hpp','.md'].includes(ext)?fs.readFileSync(file,'utf8'):'';
  const rendered=ext==='.md'?render(raw).html:raw?`<pre><code class="hljs">${hljs.highlight(raw,{language:'cpp',ignoreIllegals:true}).value}</code></pre>`:'';
  return {id:`file-${i}`,name:path.basename(file),path:rel,url:'resources/'+rel.split('/').map(encodeURIComponent).join('/'),ext:ext.slice(1),size:fs.statSync(file).size,html:rendered,raw};
 });
 if(!files.length)continue;
 const id=dir.name;
 const type=id.match(/^(HD|P|C|D)/)?.[1]||'Other';
 const descriptions={P:'基础实践',C:'进阶实践',D:'深入探索',HD:'挑战项目',Other:'补充练习'};
 const data={id,type,description:descriptions[type],files};
 fs.writeFileSync(path.join(dist,'assets',`assignment-${encodeURIComponent(id)}.json`),JSON.stringify(data));
 groups.push({id,type,description:descriptions[type],count:files.length,pdfCount:files.filter(f=>f.ext==='pdf').length,codeCount:files.filter(f=>['cpp','h','hpp'].includes(f.ext)).length,fileNames:files.map(f=>f.name)});
}
const config=JSON.parse(fs.readFileSync('site.config.json','utf8'));
fs.writeFileSync(path.join(dist,'assets','catalog.json'),JSON.stringify({config,weeks,assignments:groups,totalFiles:groups.reduce((s,g)=>s+g.count,0)}));
const compiler=spawnSync(process.execPath,[path.join(root,'node_modules/typescript/bin/tsc'),'-p','tsconfig.json'],{stdio:'inherit'});
if(compiler.status!==0)process.exit(compiler.status||1);
fs.copyFileSync('src/style.css',path.join(dist,'assets','style.css'));
fs.copyFileSync('index.html',path.join(dist,'index.html'));
fs.copyFileSync('public/favicon.svg',path.join(dist,'favicon.svg'));
console.log(`Built ${weeks.length} weeks, ${groups.length} assignment groups and ${groups.reduce((s,g)=>s+g.count,0)} resources.`);

// Self-contained entry for opening the built site directly from disk.
const bundle={};
for(const f of fs.readdirSync(path.join(dist,'assets')).filter(n=>n.endsWith('.json')))bundle['assets/'+f]=JSON.parse(fs.readFileSync(path.join(dist,'assets',f),'utf8'));
const json=JSON.stringify(bundle).replace(/</g,'\\u003c');
const css=fs.readFileSync(path.join(dist,'assets/style.css'),'utf8');
const js=fs.readFileSync(path.join(dist,'assets/main.js'),'utf8').replace(/<\/script/gi,'<\\/script');
const offline=fs.readFileSync('index.html','utf8').replace('<link rel="stylesheet" href="./assets/style.css">',()=>`<style>${css}</style>`).replace('<script type="module" src="./assets/main.js"></script>',()=>`<script>globalThis.__STUDY_DATA__=${json};</script><script>${js}</script>`);
fs.writeFileSync(path.join(dist,'offline.html'),offline);
