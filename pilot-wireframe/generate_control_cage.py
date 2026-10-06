#!/usr/bin/env python3
"""Generate the pilot identity/control wire cage.

This is deliberately a line cage, not a surface or volume mesh. It records
identity/silhouette/feature constraints without prematurely choosing a
triangulation, quad layout, or finite-element discretization.

Coordinates are unitless:
  x: left/right
  y: inferior/superior
  z: posterior/anterior
The face looks toward +z.

All depth values in v0 are hypotheses constrained only weakly by the current
drawings. See parameters.tsv and reference-observations.md.
"""
from __future__ import annotations
import math
from pathlib import Path
import sys

HERE = Path(__file__).resolve().parent

RINGS = [
    ("apex", 1.18, 0.20, 0.24, -0.10),
    ("crown", 1.08, 0.52, 0.54, -0.04),
    ("upper_forehead", 0.82, 0.69, 0.68, 0.00),
    ("brow", 0.52, 0.77, 0.74, 0.02),
    ("eye", 0.24, 0.83, 0.79, 0.04),
    ("malar", 0.00, 0.94, 0.84, 0.07),
    ("cheek", -0.27, 0.98, 0.86, 0.09),
    ("mouth", -0.50, 0.92, 0.81, 0.10),
    ("jowl", -0.72, 0.86, 0.74, 0.08),
    ("jaw", -0.90, 0.72, 0.65, 0.04),
    ("chin", -1.02, 0.50, 0.56, 0.02),
]
RING_SAMPLES = 24

PARAMETER_ROWS = [
    ("coordinate_scale", "unitless", "head identity cage", "free", "absolute head size is not constrained by the drawings"),
    ("cheek_half_width", "0.98", "final heavy-pilot drawings", "moderate", "broadest facial region; deliberately wider than brow"),
    ("brow_half_width", "0.77", "final heavy-pilot drawings", "moderate", "relative frontal width only"),
    ("jowl_half_width", "0.86", "final heavy-pilot drawings", "moderate", "heavy lower face remains broad below mouth"),
    ("chin_half_width", "0.50", "final heavy-pilot drawings", "moderate", "chin narrows relative to jowl"),
    ("cheek_half_depth", "0.86", "three-quarter appearance", "low", "depth is underdetermined; keep parameter free"),
    ("nose_tip_extra_depth", "0.32", "three-quarter appearance", "low", "strong nose projection, not photogrammetric measurement"),
    ("orbit_center_x", "0.32", "final heavy-pilot drawings", "moderate", "bilateral neutral identity prior"),
    ("orbit_radius_x", "0.235", "final heavy-pilot drawings", "moderate", "orbital guide, not eyeball radius"),
    ("orbit_radius_y", "0.155", "final heavy-pilot drawings", "moderate", "heavy lid identity will be a later surface/tissue property"),
    ("mouth_radius_x", "0.34", "final heavy-pilot drawings", "moderate", "outer perioral guide; mustache not part of cage"),
    ("mouth_radius_y", "0.105", "final heavy-pilot drawings", "low", "expression in references makes exact neutral aperture uncertain"),
]

def interp(y: float, column: int) -> float:
    rows = sorted(RINGS, key=lambda r: r[1])
    if y <= rows[0][1]:
        return rows[0][column]
    if y >= rows[-1][1]:
        return rows[-1][column]
    for a, b in zip(rows, rows[1:]):
        if a[1] <= y <= b[1]:
            t = (y-a[1])/(b[1]-a[1])
            return a[column]*(1-t)+b[column]*t
    raise AssertionError

def front_z(y: float, x: float=0.0) -> float:
    rx = interp(y, 2)
    rz = interp(y, 3)
    zc = interp(y, 4)
    q = max(0.0, 1.0-(x/rx)**2)
    return zc + rz*math.sqrt(q)

def build():
    vertices = []
    polylines = []
    def add_point(p):
        vertices.append(tuple(p))
        return len(vertices)
    def add_poly(name, pts, closed=False):
        ids = [add_point(p) for p in pts]
        if closed:
            ids.append(ids[0])
        polylines.append((name, ids))

    ring_ids = {}
    for name,y,rx,rz,zc in RINGS:
        ids=[]
        for i in range(RING_SAMPLES):
            theta=2*math.pi*i/RING_SAMPLES
            ids.append(add_point((rx*math.sin(theta),y,zc+rz*math.cos(theta))))
        ring_ids[name]=ids
        polylines.append((f"ring_{name}", ids+[ids[0]]))
    for i in range(RING_SAMPLES):
        polylines.append((f"longitude_{i:02d}", [ring_ids[r[0]][i] for r in RINGS]))

    for side,label in [(-1,"L"),(1,"R")]:
        cx=side*0.32; cy=0.22
        pts=[]
        for i in range(32):
            a=2*math.pi*i/32
            x=cx+0.235*math.cos(a)
            y=cy+0.155*math.sin(a)
            z=front_z(y,x)+0.025+0.02*math.cos(a)
            pts.append((x,y,z))
        add_poly(f"orbital_rim_{label}",pts,True)
        pts=[]
        for i in range(16):
            t=i/15
            x=cx+(t-.5)*0.50
            y=0.40+0.055*math.cos((t-.5)*math.pi)
            pts.append((x,y,front_z(y,x)+0.035))
        add_poly(f"brow_{label}",pts)

    pts=[]
    for i in range(32):
        a=2*math.pi*i/32
        x=0.34*math.cos(a)
        y=-0.49+0.105*math.sin(a)
        pts.append((x,y,front_z(y,x)+0.055+0.018*math.cos(a)))
    add_poly("mouth_outer",pts,True)

    add_poly("nose_ridge",[
        (0.0,0.44,front_z(0.44)+0.03),
        (0.0,0.27,front_z(0.27)+0.08),
        (0.0,0.08,front_z(0.08)+0.18),
        (0.0,-0.12,front_z(-0.12)+0.32),
        (0.0,-0.23,front_z(-0.23)+0.12),
    ])

    pts=[]
    for i in range(28):
        a=2*math.pi*i/28
        x=0.17*math.cos(a)
        y=-0.18+0.10*math.sin(a)
        pts.append((x,y,front_z(y,x)+0.085+0.030*math.cos(a)))
    add_poly("alar_base",pts,True)

    for side,label in [(-1,"L"),(1,"R")]:
        pts=[]
        for i in range(14):
            t=i/13
            x=side*(0.14+0.18*t)
            y=-0.22-0.26*t
            pts.append((x,y,front_z(y,x)+0.025))
        add_poly(f"nasolabial_{label}",pts)
        pts=[]
        for i in range(18):
            t=i/17; a=-0.55+1.10*t
            x=side*(0.43+0.39*math.cos(a))
            y=-0.15+0.33*math.sin(a)
            pts.append((x,y,front_z(y,x)+0.02))
        add_poly(f"cheek_mass_{label}",pts)

    pts=[]
    for i in range(33):
        t=i/32; x=-0.72+1.44*t
        y=-1.00+0.20*(abs(x)/0.72)**1.6
        pts.append((x,y,front_z(y,x)+0.01))
    add_poly("jawline_front",pts)

    pts=[]
    for i in range(21):
        t=i/20; x=-0.30+0.60*t
        y=-0.73-0.04*math.cos((t-.5)*math.pi*2)
        pts.append((x,y,front_z(y,x)+0.015))
    add_poly("labiomental_guide",pts)
    add_poly("facial_midline",[(0,y,zc+rz) for _,y,_,rz,zc in RINGS])
    return vertices,polylines

def rotate_y(p, angle):
    x,y,z=p; c=math.cos(angle); s=math.sin(angle)
    return c*x+s*z, y, -s*x+c*z

def outputs():
    vertices,polylines=build()
    obj=["# Pilot control cage v0",
         "# Unitless normalized coordinates. x left/right, y inferior/superior, z posterior/anterior.",
         "# Face looks toward +z.",
         "# Line cage only: NOT a surface topology and NOT a biomechanics volume mesh."]
    obj += [f"v {x:.6f} {y:.6f} {z:.6f}" for x,y,z in vertices]
    for name,ids in polylines:
        obj += [f"g {name}", "l "+" ".join(map(str,ids))]
    points=["index\tx\ty\tz"]+[f"{i}\t{x:.6f}\t{y:.6f}\t{z:.6f}" for i,(x,y,z) in enumerate(vertices,1)]
    params=["parameter\tvalue\tsource_class\tconfidence\tnotes"]+["\t".join(r) for r in PARAMETER_ROWS]

    svg=['<svg xmlns="http://www.w3.org/2000/svg" width="1200" height="440" viewBox="0 0 1200 440">',
         '<rect width="100%" height="100%" fill="#11161a"/>',
         '<g fill="none" stroke="#e8dfc6" stroke-width="1.1" stroke-linejoin="round" stroke-linecap="round">']
    panels=[("front",0.0,200),("three-quarter",math.radians(-38),600),("profile",math.radians(-90),1000)]
    for _,angle,cx in panels:
        for _,ids in polylines:
            pts=[]
            for idx in ids:
                x,y,_=rotate_y(vertices[idx-1],angle)
                pts.append(f"{cx+x*155:.2f},{225-y*155:.2f}")
            svg.append('<polyline points="'+" ".join(pts)+'"/>')
    svg += ['</g>','<g fill="#e8dfc6" font-family="monospace" font-size="16">']
    for label,_,cx in panels:
        svg.append(f'<text x="{cx-80}" y="418">{label}</text>')
    svg.append('</g></svg>')

    return {
        "pilot-control-cage-v0.obj":"\n".join(obj)+"\n",
        "pilot-control-cage-v0.points.tsv":"\n".join(points)+"\n",
        "pilot-control-cage-v0.parameters.tsv":"\n".join(params)+"\n",
        "pilot-control-cage-v0.svg":"\n".join(svg)+"\n",
    }

def main():
    generated=outputs()
    if "--check" in sys.argv:
        check_names=("pilot-control-cage-v0.obj","pilot-control-cage-v0.points.tsv","pilot-control-cage-v0.parameters.tsv")
        bad=[name for name in check_names if not (HERE/name).exists() or (HERE/name).read_text()!=generated[name]]
        if not (HERE/"pilot-control-cage-v0.svg").exists():
            bad.append("pilot-control-cage-v0.svg (missing preview)")
        if bad:
            print("control-cage drift: "+", ".join(bad),file=sys.stderr)
            return 1
        print("pilot control cage: generated files match")
        return 0
    for name,text in generated.items():
        (HERE/name).write_text(text)
    print("wrote control cage")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
