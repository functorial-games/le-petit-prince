#!/usr/bin/env python3
"""Generate/test pilot facial feature topology v2.

This is a local topology laboratory, not a connected face mesh. It creates
annular quad patches around the two eye apertures and the mouth aperture,
quad strips along the nasolabial regions, and filled hybrid triangle/quad
patches at the modioli.

No external Python packages are required.
"""
from __future__ import annotations
import math
from pathlib import Path
import sys
from generate_control_cage import front_z

HERE=Path(__file__).resolve().parent

def sub(a,b): return tuple(x-y for x,y in zip(a,b))
def cross(a,b): return (a[1]*b[2]-a[2]*b[1],a[2]*b[0]-a[0]*b[2],a[0]*b[1]-a[1]*b[0])
def norm(a): return math.sqrt(sum(x*x for x in a))
def area(a,b,c): return .5*norm(cross(sub(b,a),sub(c,a)))

def annulus(cx,cy,rings,n):
    vertices=[]
    for rx,ry in rings:
        for i in range(n):
            a=2*math.pi*i/n
            x=cx+rx*math.cos(a);y=cy+ry*math.sin(a)
            vertices.append((x,y,front_z(y,x)+.01))
    quads=[]
    for j in range(len(rings)-1):
        for i in range(n):
            k=(i+1)%n
            quads.append((j*n+i,(j+1)*n+i,(j+1)*n+k,j*n+k))
    return orient({"vertices":vertices,"quads":quads,"triangles":[]})

def bezier(p0,p1,p2,t):
    return ((1-t)**2*p0[0]+2*(1-t)*t*p1[0]+t*t*p2[0],(1-t)**2*p0[1]+2*(1-t)*t*p1[1]+t*t*p2[1])
def bezier_d(p0,p1,p2,t):
    return (2*(1-t)*(p1[0]-p0[0])+2*t*(p2[0]-p1[0]),2*(1-t)*(p1[1]-p0[1])+2*t*(p2[1]-p1[1]))

def nasolabial_right(nlong=14,widths=(-.045,0,.045)):
    p0=(.16,-.20);p1=(.24,-.30);p2=(.34,-.47);vertices=[]
    for w in widths:
        for i in range(nlong):
            t=i/(nlong-1);x,y=bezier(p0,p1,p2,t);dx,dy=bezier_d(p0,p1,p2,t);L=math.hypot(dx,dy)
            nx,ny=-dy/L,dx/L;xx=x+w*nx;yy=y+w*ny
            vertices.append((xx,yy,front_z(yy,xx)+.015))
    quads=[]
    for j in range(len(widths)-1):
        for i in range(nlong-1):
            quads.append((j*nlong+i,(j+1)*nlong+i,(j+1)*nlong+i+1,j*nlong+i+1))
    return orient({"vertices":vertices,"quads":quads,"triangles":[]})

def modiolus_right(n=8):
    cx=.34;cy=-.47;inner=(.04,.03);outer=(.09,.07)
    vertices=[(cx,cy,front_z(cy,cx)+.012)]
    for rx,ry in (inner,outer):
        for i in range(n):
            a=2*math.pi*i/n;x=cx+rx*math.cos(a);y=cy+ry*math.sin(a)
            vertices.append((x,y,front_z(y,x)+.012))
    triangles=[];quads=[];off=1+n
    for i in range(n):
        k=(i+1)%n;triangles.append((0,1+i,1+k));quads.append((1+i,off+i,off+k,1+k))
    return orient({"vertices":vertices,"quads":quads,"triangles":triangles})

def orient(p):
    face=(p["triangles"] or p["quads"])[0]
    a,b,c=(p["vertices"][i] for i in face[:3])
    if cross(sub(b,a),sub(c,a))[2]<0:
        p["triangles"]=[(t[0],t[2],t[1]) for t in p["triangles"]]
        p["quads"]=[(q[0],q[3],q[2],q[1]) for q in p["quads"]]
    return p

def mirror(p):
    return {"vertices":[(-x,y,z) for x,y,z in p["vertices"]],
            "triangles":[(t[0],t[2],t[1]) for t in p["triangles"]],
            "quads":[(q[0],q[3],q[2],q[1]) for q in p["quads"]]}

def build():
    eye_r=annulus(.32,.20,((.19,.075),(.235,.11),(.30,.16)),24)
    mouth=annulus(0,-.48,((.25,.05),(.30,.08),(.38,.14)),20)
    mod_r=modiolus_right();naso_r=nasolabial_right()
    return [("eye_L",mirror(eye_r)),("eye_R",eye_r),("mouth",mouth),
            ("modiolus_L",mirror(mod_r)),("modiolus_R",mod_r),
            ("nasolabial_L",mirror(naso_r)),("nasolabial_R",naso_r)]

def tri_from_quad(q,mode):
    a,b,c,d=q
    return ((a,b,c),(a,c,d)) if mode=="A" else ((a,b,d),(b,c,d))

def min_angle(a,b,c):
    L=(norm(sub(b,a)),norm(sub(c,b)),norm(sub(a,c)));best=180.
    for aa,bb,cc in ((L[0],L[1],L[2]),(L[1],L[2],L[0]),(L[2],L[0],L[1])):
        co=max(-1.,min(1.,(aa*aa+bb*bb-cc*cc)/(2*aa*bb)))
        best=min(best,math.degrees(math.acos(co)))
    return best

def topology(p):
    counts={}
    for f in p["triangles"]+p["quads"]:
        for a,b in zip(f,f[1:]+f[:1]):
            e=tuple(sorted((a,b)));counts[e]=counts.get(e,0)+1
    adj={}
    nonmanifold=0
    for (a,b),c in counts.items():
        if c==1:
            adj.setdefault(a,[]).append(b);adj.setdefault(b,[]).append(a)
        elif c>2: nonmanifold+=1
    assert all(len(n)==2 for n in adj.values())
    loops=0;seen=set()
    for s in adj:
        if s in seen:continue
        loops+=1;stack=[s];seen.add(s)
        while stack:
            x=stack.pop()
            for n in adj[x]:
                if n not in seen:seen.add(n);stack.append(n)
    return sum(c==1 for c in counts.values()),loops,nonmanifold

def metrics(p):
    modes={}
    for mode in ("A","B"):
        angles=[];total=0.
        for t in p["triangles"]:
            v=[p["vertices"][i] for i in t];total+=area(*v);angles.append(min_angle(*v))
        for q in p["quads"]:
            for t in tri_from_quad(q,mode):
                v=[p["vertices"][i] for i in t];total+=area(*v);angles.append(min_angle(*v))
        modes[mode]=(total,min(angles),sum(angles)/len(angles))
    worst=0.
    for q in p["quads"]:
        aa=sum(area(*(p["vertices"][i] for i in t)) for t in tri_from_quad(q,"A"))
        bb=sum(area(*(p["vertices"][i] for i in t)) for t in tri_from_quad(q,"B"))
        worst=max(worst,abs(aa-bb)/max(aa,bb,1e-15))
    boundary,loops,nonmanifold=topology(p)
    return modes,worst,boundary,loops,nonmanifold

def combined_obj(mode=None):
    out=[f"# Pilot facial feature topology v2{(' triangulation '+mode) if mode else ' quad-dominant mixed topology'}",
         "# Seven deliberate local patches; NOT yet stitched into the head envelope."]
    offset=0
    for name,p in build():
        out.append("g "+name)
        out.extend(f"v {x:.6f} {y:.6f} {z:.6f}" for x,y,z in p["vertices"])
        out.extend("f "+" ".join(str(i+1+offset) for i in t) for t in p["triangles"])
        if mode:
            for q in p["quads"]:
                out.extend("f "+" ".join(str(i+1+offset) for i in t) for t in tri_from_quad(q,mode))
        else:
            out.extend("f "+" ".join(str(i+1+offset) for i in q) for q in p["quads"])
        offset+=len(p["vertices"])
    return "\n".join(out)+"\n"

def svg():
    W=760;H=700;scale=320;cx=380;cy=330
    out=[f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" viewBox="0 0 {W} {H}">',
         '<rect width="100%" height="100%" fill="#11161a"/>',
         '<g fill="none" stroke="#e8dfc6" stroke-width="1.1">']
    for _,p in build():
        edges=set()
        for f in p["triangles"]+p["quads"]:
            for a,b in zip(f,f[1:]+f[:1]):edges.add(tuple(sorted((a,b))))
        for a,b in sorted(edges):
            p0=p["vertices"][a];p1=p["vertices"][b]
            out.append(f'<line x1="{cx+p0[0]*scale:.2f}" y1="{cy-p0[1]*scale:.2f}" x2="{cx+p1[0]*scale:.2f}" y2="{cy-p1[1]*scale:.2f}"/>')
    out+=['</g>','<g fill="#e8dfc6" font-family="monospace" font-size="15"><text x="20" y="28">feature topology v2 — local patches, not stitched</text></g></svg>']
    return "\n".join(out)+"\n"

def check_thresholds():
    expected={"eye_L":2,"eye_R":2,"mouth":2,"modiolus_L":1,"modiolus_R":1,"nasolabial_L":1,"nasolabial_R":1}
    global_min=180.;global_worst=0.
    for name,p in build():
        mm,worst,_,loops,nm=metrics(p)
        assert nm==0 and loops==expected[name]
        global_min=min(global_min,mm["A"][1],mm["B"][1]);global_worst=max(global_worst,worst)
    assert global_min>12.
    assert global_worst<.005

def outputs():
    return {"pilot-feature-topology-v2-quads.obj":combined_obj(),
            "pilot-feature-topology-v2-tri-A.obj":combined_obj("A"),
            "pilot-feature-topology-v2-tri-B.obj":combined_obj("B"),
            "pilot-feature-topology-v2.svg":svg()}

def main():
    check_thresholds();generated=outputs()
    if "--check" in sys.argv:
        check_names=("pilot-feature-topology-v2-quads.obj","pilot-feature-topology-v2-tri-A.obj","pilot-feature-topology-v2-tri-B.obj")
        bad=[n for n in check_names if not (HERE/n).exists()]
        if not (HERE/"pilot-feature-topology-v2.svg").exists():
            bad.append("pilot-feature-topology-v2.svg (missing preview)")
        if bad:
            print("feature-topology drift: "+", ".join(bad),file=sys.stderr);return 1
        print("pilot feature topology v2: generated files and thresholds pass");return 0
    for n,t in generated.items():(HERE/n).write_text(t)
    print("pilot feature topology v2: generated")
    return 0
if __name__=="__main__":raise SystemExit(main())
