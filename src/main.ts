type Heading = {id:string;text:string;level:number};
type Week = {id:string;number:number;title:string;zh:string;description:string;category:string;sections:number;text:string;html?:string;headings?:Heading[]};
type Assignment = {id:string;type:string;description:string;count:number;pdfCount:number;codeCount:number;fileNames:string[]};
type Resource = {id:string;name:string;path:string;url:string;ext:string;size:number;html:string;raw:string};
type AssignmentDetail = {id:string;type:string;description:string;files:Resource[]};
type Catalog = {config:{title:string;course:string;subtitle:string};weeks:Week[];assignments:Assignment[];totalFiles:number};
const app = document.querySelector<HTMLDivElement>('#app')!;
let catalog:Catalog;
let requestId=0;
let observer:IntersectionObserver|undefined;
const esc = (s:unknown) => String(s).replace(/[&<>"']/g,c=>({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[c]!));
const icons:Record<string,string>={
 cube:'<path d="m12 3 9 5v9l-9 5-9-5V8z"/><path d="m3 8 9 5 9-5M12 13v9M12 3v10"/>',
 home:'<rect x="3" y="3" width="7" height="7" rx="1"/><rect x="14" y="3" width="7" height="7" rx="1"/><rect x="3" y="14" width="7" height="7" rx="1"/><rect x="14" y="14" width="7" height="7" rx="1"/>',
 book:'<path d="M12 5c-3-2-6-2-10-1v15c4-1 7-1 10 1 3-2 6-2 10-1V4c-4-1-7-1-10 1v15"/>',
 folder:'<path d="M3 7V5a2 2 0 0 1 2-2h5l2 3h7a2 2 0 0 1 2 2v11a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2V7z"/>',
 search:'<circle cx="10.5" cy="10.5" r="6.5"/><path d="m16 16 5 5"/>',
 arrow:'<path d="M4 12h16m-6-6 6 6-6 6"/>',
 chevron:'<path d="m9 5 7 7-7 7"/>',
 moon:'<path d="M20.5 13A8.5 8.5 0 0 1 11 3.5 8.5 8.5 0 1 0 20.5 13Z"/>',
 sun:'<circle cx="12" cy="12" r="4"/><path d="M12 2v2m0 16v2M2 12h2m16 0h2M5 5l1 1m12 12 1 1M5 19l1-1M18 6l1-1"/>',
 code:'<path d="m8 5-6 7 6 7m8-14 6 7-6 7M14 3l-4 18"/>',
 file:'<path d="M14 2H5v20h14V7zM14 2v6h5M8 13h8m-8 4h6"/>',
 download:'<path d="M12 3v12m-5-5 5 5 5-5M4 16v5h16v-5"/>',
 menu:'<path d="M4 6h16M4 12h16M4 18h16"/>',
 close:'<path d="m6 6 12 12M6 18 18 6"/>',
 external:'<path d="M13 3h8v8m0-8L10 14M10 3H3v18h18v-7"/>',
 check:'<path d="m5 12 4 4L19 6"/>',
 layers:'<path d="m12 3 10 5-10 5L2 8zM2 12l10 5 10-5M2 16l10 5 10-5"/>',
 clock:'<circle cx="12" cy="12" r="9"/><path d="M12 7v5l3 2"/>',
 github:'<path d="M9 19c-5 2-5-3-7-3m14 6v-4c0-1-.3-1.7-1-2 3-.3 6-1.5 6-6 0-1.5-.5-2.5-1.5-3.5.2-1 .2-2-.3-3.2-2-.2-3.2 1.2-3.2 1.2-2.5-.7-5.5-.7-8 0 0 0-1.2-1.4-3.2-1.2-.5 1.2-.5 2.2-.3 3.2C3.5 7.5 3 8.5 3 10c0 4.5 3 5.7 6 6-.7.3-1 1-1 2v4"/>'
};
const icon=(name:string,cls='')=>`<svg class="icon ${cls}" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.65" stroke-linecap="round" stroke-linejoin="round" aria-hidden="true">${icons[name]||icons.file}</svg>`;
const pad=(n:number)=>String(n).padStart(2,'0');
const fmt=(n:number)=>n>1024*1024?`${(n/1024/1024).toFixed(1)} MB`:`${Math.max(1,Math.round(n/1024))} KB`;
function readStorage(k:string){try{return localStorage.getItem(k);}catch{return null;}}
function notify(text:string){let toast=document.querySelector('.toast');if(toast)toast.remove();toast=document.createElement('div');toast.className='toast';toast.setAttribute('role','status');toast.textContent=text;document.body.append(toast);setTimeout(()=>toast?.remove(),2500);}
async function getJSON<T>(url:string):Promise<T>{const embedded=(globalThis as typeof globalThis & {__STUDY_DATA__?:Record<string,unknown>}).__STUDY_DATA__;if(embedded&&url in embedded)return embedded[url] as T;const r=await fetch(url);if(!r.ok)throw new Error(`无法读取资料（${r.status}）`);return r.json();}
function layout(){
 app.innerHTML=`<a class="skip-link" href="#main" data-anchor="main">跳转到正文</a><button class="sidebar-shade" aria-label="关闭导航"></button><aside class="sidebar" aria-label="主导航"><a href="#/" class="brand"><span class="brand-symbol">${icon('cube')}</span><span>Study Atlas<small>FIT2085 · LEARNING SPACE</small></span></a><div class="sidebar-inner"><div class="nav-label">工作空间</div><a href="#/" class="nav-item" data-route="home">${icon('home')}<span>学习概览</span></a><a href="#/notes" class="nav-item" data-route="notes">${icon('book')}<span>学习笔记</span><span class="nav-count">09</span></a><a href="#/assignments" class="nav-item" data-route="assignments">${icon('folder')}<span>作业资料库</span></a><div class="nav-divider"></div><div class="nav-label row-between">课程内容<span>9 WEEKS</span></div><nav class="week-nav">${catalog.weeks.map(w=>`<a href="#/week/${w.number}" data-week="${w.number}" class="week-link"><span class="week-index">${pad(w.number)}</span><span>${esc(w.zh)}</span><span class="week-dot"></span></a>`).join('')}</nav><div class="sidebar-note"><span class="tiny-stars">✳</span><p>把知识连成地图。<small>Read. Understand. Build.</small></p></div></div><div class="sidebar-bottom"><span class="avatar">Y</span><span>My learning space<small>个人课程资料库</small></span><span class="status-dot"></span></div></aside><div class="workspace"><header class="topbar"><button class="icon-button mobile-menu" aria-label="打开导航">${icon('menu')}</button><div class="breadcrumb"><span>Workspace</span>${icon('chevron')}<strong id="breadcrumb">学习概览</strong></div><div class="top-actions"><button class="search-trigger">${icon('search')}<span>搜索笔记与资料</span><kbd>⌘ K</kbd></button><button class="icon-button theme-toggle" aria-label="切换深浅色主题">${icon(readStorage('fit2085-theme')==='dark'?'sun':'moon')}</button></div></header><main id="main" tabindex="-1"></main><footer class="site-footer"><span>FIT2085 · Fundamentals of Algorithms</span><span>从理解，到实现。${icon('cube')}</span></footer></div><dialog class="search-dialog" aria-label="搜索笔记与作业"><div class="search-field">${icon('search')}<input id="global-search" type="search" placeholder="搜索概念、周次、作业编号…" autocomplete="off" aria-label="搜索关键词"><button class="dialog-close" aria-label="关闭搜索">Esc</button></div><div class="search-help">搜索 Week 1–9 笔记全文与作业文件名</div><div class="search-results"></div><div class="search-footer"><span>Enter 打开首个结果</span><span>Esc 关闭</span></div></dialog>`;
}
function updateNav(route:string,week=''){
 document.querySelectorAll('.nav-item').forEach(a=>a.classList.toggle('active',(a as HTMLElement).dataset.route===route));
 document.querySelectorAll('.week-link').forEach(a=>{const yes=(a as HTMLElement).dataset.week===week;a.classList.toggle('active',yes);if(yes)a.setAttribute('aria-current','page');else a.removeAttribute('aria-current');});
}
function heading(eyebrow:string,title:string,desc:string,right=''){return `<div class="page-heading"><div><div class="eyebrow">${eyebrow}</div><h1>${title}</h1><p>${desc}</p></div>${right}</div>`;}
function card(w:Week){return `<a class="note-card" href="#/week/${w.number}" data-category="${w.category}"><div class="card-meta"><span class="week-tag">WEEK ${pad(w.number)}</span>${icon('arrow')}</div><div class="topic-art topic-${w.category}" aria-hidden="true">${w.number===1||w.number===5?'<i></i><i></i><i></i><i></i><i></i>':w.number===7?'<span>○</span><span>╱ ╲</span><span>○　○</span>':w.number===9?'<span class="hash-art">#</span>':w.number===3||w.number===6?'<span class="complexity-art">O(n)</span>':'<span class="bracket-art">[ &nbsp; ]</span>'}</div><h3>${esc(w.zh)}</h3><div class="card-title-en">${esc(w.title)}</div><p>${esc(w.description)}</p><div class="card-bottom">${icon('file')}<span>${w.sections} 个知识小节</span><span class="card-go">开始阅读 ${icon('chevron')}</span></div></a>`;}
function home(){
 const counts=[['09','周课程笔记','book'],[String(catalog.assignments.length),'组作业资料','folder'],[String(catalog.totalFiles),'份源码与文档','code']];
 return `<div class="page home-page"><div class="home-intro"><div class="eyebrow">YOUR PERSONAL KNOWLEDGE BASE</div><span class="semester-pill"><i></i> FIT2085 / Algorithms</span></div><section class="hero"><div class="hero-copy"><span class="hero-kicker">从一行代码，到一个清晰的思路。</span><h1>让每一个知识点，<br>都有迹可循<span>。</span></h1><p>九周的算法与数据结构笔记，和你亲手完成的作业。<br class="desktop-br">在这里整理、查阅，把学过的知识真正串起来。</p><div class="hero-actions"><a class="button primary" href="#/week/1">开始阅读 ${icon('arrow')}</a><a class="button text-button" href="#/assignments">浏览作业资料 ${icon('external')}</a></div></div><div class="hero-visual" aria-hidden="true"><span class="visual-label">A LITTLE ORDER. A LOT OF CLARITY.</span><div class="sort-chart"><span style="--height:35%;--delay:0"></span><span style="--height:48%;--delay:1"></span><span style="--height:43%;--delay:2"></span><span style="--height:68%;--delay:3"></span><span style="--height:59%;--delay:4"></span><span style="--height:82%;--delay:5"></span><span style="--height:100%;--delay:6"></span></div><div class="visual-bottom"><span>01 — 09</span><span>knowledge, connected.</span></div><span class="floating-label">${icon('code')} Think in algorithms</span></div></section><section class="stats-row">${counts.map(([n,t,i])=>`<div class="stat">${icon(i)}<strong>${n}</strong><span>${t}</span></div>`).join('')}<div class="stat-note">一份属于你的<br><strong>算法学习地图</strong></div></section><section><div class="section-heading"><div><span class="eyebrow">THE LEARNING PATH</span><h2>沿着知识的脉络</h2></div><a class="inline-link" href="#/notes">全部课程 ${icon('arrow')}</a></div><div class="note-grid">${catalog.weeks.slice(0,3).map(card).join('')}</div><div class="learning-strip"><span class="strip-label">接下来</span>${catalog.weeks.slice(3).map(w=>`<a href="#/week/${w.number}"><span>${pad(w.number)}</span>${esc(w.zh)}${icon('chevron')}</a>`).join('')}</div></section><section class="assignment-banner"><div class="banner-icon">${icon('folder')}</div><div><span class="eyebrow">FROM THEORY TO PRACTICE</span><h2>每一份作业，都是理解的证据。</h2><p>报告、C++ 实现与补充练习，按作业编号集中查阅。</p></div><a class="button outline" href="#/assignments">打开资料库 ${icon('arrow')}</a></section><div class="home-footnote"><span class="status-dot"></span>笔记原文保留 · Week 1 补充基础排序说明 · 资料按需加载</div></div>`;
}
function notes(){return `<div class="page">${heading('COURSE NOTES / 01—09','学习笔记','从基础排序到哈希表，按你的学习顺序展开。')}<div class="filter-row" role="group" aria-label="按主题筛选笔记">${[['all','全部内容'],['sorting','排序与递归'],['memory','指针与内存'],['analysis','分析与证明'],['structures','数据结构']].map(([id,t],i)=>`<button class="filter-chip ${i===0?'selected':''}" data-note-filter="${id}">${t}</button>`).join('')}<span class="filter-total">9 篇笔记</span></div><div class="note-grid all-notes">${catalog.weeks.map(card).join('')}</div></div>`;}
function assignments(){return `<div class="page">${heading('ASSIGNMENT LIBRARY','作业资料库','按作业编号整理的报告、源码与练习。选择一组资料开始浏览。',`<span class="count-badge">${catalog.totalFiles} 份文件</span>`)}<div class="library-toolbar"><div class="filter-row" role="group" aria-label="按作业类型筛选">${['all','P','C','D','HD','Other'].map((id,i)=>`<button class="filter-chip ${i===0?'selected':''}" data-assignment-filter="${id}">${id==='all'?'全部资料':id==='Other'?'补充练习':id+' 系列'}</button>`).join('')}</div><label class="library-search">${icon('search')}<input type="search" id="assignment-search" placeholder="查找编号或文件名" aria-label="搜索作业"></label></div><div class="assignment-grid">${catalog.assignments.map(g=>`<a class="assignment-card" href="#/assignment/${encodeURIComponent(g.id)}" data-type="${g.type}" data-search="${esc(g.id+' '+g.fileNames.join(' '))}"><div class="assignment-card-top"><span class="folder-tile type-${g.type}">${icon('folder')}</span><span class="type-badge">${g.type==='Other'?'EXTRA':g.type}</span></div><h3>${esc(g.id)}</h3><p>${g.description}</p><div class="assignment-card-bottom"><span>${g.count} 份文件</span><span>${g.pdfCount?`${g.pdfCount} PDF`:''}${g.pdfCount&&g.codeCount?' · ':''}${g.codeCount?`${g.codeCount} 源码`:''}</span>${icon('arrow')}</div></a>`).join('')}</div><div class="library-empty" hidden>${icon('search')}<h3>没有找到相关作业</h3><p>试试其他编号或文件名。</p></div></div>`;}
function reader(w:Week){
 const toc=w.headings||[];
 return `<div class="reader-layout"><article class="reader"><div class="reader-top"><a href="#/notes" class="back-link">← 全部笔记</a><span class="week-tag">WEEK ${pad(w.number)} / 09</span></div><div class="reader-heading"><div class="eyebrow">${esc(w.title.toUpperCase())}</div><h1>${esc(w.zh)}</h1><p>${esc(w.description)}</p><div class="reader-meta">${icon('book')} 原文笔记 <span>·</span> ${toc.length} 个知识小节 <span>·</span> C++</div></div><div class="mobile-toc"><label>本页目录<select id="mobile-toc-select"><option value="">跳转到知识小节…</option>${toc.map(h=>`<option value="${h.id}">${esc(h.text)}</option>`).join('')}</select></label></div><div class="prose" id="article-content">${w.html}</div><nav class="reader-pagination" aria-label="上一篇和下一篇">${w.number>1?`<a href="#/week/${w.number-1}"><small>← 上一周</small><strong>${esc(catalog.weeks[w.number-2].zh)}</strong></a>`:'<span></span>'}${w.number<9?`<a href="#/week/${w.number+1}"><small>下一周 →</small><strong>${esc(catalog.weeks[w.number].zh)}</strong></a>`:`<a href="#/assignments"><small>从理论到实践 →</small><strong>浏览作业资料库</strong></a>`}</nav></article><aside class="toc"><div class="toc-label">ON THIS PAGE</div><div class="toc-links">${toc.map(h=>`<a href="#${h.id}" data-anchor="${h.id}" class="toc-level-${Math.min(h.level,3)}">${esc(h.text)}</a>`).join('')}</div><a class="toc-back" href="#" data-anchor="main">↑ 返回顶部</a></aside></div>`;
}
function assignmentPage(g:AssignmentDetail,selected?:string){
 const file=g.files.find(f=>f.id===selected)||g.files.find(f=>f.ext==='pdf')||g.files[0];
 const isCode=['cpp','h','hpp'].includes(file.ext);
 const image=['png','jpg','jpeg'].includes(file.ext);
 const pdfURL=`${file.url}#toolbar=1&navpanes=0`;
 return `<div class="page resource-page"><a href="#/assignments" class="back-link">← 作业资料库</a>${heading(`ASSIGNMENT / ${esc(g.type)}`,esc(g.id),`${g.description} · ${g.files.length} 份文件`)}<div class="resource-layout"><aside class="file-panel"><div class="file-panel-title">${icon('folder')} 文件列表 <span>${g.files.length}</span></div>${g.files.map(f=>`<a class="file-row ${file.id===f.id?'selected':''}" href="#/assignment/${encodeURIComponent(g.id)}/${f.id}" ${file.id===f.id?'aria-current="true"':''}>${icon(['cpp','h','hpp'].includes(f.ext)?'code':'file')}<span>${esc(f.name)}<small>${f.ext.toUpperCase()} · ${fmt(f.size)}</small></span>${file.id===f.id?icon('chevron'):''}</a>`).join('')}</aside><section class="resource-viewer"><div class="resource-toolbar"><div>${icon(isCode?'code':'file')}<strong>${esc(file.name)}</strong></div><div>${file.ext==='pdf'?`<a href="${file.url}" target="_blank" rel="noopener noreferrer" class="small-action" title="在新窗口打开">${icon('external')}<span>打开</span></a>`:''}${isCode?'<button class="small-action copy-source">复制源码</button>':''}<a href="${file.url}" download="${esc(file.name)}" class="small-action">${icon('download')}<span>下载</span></a></div></div>${file.ext==='pdf'?`<div class="pdf-hint">如浏览器未显示预览，可使用右上角“打开”或“下载”。</div><iframe class="pdf-frame" title="${esc(file.name)} PDF 预览" src="${pdfURL}"></iframe>`:isCode?`<div class="source-code">${file.html}</div>`:file.ext==='md'?`<div class="prose resource-prose">${file.html}</div>`:image?`<div class="image-preview"><img src="${file.url}" alt="${esc(file.name)}"></div>`:`<div class="download-preview">${icon('file')}<span class="type-badge">${file.ext.toUpperCase()}</span><h2>${esc(file.name)}</h2><p>此文件可下载后使用对应应用打开。</p><a class="button primary" href="${file.url}" download="${esc(file.name)}">${icon('download')} 下载文件 · ${fmt(file.size)}</a></div>`}</section></div></div>`;
}
let activeDetail:AssignmentDetail|undefined;
let activeResourceId:string|undefined;
async function route(){
 const id=++requestId;
 observer?.disconnect();
 const parts=(location.hash.slice(1)||'/').split('/').filter(Boolean);
 const main=document.querySelector<HTMLElement>('#main')!;
 document.body.classList.remove('nav-open');
 (document.querySelector('.search-dialog') as HTMLDialogElement)?.close();
 let title='学习概览';
 try{
  if(!parts.length){updateNav('home');main.innerHTML=home();}
  else if(parts[0]==='notes'){title='学习笔记';updateNav('notes');main.innerHTML=notes();}
  else if(parts[0]==='assignments'){title='作业资料库';updateNav('assignments');main.innerHTML=assignments();}
  else if(parts[0]==='week'){
   const n=Number(parts[1]);const meta=catalog.weeks.find(w=>w.number===n);if(!meta)throw new Error('没有找到这周的笔记');
   title=meta.zh;updateNav('notes',String(n));main.innerHTML='<div class="loading-state">正在打开笔记…</div>';
   const data=await getJSON<Week>(`assets/week-${n}.json`);if(id!==requestId)return;main.innerHTML=reader(data);
  } else if(parts[0]==='assignment'){
   const group=decodeURIComponent(parts[1]||'');if(!catalog.assignments.some(g=>g.id===group))throw new Error('没有找到这组作业资料');
   title=`作业资料库 / ${group}`;updateNav('assignments');main.innerHTML='<div class="loading-state">正在打开资料…</div>';
   const data=await getJSON<AssignmentDetail>(`assets/assignment-${encodeURIComponent(group)}.json`);if(id!==requestId)return;
   activeDetail=data;activeResourceId=parts[2];main.innerHTML=assignmentPage(data,parts[2]);
  }else{throw new Error('页面不存在');}
  if(id!==requestId)return;
  document.querySelector('#breadcrumb')!.textContent=title;
  document.title=`${title} · FIT2085 Study Atlas`;
  window.scrollTo({top:0});
  setupTOC();
 }catch(e){if(id!==requestId)return;main.innerHTML=`<div class="page error-page">${icon('file')}<h1>${esc((e as Error).message)}</h1><p>请返回目录重新选择；如果资料加载失败，可以刷新页面重试。</p><a href="#/" class="button primary">返回学习概览</a></div>`;}
}
function setupTOC(){
 if(!document.querySelector('.toc'))return;
 observer=new IntersectionObserver(entries=>{const visible=entries.find(e=>e.isIntersecting);if(!visible)return;document.querySelectorAll('.toc-links a').forEach(a=>a.classList.toggle('current',(a as HTMLElement).dataset.anchor===visible.target.id));},{rootMargin:'-90px 0px -65% 0px'});
 document.querySelectorAll('.prose h1,.prose h2,.prose h3').forEach(h=>observer!.observe(h));
}
function openSearch(){const dialog=document.querySelector<HTMLDialogElement>('.search-dialog')!;if(!dialog.open)dialog.showModal();document.querySelector<HTMLInputElement>('#global-search')!.focus();search();}
function search(){
 const q=document.querySelector<HTMLInputElement>('#global-search')!.value.trim().toLowerCase();
 const results=document.querySelector('.search-results')!;
 if(!q){results.innerHTML=`<div class="search-suggestions"><span class="eyebrow">试试这些关键词</span><div>${['sorting','pointer','hash','P1'].map(t=>`<button class="suggestion" data-query="${t}">${t}</button>`).join('')}</div><p>输入关键词，搜索你的课程笔记与作业。</p></div>`;return;}
 const words=q.split(/\s+/);
 const weeks=catalog.weeks.filter(w=>words.every(t=>`${w.zh} ${w.title} ${w.description} week ${w.number} ${w.text}`.toLowerCase().includes(t)));
 const groups=catalog.assignments.filter(g=>words.every(t=>`${g.id} ${g.fileNames.join(' ')}`.toLowerCase().includes(t)));
 function snippet(w:Week){const lower=w.text.toLowerCase();const index=lower.indexOf(words[0]);const start=Math.max(0,index-45);return `${start?'…':''}${esc(w.text.slice(start,start+150))}…`;}
 results.innerHTML=(!weeks.length&&!groups.length)?`<div class="search-empty">${icon('search')}<h3>没有找到“${esc(q)}”</h3><p>试试英文概念、周次或作业编号。</p></div>`:`<div class="search-result-count">${weeks.length+groups.length} 个结果</div>${weeks.length?'<div class="search-category">学习笔记</div>':''}${weeks.map(w=>`<a class="search-result" href="#/week/${w.number}">${icon('book')}<div><strong>Week ${w.number} · ${esc(w.zh)}</strong><p>${snippet(w)}</p></div>${icon('arrow')}</a>`).join('')}${groups.length?'<div class="search-category">作业资料</div>':''}${groups.map(g=>`<a class="search-result" href="#/assignment/${encodeURIComponent(g.id)}">${icon('folder')}<div><strong>${esc(g.id)} · ${g.count} 份文件</strong><p>${esc(g.fileNames.join(' · '))}</p></div>${icon('arrow')}</a>`).join('')}`;
}
async function copy(text:string,button?:HTMLButtonElement){
 try{await navigator.clipboard.writeText(text);if(button){const old=button.textContent;button.textContent='已复制 ✓';setTimeout(()=>{if(button.isConnected)button.textContent=old;},1800);}notify('已复制到剪贴板');}catch{notify('浏览器未允许复制，请选中代码手动复制');}
}
function filterAssignments(){
 const type=(document.querySelector('[data-assignment-filter].selected') as HTMLElement)?.dataset.assignmentFilter||'all';
 const q=document.querySelector<HTMLInputElement>('#assignment-search')?.value.trim().toLowerCase()||'';
 let visible=0;document.querySelectorAll<HTMLElement>('.assignment-card').forEach(el=>{const show=(type==='all'||el.dataset.type===type)&&el.dataset.search!.toLowerCase().includes(q);el.hidden=!show;if(show)visible++;});
 const empty=document.querySelector<HTMLElement>('.library-empty');if(empty)empty.hidden=visible>0;
}
function events(){
 document.addEventListener('click',e=>{
  const el=e.target as HTMLElement;
  if(el.closest('.search-trigger'))openSearch();
  if(el.closest('.dialog-close'))document.querySelector<HTMLDialogElement>('.search-dialog')!.close();
  if(el.closest('.mobile-menu'))document.body.classList.toggle('nav-open');
  if(el.closest('.sidebar-shade'))document.body.classList.remove('nav-open');
  if(el.closest('.theme-toggle')){const theme=document.documentElement.dataset.theme==='dark'?'light':'dark';document.documentElement.dataset.theme=theme;try{localStorage.setItem('fit2085-theme',theme);}catch{}document.querySelector('.theme-toggle')!.innerHTML=icon(theme==='dark'?'sun':'moon');}
  const copyBtn=el.closest<HTMLButtonElement>('.copy-code');if(copyBtn)void copy(copyBtn.closest('.code-block')!.querySelector('code')!.textContent||'',copyBtn);
  const sourceBtn=el.closest<HTMLButtonElement>('.copy-source');if(sourceBtn&&activeDetail){const file=activeDetail.files.find(f=>f.id===activeResourceId)||activeDetail.files.find(f=>f.ext==='pdf')||activeDetail.files[0];void copy(file.raw,sourceBtn);}
  const anchor=el.closest<HTMLElement>('[data-anchor]');if(anchor){e.preventDefault();const target=document.getElementById(anchor.dataset.anchor!);target?.scrollIntoView({behavior:matchMedia('(prefers-reduced-motion: reduce)').matches?'auto':'smooth',block:'start'});}
  const filter=el.closest<HTMLElement>('[data-note-filter]');if(filter){document.querySelectorAll('[data-note-filter]').forEach(b=>b.classList.toggle('selected',b===filter));let count=0;document.querySelectorAll<HTMLElement>('.all-notes .note-card').forEach(c=>{c.hidden=filter.dataset.noteFilter!=='all'&&c.dataset.category!==filter.dataset.noteFilter;if(!c.hidden)count++;});document.querySelector('.filter-total')!.textContent=`${count} 篇笔记`;}
  const afilter=el.closest<HTMLElement>('[data-assignment-filter]');if(afilter){document.querySelectorAll('[data-assignment-filter]').forEach(b=>b.classList.toggle('selected',b===afilter));filterAssignments();}
  const suggestion=el.closest<HTMLElement>('[data-query]');if(suggestion){document.querySelector<HTMLInputElement>('#global-search')!.value=suggestion.dataset.query!;search();document.querySelector<HTMLInputElement>('#global-search')!.focus();}
 });
 document.addEventListener('input',e=>{if((e.target as HTMLElement).id==='global-search')search();if((e.target as HTMLElement).id==='assignment-search')filterAssignments();});
 document.addEventListener('change',e=>{if((e.target as HTMLElement).id==='mobile-toc-select'){const value=(e.target as HTMLSelectElement).value;if(value)document.getElementById(value)?.scrollIntoView({block:'start'});}});
 document.addEventListener('keydown',e=>{if((e.metaKey||e.ctrlKey)&&e.key.toLowerCase()==='k'){e.preventDefault();openSearch();}if(e.key==='Enter'&&(e.target as HTMLElement).id==='global-search'){const link=document.querySelector<HTMLAnchorElement>('.search-result');if(link)link.click();}if(e.key==='Escape')document.body.classList.remove('nav-open');});
 const dialog=document.querySelector<HTMLDialogElement>('.search-dialog')!;
 dialog.addEventListener('click',e=>{if(e.target===dialog){const r=dialog.getBoundingClientRect();if(e.clientX<r.left||e.clientX>r.right||e.clientY<r.top||e.clientY>r.bottom)dialog.close();}});
 dialog.addEventListener('click',e=>{if((e.target as HTMLElement).closest('.search-result'))dialog.close();});
 window.addEventListener('hashchange',()=>void route());
}
async function init(){try{catalog=await getJSON<Catalog>('assets/catalog.json');layout();events();await route();}catch{app.innerHTML='<div class="initial-loading"><h1>暂时无法加载资料库</h1><p>请通过网站地址打开，而非直接双击 HTML 文件。也可以刷新页面重试。</p><button onclick="location.reload()">重新加载</button></div>';}}
void init();
