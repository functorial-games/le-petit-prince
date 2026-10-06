#!/usr/bin/env python3
"""Generate and audit the first pilot surface-topology hypothesis.

Envelope-only baseline fitted to the transverse rings of the v0 control cage.
This is not the biomechanics volume mesh and does not yet cut eye/mouth
apertures.
"""
from __future__ import annotations
import math
from pathlib import Path
import sys
from generate_control_cage import RINGS, RING_SAMPLES as BASE_SAMPLES

HERE=Path(__file__).resolve().parent
def add(a,b): return tuple(x+y for x,y in zip(a,b))
def sub(a,b): return tuple(x-y for x,y in zip(a,b))
def mul(a,s): return tuple(x*s for x in a)
def dot(a,b): return sum(x*y for x,y in zip(a,b))
def cross(a,b): return (a[1]*b[2]-a[2]*b[1],a[2]*b[0]-a[0]*b[2],a[0]*b[1]-a[1]*b[0])
def norm(a): return math.sqrt(dot(a,a))

def build_grid(refine=1):
    ntheta=BASE_SAMPLES*refine
    rows=[]
    for band in range(len(RINGS)-1):
        a,b=RINGS[band],RINGS[band+1]
        for s in range(refine):
            t=s/refine
            rows.append(tuple(a[k]*(1-t)+b[k]*t for k in range(1,5)))
    a=RINGS[-1]; rows.append(tuple(a[k] for k in range(1,5)))
    vertices=[]
    for y,rx,rz,zc in rows:
        for i in range(ntheta):
            th=2*math.pi*i/ntheta
            vertices.append((rx*math.sin(th),y,zc+rz*math.cos(th)))
    quads=[]
    for j in range(len(rows)-1):
        for i in range(ntheta):
            i1=(i+1)%ntheta
            q=[j*ntheta+i,j*ntheta+i1,(j+1)*ntheta+i1,(j+1)*ntheta+i]
            pa,pb,pc,pd=(vertices[k] for k in q)
            n=cross(sub(pb,pa),sub(pc,pa)); ctr=mul(add(add(pa,pb),add(pc,pd)),.25)
            if dot(n,(ctr[0],0,ctr[2]))<0: q=[q[0],q[3],q[2],q[1]]
            quads.append(tuple(q))
    return vertices,quads,ntheta,len(rows)

def tris(q,mode):
    a,b,c,d=q
    return ((a,b,c),(a,c,d)) if mode=="A" else ((a,b,d),(b,c,d))
def tri_area(a,b,c): return .5*norm(cross(sub(b,a),sub(c,a)))
def tri_min_angle(a,b,c):
    L=(norm(sub(b,a)),norm(sub(c,b)),norm(sub(a,c))); best=180.
    for aa,bb,cc in ((L[0],L[1],L[2]),(L[1],L[2],L[0]),(L[2],L[0],L[1])):
        co=max(-1.,min(1.,(aa*aa+bb*bb-cc*cc)/(2*aa*bb)))
        best=min(best,math.degrees(math.acos(co)))
    return best
def surface_area(v,q,mode): return sum(tri_area(v[a],v[b],v[c]) for Q in q for a,b,c in tris(Q,mode))
def quality(v,q,mode):
    x=[tri_min_angle(v[a],v[b],v[c]) for Q in q for a,b,c in tris(Q,mode)]
    return min(x),sum(x)/len(x)
def topo_counts(q):
    counts={}
    for Q in q:
        for a,b in zip(Q,Q[1:]+Q[:1]):
            e=tuple(sorted((a,b))); counts[e]=counts.get(e,0)+1
    return sum(x==1 for x in counts.values()),sum(x==2 for x in counts.values()),sum(x>2 for x in counts.values())
def symmetry(v,nt,nr):
    worst=0.
    for j in range(nr):
        for i in range(nt):
            k=(-i)%nt;p=v[j*nt+i];q=v[j*nt+k]
            worst=max(worst,norm((p[0]+q[0],p[1]-q[1],p[2]-q[2])))
    return worst

def metrics():
    v,q,nt,nr=build_grid(); boundary,interior,nonmanifold=topo_counts(q)
    total_a=total_b=worst=0.
    for Q in q:
        aa=sum(tri_area(v[a],v[b],v[c]) for a,b,c in tris(Q,"A"))
        bb=sum(tri_area(v[a],v[b],v[c]) for a,b,c in tris(Q,"B"))
        worst=max(worst,abs(aa-bb)/max(aa,bb,1e-15));total_a+=aa;total_b+=bb
    qa=quality(v,q,"A");qb=quality(v,q,"B")
    m={"base_vertices":len(v),"base_quads":len(q),"boundary_edges":boundary,"interior_edges":interior,
       "nonmanifold_edges":nonmanifold,"symmetry_max_error":symmetry(v,nt,nr),
       "triangulation_A_area":total_a,"triangulation_B_area":total_b,
       "triangulation_total_area_relative_delta":abs(total_a-total_b)/max(total_a,total_b),
       "triangulation_worst_quad_area_relative_delta":worst,
       "triangulation_A_min_angle_deg":qa[0],"triangulation_A_mean_min_angle_deg":qa[1],
       "triangulation_B_min_angle_deg":qb[0],"triangulation_B_mean_min_angle_deg":qb[1]}
    prev=None
    for f in (1,2,4,8):
        vv,qq,_,_=build_grid(f);a=surface_area(vv,qq,"A")
        m[f"refine_{f}_area"]=a;m[f"refine_{f}_vertices"]=len(vv);m[f"refine_{f}_quads"]=len(qq)
        if prev is not None:m[f"refine_{f}_relative_area_change"]=abs(a-prev)/a
        prev=a
    return m

def obj(v,q,mode=None):
    out=["# Pilot surface topology v1 envelope baseline"]
    out += [f"v {x:.8f} {y:.8f} {z:.8f}" for x,y,z in v];out.append("g envelope")
    if mode is None: out += ["f "+" ".join(str(i+1) for i in Q) for Q in q]
    else:
        for Q in q:
            out += ["f "+" ".join(str(i+1) for i in T) for T in tris(Q,mode)]
    return "\n".join(out)+"\n"

def outputs():
    v,q,_,_=build_grid();m=metrics()
    tsv="metric\tvalue\n"+"\n".join(f"{k}\t{v:.12g}" if isinstance(v,float) else f"{k}\t{v}" for k,v in m.items())+"\n"
    report=f"""# Surface topology v1 numerical report

Envelope-only baseline. It has {m['base_vertices']} vertices and {m['base_quads']} quads.
Nonmanifold edges: {m['nonmanifold_edges']}. Boundary edges: {m['boundary_edges']}.
Total-area diagonal sensitivity: {m['triangulation_total_area_relative_delta']:.3g}.
Worst single-quad diagonal area sensitivity: {m['triangulation_worst_quad_area_relative_delta']:.3g}.
Minimum triangle angle: {min(m['triangulation_A_min_angle_deg'],m['triangulation_B_min_angle_deg']):.3f} degrees.
4x -> 8x refinement area change: {m['refine_8_relative_area_change']:.3g}.

This passes the baseline numerical checks but is not accepted facial topology:
eye/mouth apertures and local facial patch structure still have to be introduced.
"""
    return {"pilot-surface-envelope-v1-quads.obj":obj(v,q),
            "pilot-surface-envelope-v1-tri-A.obj":obj(v,q,"A"),
            "pilot-surface-envelope-v1-tri-B.obj":obj(v,q,"B"),
            "surface-topology-v1-metrics.tsv":tsv,
            "surface-topology-v1-report.md":report}

def check_thresholds(m):
    assert m["boundary_edges"]==2*BASE_SAMPLES
    assert m["nonmanifold_edges"]==0
    assert m["symmetry_max_error"]<1e-12
    assert min(m["triangulation_A_min_angle_deg"],m["triangulation_B_min_angle_deg"])>5
    assert m["triangulation_total_area_relative_delta"]<1e-6
    assert m["triangulation_worst_quad_area_relative_delta"]<.005
    assert m["refine_8_relative_area_change"]<.001

def main():
    generated=outputs();check_thresholds(metrics())
    if "--check" in sys.argv:
        bad=[n for n,t in generated.items() if not (HERE/n).exists() or (HERE/n).read_text()!=t]
        if bad:
            print("surface-topology drift: "+", ".join(bad),file=sys.stderr);return 1
        print("pilot surface topology v1: generated files and thresholds pass");return 0
    for n,t in generated.items():(HERE/n).write_text(t)
    print(generated["surface-topology-v1-metrics.tsv"],end="")
    return 0
if __name__=="__main__":raise SystemExit(main())
