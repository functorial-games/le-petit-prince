# Facial rig notes

Goal: keep the face anatomically rich enough that expressions can be layered instead of replacing one another.

The important distinction is:

- **muscles / anatomical regions** are the deformation model;
- **control points / rig controls** are the animator interface;
- **blend shapes / corrective shapes** handle deformations that bones alone do poorly;
- **high-level expression controls** drive groups of lower-level controls.

Do not equate "number of facial muscles" with "number of animation controls." Anatomical counts vary according to what is included and how muscles are subdivided.

## Muscle coverage

Represent every independently useful facial-expression action, with left/right independence where anatomy is paired.

### Forehead and brow

- frontalis;
- corrugator supercilii;
- procerus;
- depressor supercilii.

Frontalis should have regional control rather than one global scalar so inner and outer brow elevation can differ.

### Eyes

- orbicularis oculi, with palpebral and orbital behavior represented separately where useful;
- upper-eyelid elevation;
- lower-eyelid position/tension;
- eyeball yaw and pitch;
- convergence where needed;
- blink and squint behavior.

The eyelids must follow the eyeball rather than behaving like flat shutters.

### Nose

- nasalis, transverse portion;
- nasalis, alar portion;
- depressor septi nasi;
- levator labii superioris alaeque nasi.

Provide nostril flare/compression and small asymmetric nose movement.

### Upper lip and cheek

- levator labii superioris;
- levator anguli oris;
- zygomaticus major;
- zygomaticus minor.

### Mouth corners and lateral mouth

- risorius;
- depressor anguli oris.

### Lower lip and chin

- depressor labii inferioris;
- mentalis.

### Cheek

- buccinator.

### Lips

- orbicularis oris.

Do not treat orbicularis oris as one control. Divide useful action into upper/lower and left/right regions, with controls for purse, press, protrude, narrow and relax.

### Jaw and mastication

- masseter;
- temporalis;
- medial pterygoid;
- lateral pterygoid.

Represent jaw opening, closing, protrusion, retrusion and lateral motion.

### Neck contribution

- platysma.

Platysma matters for age, strain, fear, fatigue and the transition from slack exhaustion to startled tension.

### Optional speech / oral detail

If close-up speech becomes important, add tongue, floor-of-mouth and hyoid-related controls as a separate speech subsystem rather than pretending they are facial-expression muscles.

## Regional actuators

A literal one-slider-per-named-muscle rig is too crude. Broad structures should be split into independently controllable regions, especially:

- frontalis;
- orbicularis oculi;
- orbicularis oris;
- buccinator;
- platysma;
- eyelids.

Once left/right independence and regional subdivisions are included, the deformation layer can reasonably contain many dozens of actuators. A rough design target of 70–100 low-level actuators is plausible, but this is a rig-design count, **not** a claim that the human face has 70–100 distinct named muscles.

## High-level controls

Animation code should not manipulate every actuator individually. Build high-level controls that distribute motion into the lower layer.

Useful controls for this character include:

- exhausted neutral;
- eyelid heaviness;
- brow fatigue;
- cheek slackness;
- mouth-corner droop;
- jaw slackness;
- blink;
- squint;
- eye widen;
- inner-brow raise;
- outer-brow raise;
- brow knit;
- jaw drop;
- lip part;
- nostril flare;
- cheek tension;
- mustache follow-through;
- left/right eye aim;
- head tilt;
- speech visemes if dialogue is added.

## Pilot-specific expression composition

### Exhaustion / depression base

Keep active:

- upper lids lowered;
- cheeks and lower face relatively slack;
- mouth corners slightly depressed;
- jaw slightly slack;
- inner brow shape suggesting fatigue rather than anger;
- head and neck carried low;
- shoulders rounded.

### Astonishment layer

Add on top:

- strong upper-eyelid raise;
- eye widen;
- brow elevation, potentially stronger centrally;
- slight lower-lid change rather than a generic round-eye cartoon;
- jaw opening;
- lips parting;
- small neck/platysma tension;
- head lift toward the Prince.

Do **not** zero the exhaustion controls when astonishment activates.

Conceptually:

```
pilot_sees_prince =
    exhaustion_base
  + sudden_eye_widen
  + brow_raise
  + slight_jaw_drop
  + head_lift
```

The visual target is: **his whole body still looks tired of life, while his eyes abruptly look as though the impossible has appeared in front of him.**

## Mustache

Treat the large black mustache as part of the facial performance, not a rigid decal.

It should:

- move with the upper lip;
- lag slightly during sudden jaw/head motion;
- compress when the upper lip rises;
- preserve its heavy silhouette;
- avoid hiding all mouth-corner information.

A few secondary controls or lightweight dynamics are sufficient.

## Relation to FACS

A later implementation can map the rig to Facial Action Coding System action units. FACS is useful as a descriptive bridge between anatomy and expressions, but production controls do not need a one-to-one correspondence with action units.

The priority is expressive independence: the rig must be able to combine tired lids, drooping mouth, asymmetry, widened eyes, raised brows and jaw opening without one preset erasing another.
