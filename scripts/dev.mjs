import {spawnSync} from 'node:child_process';
import fs from 'node:fs';
import path from 'node:path';
const root=path.resolve(import.meta.dirname,'..');
function build(){const r=spawnSync(process.execPath,[path.join(root,'scripts/build.mjs')],{cwd:root,stdio:'inherit'});return r.status===0;}
if(!build())process.exit(1);
await import('./serve.mjs');
let timer;for(const dir of ['src','content'])fs.watch(path.join(root,dir),{recursive:true},()=>{clearTimeout(timer);timer=setTimeout(()=>{if(build())console.log('Rebuilt. Refresh the browser to see changes.');},350);});
