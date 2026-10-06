# Stitching plan after feature topology v2

The v1 envelope and v2 feature patches deliberately remain separate.

## Why not weld by nearest vertex

Nearest-neighbor welding would make the first discretization accident determine anatomy:

- eye outer loops would inherit arbitrary envelope latitude/longitude edges;
- the mouth corner would lose its explicit modiolus pole;
- nasolabial resolution would be driven by whichever envelope vertices happen to be close;
- fat-compartment and retaining-structure boundaries would become difficult to distinguish from mesh artifacts.

## v3 connected frontal sheet

The next candidate should construct a connected frontal sheet with the following fixed interfaces:

1. preserve the two inner eyelid aperture loops;
2. preserve the inner lip aperture loop;
3. preserve a filled modiolus pole on each side;
4. connect the nasolabial strips into the cheek/perioral sheet;
5. use the outer eye and mouth loops as patch interfaces, not as biological boundaries;
6. place unavoidable poles or triangles away from eyelid/lip contact and away from expected high-strain folds when possible.

Triangles are allowed where they solve an actual topological transition. Quads are preferred around ring-like apertures and directional strips because they give clear circumferential/radial or longitudinal/transverse coordinates.

## Required tests

A v3 stitch is not accepted until it has:

- one connected frontal component;
- exactly three physical aperture boundary loops: left eye, right eye, mouth;
- zero nonmanifold edges;
- bilateral symmetry on the symmetric reference geometry;
- no inverted faces;
- alternate quad-diagonal sensitivity at or below the v2 local-patch level in the feature zones;
- mesh-refinement convergence;
- explicit zone labels linking topology to the fat/retaining-structure ledgers;
- a documented map from the connected surface to a separately chosen biomechanics volume mesh.

The surface topology remains a display/control discretization unless and until a volume formulation proves otherwise.
